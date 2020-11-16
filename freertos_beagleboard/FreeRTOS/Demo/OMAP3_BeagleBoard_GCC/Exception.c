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

// Exception.c
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

void __attribute__((noinline)) serial_printf(char *fmt,...);

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
