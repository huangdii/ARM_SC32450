/*******************************************************************************
**	File:
**
**		05_MEMORY.c
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
//OS_STK Task1Stk[TASK_STK_SIZE]; /* Task #1 stack */
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
char *block, *buffer, *buffer2;
OS_MEM *pVMem;
INT8U allocBuf[100][64];

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
	OS_MEM_DATA mem_data;	/* memory block control structure */

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

	/* TODO #1: 
		create a memory partition 
	    use pVMem and allocBuf[][] */
#if 1
pVMem= OSMemCreate(allocBuf, 100, 64, &err);
#endif // TODO #1

	/* TODO #2: 
		get a memory block
	    use block */
#if 1
block= OSMemGet(pVMem, &err);
#endif // TODO #2

	// 메모리 블럭에 데이타('A') 저장
    *block = 'A';
	printf("Before *block[0x%08x] = %c\n",(int)&block[0], block[0]);

    err = OSMemQuery(pVMem, &mem_data);
    if (err) printf("OSMemQuery error found, code[0x%X]\n",err);

	/* TODO #2: 
		release a memory block */
#if 1
OSMemPut(pVMem, block);
#endif // TODO #2

	printf("After *block[0x%08x] = %c\n",(int)&block[0], block[0]);
	
	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

/*-----------------------------------------------------------------------------
 * Program : 05_MEMORY.C
-----------------------------------------------------------------------------*/
