/*
The MIT License (MIT)

Copyright (c) 2015 guileschool

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "omap3.h"

#define USER_BUTTON	(1<<7)

int Key_Get_Pressed(void);

static int buttoncounter;

// Semaphore 선언
SemaphoreHandle_t sem1;

void vKey_ISR_Handler( void );

int getButtoncounter(void)
{
	return buttoncounter;
}

void vKey_ISR_Handler( void )
{
	static portBASE_TYPE xHigherPriorityTaskWoken;
	portBASE_TYPE xSemaStatus;

	/* Clear GPIO interrupt */
	(*(REG32(GPIO1_BASE+GPIO_IRQSTATUS1)))= PIN7;
//	(*(REG32(GPIO1_BASE+GPIO_IRQSTATUS2)))= PIN7;
//	(*(REG32(GPIO1_BASE+GPIO_CLEARWKUENA)))= PIN7;

	*(REG32(MPU_INTC + INTCPS_CONTROL)) = 0x1;

// Data Synchronization Barrier
	__asm volatile("MOV R0, #0 \n\t MCR P15, #0, R0, C7, C10, #4 \n\t");

//	serial_putchar('o');
	buttoncounter++;

/* counting semaphore example12 */
	xHigherPriorityTaskWoken = pdFALSE;

	// Semaphore를 'Give' 하여 Blocked Task를 Unblock 상태가 되게 한다.
	xSemaStatus = xSemaphoreGiveFromISR( sem1, &xHigherPriorityTaskWoken );
#if 1
	if( (xSemaStatus == pdTRUE) && (xHigherPriorityTaskWoken == pdTRUE) )
	{
		printf("xHigherPriorityTaskWoken\n");
		portYIELD_FROM_ISR();
	}
#endif
}

void Key_Port_Init(void)
{
	(*(REG32(GPIO1_BASE+GPIO_OE))) |= (PIN7); // for USER Button

	printf("GPIO_DATAIN is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_DATAIN))));
	printf("GPIO_OE is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_OE))));
}

void Key_INT_Init(void)
{
	(*(REG32(GPIO1_BASE+GPIO_SETIRQENABLE1))) = PIN7;
	(*(REG32(GPIO1_BASE+GPIO_FALLINGDETECT))) = PIN7;
	(*(REG32(GPIO1_BASE+GPIO_DEBOUNCEINGTIME))) = 250;
	(*(REG32(GPIO1_BASE+GPIO_DEBOUNCENABLE))) = PIN7;

	printf("GPIO_DATAIN is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_DATAIN))));
	printf("GPIO_OE is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_OE))));
	printf("GPIO_IRQENABLE1 is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_IRQENABLE1))));
}

int Key_Get_Pressed(void)
{
	return ( (*(REG32(GPIO1_BASE+GPIO_DATAIN))) & USER_BUTTON );
}

int Key_Wait_Get_Pressed(void)
{
	/* YOUR CODE HERE */
	int a; 
	while (! (a= Key_Get_Pressed()));
	return a; 

}

void Key_Wait_Get_Released(void)
{
	/* YOUR CODE HERE */
	while (Key_Get_Pressed());
	printf("released\n");
}

/* Key Initialization */
void KEY1_INIT(void)
{
}

/* Initialize user key(button) */
void KEYPAD_Init(void)
{

}

int KEYPAD_Read(void)
{
	return ( (*(REG32(GPIO1_BASE+GPIO_DATAIN))) & USER_BUTTON );
}
