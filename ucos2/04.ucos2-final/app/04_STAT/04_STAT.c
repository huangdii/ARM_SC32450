/*******************************************************************************
**	File:
**
**		04_STAT.c
**
**  Contents:
**		uC/OS-II programming
**
**	History:
**		Date        Name              Description
**		------------------------------------------------------------------------
**
*******************************************************************************/

#include "../../include/includes.h"
//#include "ui.h"

// filename
char* filename = "04_STAT";

#define          MSG_QUEUE_SIZE     20                /* Size of message queue used in example         */

/* ...........................................................................
 *
 * 태스크 스택 정의
 * ===================
 */
OS_STK           TaskStartStk[TASK_STK_SIZE];         /* Startup    task stack                         */
OS_STK           Task1Stk[TASK_STK_SIZE];             /* Task #1    task stack                         */
OS_STK           Task2Stk[TASK_STK_SIZE];             /* Task #2    task stack                         */
OS_STK           Task3Stk[TASK_STK_SIZE];             /* Task #3    task stack                         */
OS_STK           Task4Stk[TASK_STK_SIZE];             /* Task #4    task stack                         */
OS_STK           Task5Stk[TASK_STK_SIZE];             /* Task #5    task stack                         */

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
OS_EVENT        *MsgQueue;                            /* Message queue pointer                         */
void            *MsgQueueTbl[20];                     /* Storage for messages                          */
// 1초마다 콘솔에 동작중을 표시(.)
extern INT32U USE_OSTimeTickHook;

/* ...........................................................................
 *
 * 태스크 함수 원형
 * ===================
 */
void             TaskStart(void *data);               /* Function prototypes of tasks                  */
void             TaskStartCreateTasks(void);
void             TaskClk(void *data);
void             Task1(void *data);
void             Task2(void *data);
void             Task3(void *data);
void             Task4(void *data);
void             Task5(void *data);

//
// 어플리케이션 메인 루틴
//
int main (void)
{
	OSInit(); /* microC/OS-II 초기화 */

    PC_ElapsedInit(); /* Initialized elapsed time measurement     */

    strcpy(TaskUserData[TASK_START_ID].TaskName, "Start");
    OSTaskCreateExt(TaskStart,
                    (void *)0,
                    &TaskStartStk[TASK_STK_SIZE - 1],
                    TASK_START_PRIO,
                    TASK_START_ID,
                    &TaskStartStk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_START_ID],
                    0);
    OSStart();                                            /* Start multitasking                       */
}

//
// 태스크 'START'
//
void  TaskStart (void *data)
{
    INT8U err;

	data = data;                                           /* Prevent compiler warning                 */

	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	// Task Statistics Title Display
	TaskStatisticsTitleDisplay(filename);

    OSStatInit();

	// 1초마다 콘솔에 동작중을 표시(.)
	USE_OSTimeTickHook = 1; // refer to 'OS_CPU_C.C'

    MsgQueue = OSQCreate(&MsgQueueTbl[0], MSG_QUEUE_SIZE); /* Create a message queue                   */

    TaskStartCreateTasks();

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// 태스크 'TaskStartCreateTasks'
//
void  TaskStartCreateTasks (void)
{
    strcpy(TaskUserData[TASK_1_ID].TaskName, "MsgQ Tx");
    OSTaskCreateExt(Task1,
                    (void *)0,
                    &Task1Stk[TASK_STK_SIZE - 1],
                    TASK_1_PRIO,
                    TASK_1_ID,
                    &Task1Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_1_ID],
                    0);

    strcpy(TaskUserData[TASK_2_ID].TaskName, "MsgQ Rx#1");
    OSTaskCreateExt(Task2,
                    (void *)0,
                    &Task2Stk[TASK_STK_SIZE - 1],
                    TASK_2_PRIO,
                    TASK_2_ID,
                    &Task2Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_2_ID],
                    0);

    strcpy(TaskUserData[TASK_3_ID].TaskName, "MsgQ Rx#2");
    OSTaskCreateExt(Task3,
                    (void *)0,
                    &Task3Stk[TASK_STK_SIZE - 1],
                    TASK_3_PRIO,
                    TASK_3_ID,
                    &Task3Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_3_ID],
                    0);

    strcpy(TaskUserData[TASK_4_ID].TaskName, "MboxPostPend");
    OSTaskCreateExt(Task4,
                    (void *)0,
                    &Task4Stk[TASK_STK_SIZE - 1],
                    TASK_4_PRIO,
                    TASK_4_ID,
                    &Task4Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_4_ID],
                    0);

    strcpy(TaskUserData[TASK_5_ID].TaskName, "TimeDly");
    OSTaskCreateExt(Task5,
                    (void *)0,
                    &Task5Stk[TASK_STK_SIZE - 1],
                    TASK_5_PRIO,
                    TASK_5_ID,
                    &Task5Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_5_ID],
                    0);
}

//
// 태스크 'Task1'
//
void  Task1 (void *data)
{
    char one   = '1';
    char two   = '2';
    char three = '3';

    data = data;
    for (;;) {
        OSQPost(MsgQueue, (void *)&one);
        OSTimeDlyHMSM(0, 0, 1,   0);        /* Delay for 1 second                                      */
        OSQPost(MsgQueue, (void *)&two);
        OSTimeDlyHMSM(0, 0, 0, 500);        /* Delay for 500 mS                                        */
        OSQPost(MsgQueue, (void *)&three);
        OSTimeDlyHMSM(0, 0, 1,   0);        /* Delay for 1 second                                      */
    }
}

//
// 태스크 'Task2'
// Description: This task waits for messages sent by task #1.
//
void  Task2 (void *data)
{
    char *msg;
    INT8U  err;

    data = data;
    for (;;) {
        msg = (char *)OSQPend(MsgQueue, 0, &err);    /* Wait forever for message                      */
        PC_DispChar(14, 9, *msg, (INT8U)YELLOW);
        OSTimeDlyHMSM(0, 0, 0, 500);                  /* Delay for 500 mS                              */
    }
}

//
// 태스크 'Task3'
// Description: This task waits for up to 250 mS for a message sent by task #1.
//
void  Task3 (void *data)
{
    INT8U *msg;
    INT8U  err;

    data = data;
    for (;;) {
        msg = (INT8U *)OSQPend(MsgQueue, OS_TICKS_PER_SEC / 4, &err);  /* Wait up to 250 mS for a msg  */
        if (err == OS_TIMEOUT) {
            PC_DispChar(14, 9, 'T',  BLUE);
        } else {
            PC_DispChar(14, 9, *msg, BLUE);
        }
    }
}

//
// 태스크 'Task4'
// Description: This task posts a message to a mailbox and then immediately reads the message.
//
void  Task4 (void *data)
{
    OS_EVENT *mbox;
    INT8U     err;

    data = data;
    mbox = OSMboxCreate((void *)0);
    for (;;) {
        OSMboxPost(mbox, (void *)1);             /* Send message to mailbox                            */
        OSMboxPend(mbox, 0, &err);               /* Get message from mailbox                           */
        OSTimeDlyHMSM(0, 0, 0, 10);              /* Delay 10 mS                                        */
    }
}

//
// 태스크 'Task5'
// Description: This task simply delays itself.  We basically want to determine how long OSTimeDly() takes
//              to execute
//
void  Task5 (void *data)
{
    data = data;
    for (;;) {
        OSTimeDly(1);
    }
}

/*-----------------------------------------------------------------------------
 * Program : 04_STAT.C
-----------------------------------------------------------------------------*/
