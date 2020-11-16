/*******************************************************************************
**	File:
**
**		06_SEM.c
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
OS_EVENT *sem_id;	/* Semaphore Pointer */

extern INT32U USE_OSTimeTickHook;

/* ...........................................................................
 *
 * 태스크 함수 원형
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  spost(void *pdata);
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
	INT8U err;
	int sem_init;
	OS_SEM_DATA sem_val;	// semaphore control
	int loops;

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */
	
	USE_OSTimeTickHook = 0; 
	
	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

	// 세마포어 초기값
	sem_init = 1;

	/* TODO #1: 
		create a binary semaphore
	    use sem_id and sem_init */
#if 0

#endif // TODO #1

	// 세마포어 쿼리
    err = OSSemQuery(sem_id, &sem_val);
    if (err) printf("OSSemQuery error found, code[0x%X]\n",err);

	strcpy(TaskUserData[TASK_1_ID].TaskName, "spost");

	// TASK CREATE
	OSTaskCreateExt(spost,
	                (void *)0,
	                &Task1Stk[TASK_STK_SIZE - 1],
	                TASK_1_PRIO,
	                TASK_1_ID,
	                &Task1Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_1_ID],
	                0);
	// loop counter
	loops = 10;
    for(;;) {
		// 세마포어 대기
		OSSemPend(sem_id, 0, &err);
		printf("."); fflush(stdout);
    	err = OSSemQuery(sem_id, &sem_val);
    	if (err) printf("OSSemQuery error found, code[0x%X]\n",err);

		if(loops == 0)
			break; // exit

		loops--;
	}

	/* TODO #3: 
		delete a semaphore */
#if 0

#endif // TODO #3

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// 태스크 'spost'
//
void spost(void *pdata)
{
	INT8U err;
	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

    printf("spost TASK created,\n");

	for(;;) {
	/* TODO #2: 
		post to a semaphore */
#if 0

#endif // TODO #2
        if (err) printf("OSSemPost error found, code[0x%X]\n",err);
        OSTimeDly(100);
 	}
}

/*-----------------------------------------------------------------------------
 * Program : 06_SEM.C
-----------------------------------------------------------------------------*/
