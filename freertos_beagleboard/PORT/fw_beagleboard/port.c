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

/* Standard includes. */
#include <stdlib.h>
#include "omap3.h"

/* U-BOOT 26,000,000 -> 13,000,000MHz */
/* source -> u-boot/arch/arm/mach-omap2/omap3/clock.c */
#define configCPU_CLOCK_HZ			( ( unsigned long ) 13000000 ) /* = 13.000MHz clk gen */
#define configTICK_RATE_HZ			( ( int ) 1000 )

/*
 * Setup the timer 0 to generate the tick interrupts at the required frequency.
 */
void prvSetupTimerInterrupt( void )
{
	unsigned long ulCompareMatch;
	extern void ( vIRQHandler )( void );
	extern void ( vPortYieldProcessor ) ( void );

	/* Reset Interrupt Controller */
	(*(REG32(MPU_INTC + INTCPS_SYSCONFIG))) = 0x00000002;
	(*(REG32(MPU_INTC + INTCPS_IDLE))) = 0x00000001;

	serial_printf("(*(REG32(MPU_INTC + INTCPS_MIR1))) is 0x%08x\n", (*(REG32(MPU_INTC + INTCPS_MIR1))));

	/* Enable Interrupt 37 which is used for GPTIMER 1 and interrupt 74 for UART3*/
	(*(REG32(MPU_INTC + INTCPS_MIR_CLEAR0))) = ~(*(REG32(MPU_INTC + INTCPS_MIR0)))|0x20000000; // KEYPAD(GPIO1-7)
	(*(REG32(MPU_INTC + INTCPS_MIR_CLEAR1))) = ~(*(REG32(MPU_INTC + INTCPS_MIR1)))|0x20; // GPTIMER1
	(*(REG32(MPU_INTC + INTCPS_MIR_CLEAR2))) = ~(*(REG32(MPU_INTC + INTCPS_MIR2)))|0xffff; // UART3
	*(REG32(SERIAL_BASE + IER_REG)) = (0x5<<0); //UART RX interrupt

	/* Calculate the match value required for our wanted tick rate */
	ulCompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;
	
	/* Setup interrupt handler */
	E_IRQ = ( long ) vIRQHandler;

	/* The timer must be in compare mode, and should be the value
	 * holded in ulCompareMatch
	 * bit 0=1 -> enable timer
	 * bit 1=1 -> autoreload
	 * bit 6=1 -> compare mode
	 * The source is 32Khz
	 * */
	(*(REG32(GPTI1 + GPTI_TIOCP_CFG))) = 0x2; // reset interface
	(*(REG32(GPTI1 + GPTI_TLDR))) = 0; // initialize the load register
	(*(REG32(GPTI1 + GPTI_TCRR))) = 0; // initialize counter
	(*(REG32(GPTI1 + GPTI_TMAR))) = ulCompareMatch; // load match value
	
	/* Clear pending matching interrupt (if any) */
	(*(REG32(GPTI1 + GPTI_TISR))) = 0x1;
	
	/* Enable matching interrupts */
	(*(REG32(GPTI1 + GPTI_TIER))) = 0x1;

	/* Timer Control Register
	 * bit 0 -> start
	 * bit 1 -> autoreload
	 * bit 6 -> compare enabled
	 */

	(*(REG32(GPTI1 + GPTI_TCLR))) = 0x43;
	
	/* Reset the timer */
	(*(REG32(GPTI1 + GPTI_TTGR))) = 0xFF;

/* PRCM Clock Setting */
	serial_printf("CM_FCLKEN_WKUP is 0x%08x\n", *(REG32(CM_FCLKEN_WKUP))); // CM_FCLKEN_WKUP
//	(*(REG32(CM_FCLKEN_WKUP))) &=~ (1<<0); // GPTIMER 1 functional clock is disabled
	serial_printf("CM_ICLKEN_WKUP is 0x%08x\n", *(REG32(CM_ICLKEN_WKUP))); // CM_ICLKEN_WKUP
	serial_printf("CM_CLKSEL_WKUP is 0x%08x\n", *(REG32(CM_CLKSEL_WKUP))); // CM_CLKSEL_WKUP
	serial_printf("CM_CLKSEL1_PLL is 0x%08x\n", *(REG32(CM_CLKSEL1_PLL))); // CM_CLKSEL1_PLL

}
