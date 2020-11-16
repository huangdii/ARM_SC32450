/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 2 spaces!
 *
 * Created by YUNG KI HONG on 2018-12-13
 * Copyright (c) 2015 guileschool.com All rights reserved.
 */

/* FreeRTOS.org includes. */
#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "partest.h"

void vLogicSniffMultiSetLED5( unsigned int channel, int xValue );
void vLogicSniffMultiSetLED6( unsigned int channel, int xValue );
void vLogicSniffSetLED( unsigned int channel, int xValue );
void Key_INT_Init(void);

/* task's priority */
#define TASK_MAIN_PRIO	20
#define TASK_1_PRIO			10
#define TASK_2_PRIO			 9
#define TASK_3_PRIO			 8
#define TASK_4_PRIO			 7

struct Param_types {	/* struct for parameter passing to task */
       char *msg;
       int  P1,P2;
} Param_Tbl; 

/* The task functions. */
void TaskMain( void *pvParameters );
void Task1( void *pvParameters );
void Task2( struct Param_types *Param );

TaskHandle_t xHandleMain, xHandle1, xHandle2;
int	task1timer, task2timer;

extern SemaphoreHandle_t sem1;
char counter;

#ifdef FREERTOS_MODULE_TEST
BaseType_t prvExampleTaskHook( void * pvParameter );
#endif

#define BUFSIZE 1048576
static int copyTimes = 0;
static unsigned char b1[BUFSIZE];
static unsigned char b2[BUFSIZE];

static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

int main( void )
{
	/* Setup the hardware for use with the Beagleboard. */
	prvSetupHardware();

	/* Create one of the two tasks. */
	xTaskCreate(	TaskMain,		/* Pointer to the function that implements the task. */
					"TaskMain",	/* Text name for the task.  This is to facilitate debugging only. */
					1024,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_MAIN_PRIO,	/* This task will run at this priority */
					&xHandleMain );		/* We are not using the task handle. */

	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();	
	
	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
	return 0;
}
/*-----------------------------------------------------------*/

void TaskMain( void *pvParameters )
{
	const char *pcTaskName = "TaskMain";
	struct Param_types *Param;

	pvParameters = pvParameters; // for compiler warning

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

#ifdef FREERTOS_MODULE_TEST
	vTaskSetApplicationTaskTag( NULL, prvExampleTaskHook );
#endif

	/* TODO #1 #2: 
		create a binary semaphore
	    use sem_id */
#if 0
 	sem1=xSemaphoreCreateBinary();
// 	sem1 = xSemaphoreCreateCounting( 10, 0 );
if (sem1 == NULL) printf("xSemaphoreCreateBinary error found\n");
#endif // TODO #1

	/* Create the other task in exactly the same way. */
	xTaskCreate(	(TaskFunction_t)Task1,		/* Pointer to the function that implements the task. */
					"Task1",	/* Text name for the task.  This is to facilitate debugging only. */
					1024,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_1_PRIO,	/* This task will run at this priority */
					&xHandle1 );		/* We are not using the task handle. */

	/* Create the other task in exactly the same way. */
	Param = &Param_Tbl;		/* get parameter tbl addr */
	Param->P1 = 111111;		/* set parameter */
	Param->P2 = 222222;
	xTaskCreate( (TaskFunction_t)Task2, "Task2", 1024, (void*)Param, TASK_2_PRIO, &xHandle2 );

	/* delete self task */
	/* Print out the name of this task. */
	printf( "%s is deleted\r\n", pcTaskName );
	vTaskDelete (xHandleMain);	// vTaskDelete (NULL);
}
/*-----------------------------------------------------------*/

void Task1( void *pvParameters )
{
	const char *pcTaskName = "Task1";

	pvParameters = pvParameters; // for compiler warning

#ifdef FREERTOS_MODULE_TEST
	vTaskSetApplicationTaskTag( NULL, prvExampleTaskHook );
#endif

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

	while(1) {
		if(xSemaphoreTake( sem1, portMAX_DELAY ) == pdTRUE){
//			vTaskDelay(SW_DEBOUNCE_TIME);
			counter++;
		}
		printf("S%d(%d) ", counter, uxSemaphoreGetCount(sem1));
		task1timer++;
	}
}
/*-----------------------------------------------------------*/


void Task2( struct Param_types *Param )
{
	const char *pcTaskName = "Task2";

#ifdef FREERTOS_MODULE_TEST
	vTaskSetApplicationTaskTag( NULL, prvExampleTaskHook );
#endif

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

	printf("\n-------  Task2 parameter passed from main --------\n");
	printf("task2 first parameter = %d \n",Param->P1);
	printf("task2 second parameter = %d \n",Param->P2);
	printf("--------------------------------------------------\n");

	while(1) {
#if 1
//vTaskDelay (pdMS_TO_TICKS (1));
//printf("b"); fflush(stdout);	// 문자 'a' 출력
#endif

		task2timer++;
	}
}

// 시간이 많이 소요되는 특징으로써 정의한 함수
void heavyCopyLoader(void)
{
	int i;

	for(i=0; i<45; i++)
		memcpy(b2,b1,BUFSIZE);

	copyTimes++; 
}

void vApplicationIdleHook (void)
{
#ifdef FREERTOS_MODULE_TEST
	vLogicSniffMultiSetLED5(LS_CHANNEL_ALL5, 0); // All Leds Clear
	vLogicSniffMultiSetLED6(LS_CHANNEL_ALL6, 0); // All Leds Clear
	vLogicSniffSetLED( LS_CHANNEL_0, 1 ); // LED 'ON'	
#endif
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Initialise LED outputs. */
	vParTestInitialise();

	Key_INT_Init();
}
