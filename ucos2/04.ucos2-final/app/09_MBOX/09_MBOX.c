/*******************************************************************************
**	File:
**
**		09_MBOX.c
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
//OS_STK Task2Stk[TASK_STK_SIZE]; /* Task #2 stack */
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
OS_EVENT *mbox;	/* Mailbox Pointer */

/* ...........................................................................
 *
 * 태스크 함수 원형
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  Task1(void *pdata);
void  Task2(void *pdata);          
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
	void *msg;
//	long timeout= 0L;			/* Wait Forever */
	long timeout= 300L;			/* 3sec */
	INT8U err;
	OS_MBOX_DATA data;

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

	/* TODO #1: 
		create a Mailbox
	    use mbox and initial value= 0*/
#if 0

#endif // TODO #1

	// 메일박스 쿼리
    err = OSMboxQuery(mbox, &data);
    if (err) printf("OSMboxQuery error found, code[0x%X]\n",err);

	strcpy(TaskUserData[TASK_1_ID].TaskName, "Task1");

	// TASK CREATE
	OSTaskCreateExt(Task1,
	                (void *)0,
	                &Task1Stk[TASK_STK_SIZE - 1],
	                TASK_1_PRIO,
	                TASK_1_ID,
	                &Task1Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_1_ID],
	                0);

	printf("\nretrieve a message from mailbox\n");

	for(;;) {
		printf("Please wait...");
		msg = OSMboxPend(mbox, timeout, &err);
		
		// check err code
		if (err)
		{
			if (err == OS_TIMEOUT)
				printf("TIMEOUT\n");
			else
				printf("Unknown Error!\n");
		}
		else
			printf("got it %s\n\n",(char*)msg);
	}
}

//
// 태스크 1
//
void Task1(void *pdata)
{
    INT16U count;
    char s[81];
	int err, i;
   	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

    count = 0;
	for(i=1; i< 10000; i++) {
        sprintf(s, "%05d", count);

	/* TODO #2: 
		post message s[] to TASK1 */
#if 0

#endif // TODO #2

	   	OSTimeDly(100);	/* 1 Sec Delay */
        count++;
	}
}

/*-----------------------------------------------------------------------------
 * Program : 09_MBOX.C
-----------------------------------------------------------------------------*/
