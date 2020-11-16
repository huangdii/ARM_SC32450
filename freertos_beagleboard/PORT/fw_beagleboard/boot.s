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

	/* Sample initialization file */

	.extern main
	.extern exit
  .extern vParTestInitialise

	.text
	.code 32


	.align  0

  .extern rodata_end_addr
  .extern data_start_addr
  .extern data_end_addr
  .extern bss_end_addr
  .extern bss_start_addr

	.global start
	.global endless_loop

	/* Stack Sizes */
    .set  UND_STACK_SIZE, 0x00004000
    .set  ABT_STACK_SIZE, 0x00004000
    .set  FIQ_STACK_SIZE, 0x00004000
    .set  IRQ_STACK_SIZE, 0X00004000
    .set  SVC_STACK_SIZE, 0x00004000

	/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs */
    .set  MODE_USR, 0x10            /* User Mode */
    .set  MODE_FIQ, 0x11            /* FIQ Mode */
    .set  MODE_IRQ, 0x12            /* IRQ Mode */
    .set  MODE_SVC, 0x13            /* Supervisor Mode */
    .set  MODE_ABT, 0x17            /* Abort Mode */
    .set  MODE_UND, 0x1B            /* Undefined Mode */
    .set  MODE_SYS, 0x1F            /* System Mode */

    .equ  I_BIT, 0x80               /* when I bit is set, IRQ is disabled */
    .equ  F_BIT, 0x40               /* when F bit is set, FIQ is disabled */


start:
_start:
_mainCRTStartup:

	/* Set vector address in CP15 VBAR register */
	ldr	r0, =_EVT
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR

	/* Setup a stack for each mode - note that this only sets up a usable stack
	for system/user, SWI and IRQ modes.   Also each mode is setup with
	interrupts initially disabled. */
    ldr   r0, .LC6
    msr   CPSR_c, #MODE_UND|I_BIT|F_BIT /* Undefined Instruction Mode */
    mov   sp, r0
    sub   r0, r0, #UND_STACK_SIZE
    msr   CPSR_c, #MODE_ABT|I_BIT|F_BIT /* Abort Mode */
    mov   sp, r0
    sub   r0, r0, #ABT_STACK_SIZE
    msr   CPSR_c, #MODE_FIQ|I_BIT|F_BIT /* FIQ Mode */
    mov   sp, r0
    sub   r0, r0, #FIQ_STACK_SIZE
    msr   CPSR_c, #MODE_IRQ|I_BIT|F_BIT /* IRQ Mode */
    mov   sp, r0
    sub   r0, r0, #IRQ_STACK_SIZE
    msr   CPSR_c, #MODE_SVC|I_BIT|F_BIT /* Supervisor Mode */
    mov   sp, r0
    sub   r0, r0, #SVC_STACK_SIZE
    msr   CPSR_c, #MODE_SYS|I_BIT|F_BIT /* System Mode */
    mov   sp, r0
    
	/* We want to start in supervisor mode.  Operation will switch to system
	mode when the first task starts. */
		msr   CPSR_c, #MODE_SVC

    /* Copy and paste RW data zero initialized data */
    ldr r0, =rodata_end_addr    /* text_end_addr Get pointer to ROM data */
    ldr r1, =data_start_addr    /* bss_end_addr and RAM copy */
    ldr r3, =data_end_addr      /* bss_start_addr  */
    mov r2, r0, lsr #12
    mov r4, r1, lsr #12

    /* Zero init base => top of initialised data */
    cmp r2, r4              /* Check that they are different */
    beq 2f
1:       
    cmp r1, r3              /* Copy init data */
    ldrcc   r2, [r0], #4    /* --> LDRCC r2, [r0] + ADD r0, r0, #4 */         
    strcc   r2, [r1], #4    /* --> STRCC r2, [r1] + ADD r1, r1, #4 */
    bcc 1b
2:       
    mov r2, #0 
    ldr r1,=bss_end_addr 
    ldr r3,=bss_start_addr
3:       
    cmp r3, r1      /* Zero init */
    strcc   r2, [r3], #4
    bcc 3b

		mov		r0, #0          /* no arguments  */
		mov		r1, #0          /* no argv either */

		bl		main

endless_loop:
	b               endless_loop

	.align 0

	.LC6:
	.word   __stack_end__


	/* Setup vector table.  Note that undf, pabt, dabt, fiq just execute
	a null loop. */

.section .startup,"ax"
         .code 32
         .align 0
_EVT:
	b     _start						/* reset - _start			*/
	ldr   pc, =_undf					/* undefined - _undf		*/
	ldr   pc, =_swi						/* SWI - _swi				*/
	ldr   pc, =_pabt					/* program abort - _pabt	*/
	ldr   pc, =_dabt					/* data abort - _dabt		*/
	nop									/* reserved					*/
	ldr   pc, =_irq						/* IRQ - read the VIC		*/
	ldr   pc, =_fiq						/* FIQ - _fiq				*/

.if 0
_undf:  .word 0x4020FFE4                    /* undefined				*/
_swi:   .word 0x4020FFE8	        		/* SWI						*/
_pabt:  .word 0x4020FFEC                    /* program abort			*/
_dabt:  .word 0x4020FFF0                    /* data abort				*/
_fiq:   .word 0x4020FFFc                    /* FIQ						*/

__undf: b     .                         /* undefined				*/
__pabt: b     .                         /* program abort			*/
__dabt: b     .                         /* data abort				*/
__fiq:  b     .                         /* FIQ						*/
.else 
	.extern Undef_Handler
	.extern Swi_Handler
	.extern Pabort_Handler
	.extern Dabort_Handler
	.extern vIRQHandler
_undf:  ldr pc, =Undef_Handler              /* undefined				*/
_swi:   ldr pc, =Swi_Handler				/* SWI						*/
_pabt:  ldr pc, =Pabort_Handler			    /* program abort			*/
_dabt:  ldr pc, =Dabort_Handler			    /* data abort				*/
_used:  b .			        				/* Unused					*/
_irq:   ldr pc, =vIRQHandler                /* IRQ						*/
_fiq:   .word 0x4020FFFc                    /* FIQ						*/

__undf: ldr pc, =Undef_Handler              /* undefined				*/
__pabt: ldr pc, =Pabort_Handler             /* program abort			*/
__dabt: ldr pc, =Dabort_Handler	            /* data abort				*/
__fiq:  b     .                         	/* FIQ						*/
.endif
