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

/*
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 *
 * Main.c also creates a task called "Check".  This only executes every three
 * seconds but has the highest priority so is guaranteed to get processor time.
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is
 * incremented each time the task successfully completes its function.  Should
 * any error occur within such a task the count is permanently halted.  The
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 * To check the operation of the memory allocator the check task also
 * dynamically creates a task before delaying, and deletes it again when it
 * wakes.  If memory cannot be allocated for the new task the call to xTaskCreate
 * will fail and an error is signalled.  The dynamically created task itself
 * allocates and frees memory just to give the allocator a bit more exercise.
 *
 */

/* Scheduler includes. */
#include <stdio.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "partest.h"

/* The periods assigned to the one-shot and auto-reload timers are 3.333 second and half a second respectively. */
#define mainONE_SHOT_TIMER_PERIOD		pdMS_TO_TICKS( 100 )
#define mainAUTO_RELOAD_TIMER_PERIOD	pdMS_TO_TICKS( 50 )

static void prvOneShotTimerCallback( TimerHandle_t xTimer );
static void prvAutoReloadTimerCallback( TimerHandle_t xTimer );

static int ulCallCount;

static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

/*
 * Starts all the other tasks, then starts the scheduler.
 */
int main( void )
{
	TimerHandle_t xAutoReloadTimer, xOneShotTimer;
	BaseType_t xTimer1Started, xTimer2Started;

	/* Setup the hardware for use with the Beagleboard. */
	prvSetupHardware();

	/* TODO #1: 
		원샷 소프트웨어 타이머(xOneShotTimer)의 구현 */
#if 0
	/* Create the one shot timer, storing the handle to the created timer in xOneShotTimer. */
	xOneShotTimer = xTimerCreate(
					/* Text name for the software timer - not used by FreeRTOS. */
					"OneShot",
					/* The software timer's period in ticks. */
					mainONE_SHOT_TIMER_PERIOD,
					/* Setting uxAutoRealod to pdFALSE creates a one-shot software timer. */
					pdFALSE,
					/* This example does not use the timer id. */
					0,
					/* The callback function to be used by the software timer being created. */
					prvOneShotTimerCallback );
#endif // TODO #1

	/* TODO #2: 
		자동 반복 소프트웨어 타이머(xAutoReloadTimer)의 구현 */
#if 0

#endif // TODO #2

	/* Check the software timers were created. */
	if( xOneShotTimer != NULL )
	{
	/* Start the software timers, using a block time of 0 (no block time).
		The scheduler has not been started yet so any block time specified here would be ignored anyway. */
		xTimer1Started = xTimerStart( xOneShotTimer, 0 );

	/* The implementation of xTimerStart() uses the timer command queue, and xTimerStart()
		will fail if the timer command queue gets full.
		The timer service task does not get created until the scheduler is started, so all commands sent to the command queue will
		stay in the queue until after the scheduler has been started.
		Check both calls to xTimerStart() passed. */

		if( xTimer1Started == pdPASS )
		{
			printf("xTimer1Started GOOD\n");
			/* Start the scheduler. */
		}
		printf("xTimer1Started BAD\n");
	}

	/* Check the software timers were created. */
	if( xAutoReloadTimer != NULL )
	{
	/* Start the software timers, using a block time of 0 (no block time).
		The scheduler has not been started yet so any block time specified here would be ignored anyway. */
		xTimer2Started = xTimerStart( xAutoReloadTimer, 0 );

	/* The implementation of xTimerStart() uses the timer command queue, and xTimerStart()
		will fail if the timer command queue gets full.
		The timer service task does not get created until the scheduler is started, so all commands sent to the command queue will
		stay in the queue until after the scheduler has been started.
		Check both calls to xTimerStart() passed. */

		if( xTimer2Started == pdPASS )
		{
			printf("xTimer2Started GOOD\n");
			/* Start the scheduler. */
		}
		printf("xTimer2Started BAD\n");
	}

	vTaskStartScheduler();
	/* As always, this line should not be reached. */
	for( ;; );
}

static void prvOneShotTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow;

	xTimer = xTimer; // for compiler warning

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	printf( "One-shot timer callback executing %d\n", (int)xTimeNow );

	/* File scope variable. */
	ulCallCount++;
}

static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow;

	xTimer = xTimer; // for compiler warning

	/* Obtain the current tick count. */

	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */

	printf( "Auto-reload timer callback executing %d\n", (int)xTimeNow );

	ulCallCount++;
}

void vApplicationIdleHook (void)
{
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Initialise LED outputs. */
	vParTestInitialise();

}
