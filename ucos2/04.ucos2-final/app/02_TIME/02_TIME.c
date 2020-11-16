/*******************************************************************************
**	File:
**
**		02_TIMER.c
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
struct tm {
  int tm_msec;  /* milli seconds */
  int tm_sec;   /* seconds after the minute, 0 to 60
                   (0 - 60 allows for the occasional leap second) */
  int tm_min;   /* minutes after the hour, 0 to 59 */
  int tm_hour;  /* hours since midnight, 0 to 23 */
};
// 1초마다 콘솔에 동작중을 표시(.)
extern INT32U USE_OSTimeTickHook;

void OSTimeDlyCustom( struct tm *t );

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

    PC_ElapsedInit(); /* Initialized elapsed time measurement     */

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
	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

	// 1초마다 콘솔에 동작중을 표시(.)
	USE_OSTimeTickHook = 1; // refer to 'OS_CPU_C.C'

    strcpy(TaskUserData[TASK_1_ID].TaskName, "Task1");
    OSTaskCreateExt(Task1,
                    (void *)0,
                    &Task1Stk[TASK_STK_SIZE - 1],
                    TASK_1_PRIO,
                    TASK_1_ID,
                    &Task1Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_1_ID],
                    0);

	if (err) printf("OSTaskCreate error found, code[0x%X]\n",err);

#if 0 // No comment
	err = OSTaskSuspend(OS_PRIO_SELF);
    	if (err) printf("OSTaskSuspend error found, code[0x%X]\n",err);
#endif // TODO #3

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// 태스크 1
//
void Task1(void *pdata)
{
	INT8U err;
	struct tm time_s;	// USER defined time structure

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

    printf("\n\nTask1 TASK created,\n");

	/* TODO #1: 
		Wait 5 Second from now on
		use 'OSTimeDly()' */
	printf("Wait 5 Second From now on\n");
#if 1

#endif // TODO #1
	printf("finished\n\n");

	/* TODO #2: 
		Wait 10 Second from now on
		use 'OSTimeDlyCustom()' : 5 line */
	printf("Wait 10 Second From now on\n");
#if 0
	time_s.tm_msec=?;
	time_s.tm_sec=?;
	time_s.tm_min=?;
	time_s.tm_hour=?;
	OSTimeDlyCustom(&time_s);
#endif // TODO #2
	printf("finished\n");

#if 0 // No comment
	err = OSTaskSuspend(OS_PRIO_SELF);
    	if (err) printf("OSTaskSuspend error found, code[0x%X]\n",err);
#endif // TODO #3

	// 1초마다 콘솔 출력 '중지'
	USE_OSTimeTickHook = 0; // refer to 'OS_CPU_C.C'

	printf("\n\ntask1 terminated,\n");
	err = OSTaskDel(OS_PRIO_SELF);	/* delete self task */
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// OSTimeDlyCustom
//
void OSTimeDlyCustom( struct tm *t )
{
	OSTimeDlyHMSM (t->tm_hour, t->tm_min, t->tm_sec, t->tm_msec);
}

/*-----------------------------------------------------------------------------
 * Program : 02_TIMER.C
-----------------------------------------------------------------------------*/
