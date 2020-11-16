/*******************************************************************************
**	File:
**
**		01_TASKMAN.c
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
#include "ui.h"

// filename
char* filename = "01_TASKMAN";

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
struct Param_types {	/* struct for parameter passing to task */
       char *msg;
       int  P1,P2;
} Param_Tbl; 
int	task1timer,task2timer;
extern void seedrand(int seed);
extern int randomnumber(void);

/* ...........................................................................
 *
 * 태스크 함수 원형
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  Task1(void *pdata);
void  Task2(struct Param_types *Param);        
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
	struct Param_types *Param;

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	// Task Statistics Title Display
	TaskStatisticsTitleDisplay(filename);

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

	// TASK CREATE
	/* TODO #1: 
		Task1을 생성
		use 'OSTaskCreate' */
#if 1

#endif // TODO #1

	// Set TASK2 parameter
	Param = &Param_Tbl;		/* get parameter tbl addr */
	Param->P1 = 111111;		/* set parameter */
	Param->P2 = 222222;
	strcpy(TaskUserData[TASK_2_ID].TaskName, "task2");

	// TASK CREATE
	err = OSTaskCreateExt((void(*)(void *))Task2,
	                (void *)Param,
	                &Task2Stk[TASK_STK_SIZE - 1],
	                TASK_2_PRIO,
	                TASK_2_ID,
	                &Task2Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_2_ID],
	                0);
	                
   	if (err) Uart_Printf("OSTaskCreateExt error found, code[0x%X]\n",err);

	/* TODO #2: 
		Task1을 중지
		use 'OSTaskSuspend' */
#if 1

#endif // TODO #2

	/* TODO #4: 
		Task1의 우선 순위를 'TASK_3_PRIO' 으로 변경
		use 'OSTaskChangePrio' and 'OSTaskResume' */
#if 1

#endif // TODO #4

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) Uart_Printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// 태스크 1
//
void Task1(void *pdata)
{
	INT8U err;
	OS_TCB task_data;

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

    Uart_Printf("\n\nTask1 TASK created,\n");

	err = OSTaskQuery(OS_PRIO_SELF, &task_data);
    	if (err) Uart_Printf("OSTaskQuery error found, code[0x%X]\n",err);

	Uart_Printf("\n-------  Task1 information -------\n");
	Uart_Printf("task1 id = %d \n",task_data.OSTCBPrio);
	Uart_Printf("task1 priority = %d \n",task_data.OSTCBPrio);
	Uart_Printf("task1 status = %d \n",task_data.OSTCBStat);
	Uart_Printf("----------------------------------\n");

	while(1) {
	/* TODO #3: 
		코드를 실행 하여 보고
		OSTimeDly() 코드를 주석 처리한 후 그 결과를 설명한다 */
#if 0 // No comment
		OSTimeDly(100);
		Uart_Printf("a");	fflush(stdout);// 문자 'a' 출력
#endif // TODO #3

		task1timer++;
	}
}

//
// 태스크 2
//
void Task2(struct Param_types *Param)
{
    Uart_Printf("Task2 TASK created,\n");

	Uart_Printf("\n-------  Task2 parameter passed from main --------\n");
	Uart_Printf("task2 first parameter = %d \n",Param->P1);
	Uart_Printf("task2 second parameter = %d \n",Param->P2);
	Uart_Printf("--------------------------------------------------\n");

	while(1) {
	/* TODO #3: 
		코드를 실행 하여 보고
		OSTimeDly() 코드를 주석 처리한 후 그 결과를 설명한다 */
#if 0 // No comment
		OSTimeDly(100);
		Uart_Printf("b");	fflush(stdout);// 문자 'b' 출력
#endif // TODO #3

		task2timer++;
	}
}

/*-----------------------------------------------------------------------------
 * Program : 01_TASKMAN.C
-----------------------------------------------------------------------------*/
