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

#include "omap3.h"

/*
 * Configure the processor for use with the Olimex demo board.  This includes
 * setup for the I/O, system clock, and access timings.
 */
static void prvSetupHardware( void );
void prvSetupTimerInterrupt( void );

void vParTestSetLED( unsigned int uxLED, int xValue );
void vParTestToggleLED( unsigned int uxLED );
int getTick(void);

/*-----------------------------------------------------------*/

/*
 * Starts all the other tasks, then starts the scheduler.
 */
int main( void )
{
	int i,j;

	/* Setup the hardware for use with the Beagleboard. */
	prvSetupHardware();

vParTestSetLED( 0, 0 ); // USR1 'OFF'
vParTestSetLED( 1, 0 ); // USR0 'OFF'

	serial_printf("Beagleboard FW start!\n");

vParTestSetLED( 0, 1 ); // USR1 'ON'
vParTestSetLED( 1, 1 ); // USR0 'ON'

	while(1)
	{
		vParTestToggleLED( 0 );
		vParTestToggleLED( 1 );
		for(i=0; i<1000000; i++)
			for(j=0; j<10; j++);
		serial_printf("\rtick %d\r", getTick());
	}

	/* Should never reach here! */
	return 0;
}

/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Initialise LED outputs. */
	prvSetupTimerInterrupt();

	/* Initialize GPIOs */
	/* GPIO5: 31,30,29,28,22,21,15,14,13,12
	 * GPIO6: 23,10,08,02,01 */
	(*(REG32(GPIO5_BASE+GPIO_OE))) = ~(PIN31|PIN30|PIN29|PIN28|PIN22|PIN21|PIN15|PIN14|PIN13|PIN12);
	// 7seg
	(*(REG32(GPIO5_BASE+GPIO_OE))) &= ~(PIN7|PIN6|PIN5|PIN4|PIN3|PIN2);

	(*(REG32(GPIO6_BASE+GPIO_OE))) = ~(PIN23|PIN10|PIN8|PIN2|PIN1);

	/* Switch off the leds */
	(*(REG32(GPIO5_BASE+GPIO_CLEARDATAOUT))) = (PIN22|PIN21);
	// 7 seg
	(*(REG32(GPIO5_BASE+GPIO_CLEARDATAOUT))) = (PIN28|PIN30|PIN4|PIN2);
	
	(*(REG32(GPIO5_BASE+GPIO_SETDATAOUT))) = (PIN31|PIN29|PIN7|PIN6|PIN5|PIN3);
	(*(REG32(GPIO6_BASE+GPIO_SETDATAOUT))) = (PIN1|PIN2);

}

