/*=========================*/
#include "my_lib.h"
#include "option.h"
#include "2450addr.h"
#include "./images/bicycle.h"

void Main(void)
{
	Led_Port_Init(); //LED init
	Uart_Init(115200); // baudrate=115,200kbps

	Led_Display(1); //LED is all 'on'
	Uart_Printf("Hello, UCOS~~\n");

	printf("Hello, UCOS~~ by printf\n");
	while(1); // endless loop

	return;
}
