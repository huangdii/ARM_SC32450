#include "omap3.h"

#define USER_BUTTON	(1<<7)

int Key_Get_Pressed(void);

static int buttoncounter;

void vKey_ISR_Handler( void );

int getButtoncounter(void)
{
	return buttoncounter;
}

void vKey_ISR_Handler( void )
{
	/* Clear GPIO interrupt */
	(*(REG32(GPIO1_BASE+GPIO_IRQSTATUS1)))= PIN7;
//	(*(REG32(GPIO1_BASE+GPIO_IRQSTATUS2)))= PIN7;
//	(*(REG32(GPIO1_BASE+GPIO_CLEARWKUENA)))= PIN7;

	*(REG32(MPU_INTC + INTCPS_CONTROL)) = 0x1;

// Data Synchronization Barrier
	__asm volatile("MOV R0, #0 \n\t MCR P15, #0, R0, C7, C10, #4 \n\t");

//	serial_putchar('o');
	buttoncounter++;
}

void Key_Port_Init(void)
{
	(*(REG32(GPIO1_BASE+GPIO_OE))) |= (PIN7); // for USER Button

	serial_printf("GPIO_DATAIN is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_DATAIN))));
	serial_printf("GPIO_OE is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_OE))));
}

void Key_INT_Init(void)
{
	(*(REG32(GPIO1_BASE+GPIO_SETIRQENABLE1))) = PIN7;
	(*(REG32(GPIO1_BASE+GPIO_FALLINGDETECT))) = PIN7;
	(*(REG32(GPIO1_BASE+GPIO_DEBOUNCEINGTIME))) = 250;
	(*(REG32(GPIO1_BASE+GPIO_DEBOUNCENABLE))) = PIN7;

	serial_printf("GPIO_DATAIN is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_DATAIN))));
	serial_printf("GPIO_OE is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_OE))));
	serial_printf("GPIO_IRQENABLE1 is 0x%08x\n", (*(REG32(GPIO1_BASE+GPIO_IRQENABLE1))));
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
	serial_printf("released\n");
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
