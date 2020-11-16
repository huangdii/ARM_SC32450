// Exception.c
#include <stdlib.h>

void __attribute__((noinline)) serial_printf(char *fmt,...);

void Swi_Handler(void) 
{
	serial_printf("SWI exception.\n");
	for(;;);
}

void Undef_Handler(void) 
{
	serial_printf("Undefined instruction exception.\n");
	for(;;);
}

void Pabort_Handler(void)
{
	volatile int prefetch_addr=0;
	
	 __asm volatile(
		"mov %[retval],lr\n"
		"sub %[retval],%[retval],#4\n"
		: [retval] "=r" (prefetch_addr) : :
	);	
	serial_printf("Pabort exception in address 0x%08x.\n", prefetch_addr);		
	for(;;);
}

void Dabort_Handler(void)
{
	volatile int data_addr=0;

	 __asm volatile(
		"mov %[retval1],lr\n"
		"sub %[retval1],%[retval1],#8\n"
		: [retval1] "=r" (data_addr) : :
	);	
	serial_printf("Dabort exception in address 0x%08x.\n", data_addr);				
	for(;;);
}
