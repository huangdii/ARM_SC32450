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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "FreeRTOSConfig_base.h"

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 */

/*----------------------------------------------------------*/
#define traceTASK_SWITCHED_IN() xTaskCallApplicationTaskHook( pxCurrentTCB, 0 )
#define traceTASK_SWITCHED_OUT() xTaskCallApplicationTaskHook( pxCurrentTCB, 0 )

#ifdef configUSE_TRACE_FACILITY
#undef configUSE_TRACE_FACILITY
#define configUSE_TRACE_FACILITY                 1
#endif

#ifdef configUSE_IDLE_HOOK
#undef configUSE_IDLE_HOOK
#define configUSE_IDLE_HOOK                      1
#endif

#ifdef configUSE_MUTEXES
#undef configUSE_MUTEXES
#define configUSE_MUTEXES                        1
#endif

#ifdef configUSE_RECURSIVE_MUTEXES
#undef configUSE_RECURSIVE_MUTEXES
#define configUSE_RECURSIVE_MUTEXES              1
#endif

#define configUSE_APPLICATION_TASK_TAG  1

#define FREERTOS_MODULE_TEST

  /* TODO #1 #3: 
    상수 선언 __MY_ENABLE_DEFERRED_INTERRUPT 을 활성화 한다 */

#define __MY_ENABLE_DEFERRED_INTERRUPT  0

#endif /* FREERTOS_CONFIG_H */
