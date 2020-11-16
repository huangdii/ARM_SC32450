/*******************************************************************************
**	File:
**
**		08_FLAG.c
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
#define ENGINE_OIL_PRES_OK 0x01 
#define ENGINE_OIL_TEMP_OK 0x02 
#define ENGINE_START 0x04 

OS_FLAG_GRP *group_id; 

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
	INT8U err;
	OS_FLAGS flag_value;
	OS_FLAG_GRP *pgrp; // event flag group
	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

	/* TODO #1: 
		create a event flag
	    use group_id and initial value= 0*/
#if 0

#endif // TODO #1

	// 뮤텍스 쿼리
    flag_value = OSFlagQuery(group_id, &err);
    if (err) printf("OSFlagQuery error found, code[0x%X]\n",err);

	strcpy(TaskUserData[TASK_1_ID].TaskName, "task1");

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
    if (err) printf("OSTaskCreateExt error found, code[0x%X]\n",err);

	for(;;) {
	/* TODO #2:
		get flag 'ENGINE_OIL_PRES_OK' OR 'ENGINE_OIL_TEMP_OK' from TASK2 */
#if 0

#endif // TODO #2

		printf("."); fflush(stdout);
	}

	// 이벤트 플래그 삭제
	pgrp = OSFlagDel(group_id, OS_DEL_ALWAYS, &err); 
	if (pgrp == (OS_FLAG_GRP *)0) { 
	/* The event flag group was deleted */ 
	} 

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
  	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

    printf("\n\nTask1 created,\n");

	for(;;) {
	/* TODO #2: 
		post flag 'ENGINE_OIL_PRES_OK' to TASK1 */
#if 0

#endif // TODO #2

		OSTimeDly(100);
	}
}

/*-----------------------------------------------------------------------------
 * Program : 08_FLAG.C
-----------------------------------------------------------------------------*/
