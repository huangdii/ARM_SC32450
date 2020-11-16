/*******************************************************************************
**	File:
**
**		Main.c
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
#include "COMMRTOS.H"

/* ...........................................................................
 *
 * 태스크 스택 정의
 * ===================
 */
OS_STK TaskStartStk[TASK_STK_SIZE]; /* START task stack */
OS_STK Task1Stk[TASK_STK_SIZE]; /* Task #1 stack */
OS_STK Task2Stk[TASK_STK_SIZE]; /* Task #2 stack */
OS_STK Task3Stk[TASK_STK_SIZE]; /* Task #3 stack */
OS_STK Task4Stk[TASK_STK_SIZE]; /* Task #4 stack */

/* ...........................................................................
 *
 * 사용자 메모리 정의
 * ===================
 */
TASK_USER_DATA  TaskUserData[7];

/* ...........................................................................
 *
 * 이벤트 컨트롤 블럭 정의
 * ===================
 */
// 세마포어
OS_EVENT *TOUCH_SEM;
OS_EVENT *INPUT_SEM;
OS_EVENT *LCD_SEM;

// 메시지 큐 'LCD'
OS_EVENT *qLCD_id;	/* Message Queue Pointer */
void *qLCDBuffer[10];


/* ...........................................................................
 *
 * 태스크 함수 원형
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  TaskInput(void *pdata);
void  TaskLCDdisp(void *pdata);          
void  TaskCalcOp(void *pdata);
void  Task4(void *pdata);


//
// 어플리케이션 메인 루틴
//
///////////////////////////////////////////////////////////////////////
// 주의 사항
// 컴파일시 반드시 매크로 정의 할것 !
//
// C 컴파일 설정 매크로: -D__BUFFERED_IO -D_MINI_CALC_
//          해제 매크로: -D__XDEF_REPORT_STAT( 없어야 함 )
//
// ASM 컴파일 매크로: -PD "TOUCH  SETL {TRUE}"
//
///////////////////////////////////////////////////////////////////////
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
	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

// Debugging
     Led_Display(1);

	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

// Debugging
     Led_Display(15); //while(1);

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

// 세마포어 생성
	TOUCH_SEM 	= OSSemCreate(0); // TOUCH 세마포어
	INPUT_SEM 	= OSSemCreate(0); // 'TaskInput' 세마포어
	LCD_SEM 	= OSSemCreate(0); // 'TaskLCDdisp' 세마포어

	// 메시지 큐 생성
	qLCD_id = OSQCreate(&qLCDBuffer[0], 10);
    if (!qLCD_id) Uart_Printf("qcreate error found, code[%d]\n",(int)qLCD_id);

// 태스크 TaskInput
	strcpy(TaskUserData[TASK_2_ID].TaskName, "TaskInput");
#if 1
	// TASK CREATE
	OSTaskCreateExt(TaskInput,
	                (void *)0,
	                &Task2Stk[TASK_STK_SIZE - 1],
	                TASK_2_PRIO,
	                TASK_2_ID,
	                &Task2Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_2_ID],
	                0);

// 태스크 TaskLCDdisp
	strcpy(TaskUserData[TASK_3_ID].TaskName, "TaskLCDdisp");
#endif

	// TASK CREATE
	OSTaskCreateExt(TaskLCDdisp,
	                (void *)0,
	                &Task3Stk[TASK_STK_SIZE - 1],
	                TASK_3_PRIO,
	                TASK_3_ID,
	                &Task3Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_3_ID],
	                0);

// 태스크 TaskCalcOp
 	INPUT_SEM 	= OSSemCreate(0); // 'TaskCalcOp' 세마포어 생성
	strcpy(TaskUserData[TASK_3_ID].TaskName, "TaskCalcOp");

	// TASK CREATE
	OSTaskCreateExt(TaskCalcOp,
	                (void *)0,
	                &Task4Stk[TASK_STK_SIZE - 1],
	                TASK_4_PRIO,
	                TASK_4_ID,
	                &Task4Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_4_ID],
	                0);

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) Uart_Printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}


/*-----------------------------------------------------------------------------
 * Program : Main.C
-----------------------------------------------------------------------------*/
