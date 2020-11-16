/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                         (c) Copyright 1992-2003, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
* File         : OS_CPU.H
*********************************************************************************************************
*/

#ifndef OS_CPU_H
#define OS_CPU_H

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  	BOOLEAN;
typedef unsigned char  	INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  	INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short 	INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short 	INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   	INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   	INT32S;                   /* Signed   32 bit quantity                           */
typedef float          	FP32;                     /* Single precision floating point                    */
typedef double         	FP64;                     /* Double precision floating point                    */

typedef INT32U   		OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef INT32U   		OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */

/* 
*********************************************************************************************************
*                                              ARM
*
* Method #1:  NOT IMPLEMENTED
*             Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.
*             
* Method #2:  NOT IMPLEMENTED
*             Disable/Enable interrupts by preserving the state of interrupts.  In other words, if 
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.
*             NOT IMPLEMENTED
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to 
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.  This is the prefered method to disable interrupts.
*********************************************************************************************************
*/

#define  OS_CRITICAL_METHOD    3

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  (cpu_sr = OSCPUSaveSR())  /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   (OSCPURestoreSR(cpu_sr))  /* Restore  interrupts                       */
#endif

/*
*********************************************************************************************************
*                                         ARM Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1                       /* Stack grows from HIGH to LOW memory on ARM    */

#define  OS_TASK_SW()         OSCtxSw()

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

OS_CPU_EXT  INT32U  OSIntCtxSwFlag;                   /* Used to flag a context switch                 */

/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/

#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
OS_CPU_SR  OSCPUSaveSR(void);
void       OSCPURestoreSR(OS_CPU_SR cpu_sr);
#endif

#if 1
void          OSStartHighRdy        (void);
void          OSIntCtxSw            (void);
void          OSCtxSw               (void);
#endif

#endif
