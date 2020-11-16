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

/* Standard includes. */
#include <stdlib.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "partest.h"

void vFlashCoRoutine (CoRoutineHandle_t xHandle,
                 UBaseType_t uxIndex);

#define PRIORITY_0 0

static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

int main( void )
{
	/* Setup the hardware for use with the Beagleboard. */
	prvSetupHardware();

vParTestSetLED( 0, 0 ); // USR1 'OFF'
vParTestSetLED( 1, 0 ); // USR0 'OFF'

	serial_putstring("vFlashCoRoutine\n");
	//vStartLEDFlashTasks( mainLED_TASK_PRIORITY );
	xCoRoutineCreate (vFlashCoRoutine, PRIORITY_0, 0);

  /* TODO #2: 
    두번째 코루틴 함수 생성 */
#if 0

#endif // TODO #2

vParTestSetLED( 0, 1 ); // USR1 'ON'
vParTestSetLED( 1, 1 ); // USR0 'ON'

	vTaskStartScheduler();

	/* Should never reach here! */
	return 0;
}


void vApplicationIdleHook (void)
{
  /* TODO #1: 
    코루틴 스케줄러 실행 
    vCoRoutineSchedule */
#if 0
   vCoRoutineSchedule ();
#endif // TODO #1
}


void vFlashCoRoutine (CoRoutineHandle_t xHandle,
                 UBaseType_t uxIndex)
{
   // 공동 루틴은 crSTART () 호출로 시작해야합니다.
   crSTART (xHandle);

   for (;;)
   {
      // 고정 된 기간 동안 지연.
      crDELAY (xHandle, 100);

      // LED를 플래시합니다.
      vParTestToggleLED (uxIndex);
   }

   // 공동 루틴은 crEND ()에 대한 호출로 끝나야합니다.
   crEND ();
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Initialise LED outputs. */
	vParTestInitialise();

}
