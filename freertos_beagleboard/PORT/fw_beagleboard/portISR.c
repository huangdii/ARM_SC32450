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

#include "omap3.h"

void vIRQHandler( void ) __attribute__((interrupt("IRQ")));
void vTickISR ( void );
void __attribute__((weak)) vKey_ISR_Handler( void );
void vUART_ISR_Handler( void );

static int tick;

/* Read the incoming interrupt and then jump to the appropriate ISR */
void vIRQHandler ( void )
{
	/* If this is IRQ_38 then jump to vTickISR */
	if((*(REG32(MPU_INTC + INTCPS_SIR_IRQ))) == 37)
	{
		vTickISR();
	}
	else if((*(REG32(MPU_INTC + INTCPS_SIR_IRQ)))==74)
	{
		vUART_ISR_Handler();
	}
	else if((*(REG32(MPU_INTC + INTCPS_SIR_IRQ)))==29) // GPIO1_MPU_IRQ(29) 모듈
	{
		vKey_ISR_Handler();
	}
	else
	{
		serial_printf("spurious interrupt(no.%d\n", (*(REG32(MPU_INTC + INTCPS_SIR_IRQ))));
	}
}


int getTick(void)
{
	return tick;
}

/* 
 * The ISR used for the scheduler tick.
 */
void vTickISR( void )
{
	/* Increment the RTOS tick count, then look for the highest priority 
	task that is ready to run. */

	/* tick counter ++ */
	tick++;

	/* Ready for the next interrupt.
	 * Reset the timer. Clear the interrupts
	 * Writing any value to TTGR register makes GPTimer1
	 * reloads to the value stored in TLDR
	 * TRM: 2599
	 * Clear the interrupts
	 * Page: 1060 
	 */
	/* Make sure mask is correct after handling the IRQ */

	(*(REG32(GPTI1 + GPTI_TISR))) = 0x1;  // clear Match interrupt
	(*(REG32(GPTI1 + GPTI_TTGR))) = 0xFF; // reset timer 
 	(*(REG32(MPU_INTC + INTCPS_CONTROL))) = 0x1;

// Data Synchronization Barrier
	__asm volatile("MOV R0, #0 \n\t MCR P15, #0, R0, C7, C10, #4 \n\t");
}

void start_timer(void)
{
	/* Enable matching interrupts */
	(*(REG32(GPTI1 + GPTI_TIER))) = 0x1;
}

void stop_timer(void)
{
	/* Enable matching interrupts */
	(*(REG32(GPTI1 + GPTI_TIER))) = 0x0;
}