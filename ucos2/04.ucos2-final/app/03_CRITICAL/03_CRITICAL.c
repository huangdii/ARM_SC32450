/*******************************************************************************
**	File:
**
**		03_CRITICAL.c
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


int tickets;		// MAX value( 1000 million )
int backupTickets;
int T1_Ticket=0, T2_Ticket= 0;

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
#define TICKET_1BILLION		1000000000
#define TICKET_1MILLION		1000000
#define TICKET_10000		10000
#define TICKET_1000			1000

extern void seedrand(int seed);
extern int randomnumber(void);
int buyTicket(void);

// 1초마다 콘솔에 동작중을 표시(.)
extern INT32U USE_OSTimeTickHook;

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

	//
	// set random seed
	//
	seedrand( 45 );
	
	//
	// Set Tickets
	//
	/* TODO #2: 
		초기 티켓수를 작은수(10,000 ~ 100,000정도) 입력 하여 테스트 한후 
	    그 결과를 설명한다 */
#if 1 // No comment
	tickets= backupTickets = 10 * TICKET_1MILLION; // MAX value( 1 million )
#endif // TODO #2

	// TASK CREATE
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

	// TASK CREATE
    strcpy(TaskUserData[TASK_2_ID].TaskName, "Task2");
    OSTaskCreateExt(Task2,
                    (void *)0,
                    &Task2Stk[TASK_STK_SIZE - 1],
                    TASK_2_PRIO,
                    TASK_2_ID,
                    &Task2Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_2_ID],
                    0);
	if (err) printf("OSTaskCreate error found, code[0x%X]\n",err);

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

    printf("\n\nTask1 TASK created,\n");

	while(1) {
		if(buyTicket() <= 0)
		{
			T1_Ticket++;
//			printf("[TASK1]COUNTER up to %d\n", T1_Ticket);
			/* delete other task */
			err = OSTaskDel(TASK_2_PRIO);
			break;
		}
		T1_Ticket++;
	/* TODO #1: 
		OSTimeDly() 시간 지연을 적당히 입력하여 티켓 카운트 수가 
		오류가 나도록 만든다 */
#if 0 // No comment
		OSTimeDly(?);	// 주의사항! 2 이상의 값을 입력 할 것
#endif // TODO #1
	}

	// 1초마다 콘솔 출력 '중지'
	USE_OSTimeTickHook = 0; // refer to 'OS_CPU_C.C'

	printf("\n[TASK1]Total Tickets = %d\n", T1_Ticket + T2_Ticket);

	if(backupTickets != T1_Ticket + T2_Ticket)
		printf("Not good. expectation(%d)\n", backupTickets);
	else
		printf("Good!. Matched\n");

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// 태스크 2
//
void Task2(void *pdata)
{
	INT8U err;
	INT32U timeo;
	int i;

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

    printf("Task2 TASK created,\n");

	printf("please wait");

	// 1초마다 콘솔에 동작중을 표시(.)
	USE_OSTimeTickHook = 1; // refer to 'OS_CPU_C.C'

	while(1) {
		for(i=0;i<100000;i++)
		{
			if(buyTicket()==0)
			{
				T2_Ticket++;
//				printf("[TASK2]COUNTER up to %d\n", T2_Ticket);
				/* delete other task */
				err = OSTaskDel(TASK_1_PRIO);
				goto exit;
			}
			T2_Ticket++;
		}
		timeo = randomnumber();
		OSTimeDly(timeo % 3);
	}

exit:
	// 1초마다 콘솔 출력 '중지'
	USE_OSTimeTickHook = 0; // refer to 'OS_CPU_C.C'

	printf("\n[TASK2]Total Tickets = %d\n", T1_Ticket + T2_Ticket);

	if(backupTickets != T1_Ticket + T2_Ticket)
		printf("Not good. expectation(%d)\n", backupTickets);
	else
		printf("Good!. Matched\n");

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// buyTicket
//
int buyTicket(void)
{
	OS_CPU_SR cpu_sr;

	/* TODO #3: 
		OS_ENTER_CRITICAL() 과  OS_EXIT_CRITICAL()을 이용하여 
		공유변수(tickets)를 보호한다 */
#if 0
	// CRITICAL SECTION(ENTER)

#endif // TODO #3

	tickets --;	// ticket count

#if 0
	// CRITICAL SECTION(EXIT)

#endif // TODO #3

	return(tickets);
}

/*-----------------------------------------------------------------------------
 * Program : 03_CRITICAL.C
-----------------------------------------------------------------------------*/
