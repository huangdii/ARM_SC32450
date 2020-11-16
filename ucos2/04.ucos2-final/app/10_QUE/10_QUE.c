/*******************************************************************************
**	File:
**
**		10_QUE.c
**
**  Contents:
**		uC/OS-II programming
**
**	History:
**		Date        Name              Description
**		------------------------------------------------------------------------
**
*******************************************************************************/
#include "includes.h"
	
/* ...........................................................................
 *
 * 태스크 스택 정의
 * ===================
 */
OS_STK TaskStartStk[TASK_STK_SIZE]; /* START task stack */
OS_STK Task1Stk[TASK_STK_SIZE]; /* Task #1 stack */
OS_STK Task2Stk[TASK_STK_SIZE]; /* Task #2 stack */
//OS_STK Task3Stk[TASK_STK_SIZE]; /* Task #3 stack */
//OS_STK Task4Stk[TASK_STK_SIZE]; /* Task #4 stack */

/* ...........................................................................
 *
 * 사용자 메모리 정의
 * ===================
 */
TASK_USER_DATA  TaskUserData[7];

/* ...........................................................................
 *
 * 이벤트 컨트롤 & 사용자 정의 블럭 정의
 * ===================
 */
OS_EVENT *qid;	/* Message Queue Pointer */
void *qBuffer[10];

extern INT32U USE_OSTimeTickHook;

/* ...........................................................................
 *
 * 태스크 함수 원형
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  qpend(void *pdata);
void  qpost(void *pdata);          
void  Task3(void *pdata);
void  Task4(void *pdata);

//
// 어플리케이션 메인 루틴
//
int main(void)
{
	OSInit(); /* microC/OS-II 초기화 */

	strcpy(TaskUserData[TASK_START_ID].TaskName, "TaskStart");
    
	// TASK CREATE
	OSTaskCreateExt(TaskStart,
	               (void *)0,
	               &TaskStartStk[TASK_STK_SIZE-1],
	               TASK_START_PRIO,
	               TASK_START_ID,
	               &TaskStartStk[0],
	               TASK_STK_SIZE,
	               &TaskUserData[TASK_START_ID],
	               OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSStart(); /* start scheduler */

	return 0;    
}

//
// 태스크 'START'
//
void  TaskStart (void *pdata)
{
	void qpend(void *pdata), qpost(void *pdata);
//	long timeout= 200L;			/* Wait Forever */
	INT8U err;
	char *msg;
	OS_Q_DATA qdata;
	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	USE_OSTimeTickHook = 0;
	
	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

	/* TODO #1: 
		create a Queue
	    use qid and qBuffer[] */
#if 0

#endif // TODO #1

	// 큐(QUEUE) 쿼리 
    err = OSQQuery(qid, &qdata);
    if (err) printf("OSQQuery error found, code[0x%X]\n",err);

	strcpy(TaskUserData[TASK_1_ID].TaskName, "qpend");

	// TASK CREATE
	OSTaskCreateExt(qpend,
	                (void *)0,
	                &Task1Stk[TASK_STK_SIZE - 1],
	                TASK_1_PRIO,
	                TASK_1_ID,
	                &Task1Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_1_ID],
	                0);

	strcpy(TaskUserData[TASK_2_ID].TaskName, "qpost");

	// TASK CREATE
	OSTaskCreateExt(qpost,
	                (void *)0,
	                &Task2Stk[TASK_STK_SIZE - 1],
	                TASK_2_PRIO,
	                TASK_2_ID,
	                &Task2Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_2_ID],
	                0);

	OSTimeDly(500);

	/* TODO #2: 
		broadcast a message to all TASKs
	    message : "Do you hear me?[john said]" */
#if 0

#endif // TODO #2

	printf("\n.\n.\n.\n.\non AIR...\n.\n.\n.\n.\n");

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// 태스크 'qpend'
//
void qpend(void *pdata)
{
	INT8U err;
    char *p;
	long timeout= 200L;			/* for ever */
	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	printf("[qpend]waiting...\n");

	// 메시지 큐 '브로드캐스트' 대기
    p = OSQPend(qid, 0, &err); /* Wake up By Queue Broadcast */
    if (err) printf("OSQPend error found, code[%d]\n",err);
	printf("[qpend]sync %s\n",p);

	for(;;) {
		// 메시지 큐 대기
		p = OSQPend(qid, timeout, &err);
		if (err)
			printf("OSQPend error found, code[0x%X]\n",err);
		else
			printf("get message \"%d\" from TASK qpost\n",*p);
	}
}

//
// 태스크 'qpost'
//
void qpost(void *pdata)
{
	INT8U err;
    char *p;
	char msg[80];
	int i;

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	printf("[qpost]waiting...\n");

	// 메시지 큐 '브로드캐스트' 대기
    p = OSQPend(qid, 0, &err); /* Wake up By Queue Broadcast */
    if (err) printf("OSQPend error found, code[%d]\n",err);
	printf("[qpost]sync %s\n",p);

    for(i=1; i< 10000; i++) {
    	msg[0] = i; msg[1]= NULL;

	/* TODO #3: 
		post a message to TASK qpend
	    use msg[] */
#if 0

#endif // TODO #3

		OSTimeDly(100);
	}
}

/*-----------------------------------------------------------------------------
 * Program : 10_QUE.C
-----------------------------------------------------------------------------*/
