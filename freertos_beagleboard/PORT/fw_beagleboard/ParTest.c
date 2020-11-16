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

#define partstNUM_LEDS			( 1 )

/*-----------------------------------------------------------*/

void vParTestInitialise( void )
{
	return;
}

/*-----------------------------------------------------------*/

void vParTestSetLED( unsigned int uxLED, int xValue )
{
	unsigned int GPIO_PIN = 0;
	if( uxLED <= partstNUM_LEDS){
		/* I define LED0 (GPIO_149) as 0
		 * and 	LED1 (GPIO_150) as 1 */
		switch(uxLED){
			case 0: 
				{
					GPIO_PIN=PIN21;
					break;
				}
			case 1: 
				{
					GPIO_PIN=PIN22;
					break;
				}
			default: break;
		};
		if ( xValue )
			(*(REG32(GPIO5_BASE + GPIO_SETDATAOUT))) = GPIO_PIN;
		else
			(*(REG32(GPIO5_BASE + GPIO_CLEARDATAOUT))) = GPIO_PIN;
	}
}

/*-----------------------------------------------------------*/

void vParTestToggleLED( unsigned int uxLED )
{
	volatile unsigned long ulCurrentState;
	volatile unsigned long GPIO_PIN=0;

	ulCurrentState=ulCurrentState;
	if( uxLED <= partstNUM_LEDS )
	{
		/* Toggle LED Status
		 * LED0 = GPIO_149
		 * LED1 = GPIO_150 */
		switch(uxLED){
			case 0: 
				{
					GPIO_PIN=PIN21;
					break;
				}
			case 1: 
				{
					GPIO_PIN=PIN22;
					break;
				}
			default: 
				{
					// Don't do nothing
					break;
				}
		};
	}

	ulCurrentState = (*(REG32 (GPIO5_BASE + GPIO_DATAOUT)));

// hongyk
(*(REG32 (GPIO5_BASE + GPIO_DATAOUT))) ^= GPIO_PIN;

}

