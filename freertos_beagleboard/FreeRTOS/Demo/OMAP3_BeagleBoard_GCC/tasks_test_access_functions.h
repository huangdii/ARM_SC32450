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

void __attribute__((noinline)) serial_printf(char *fmt,...);

void vLogicSniffMultiSetLED5( unsigned int channel, int xValue );
void vLogicSniffMultiSetLED6( unsigned int channel, int xValue );
void vLogicSniffSetLED( unsigned int channel, int xValue );

BaseType_t prvExampleTaskHook( void * pvParameter )
{
  pvParameter = pvParameter; // for compiler warning

/* Perform an action. This could be anything. In this example, the hook is used to output debug trace information. pxCurrentTCB is the handle of the currently executing task. (vWriteTrace() is not an API function. It's just used as an example.) */
	vLogicSniffMultiSetLED5(LS_CHANNEL_ALL5, 0); // All Leds Clear
	vLogicSniffMultiSetLED6(LS_CHANNEL_ALL6, 0); // All Leds Clear
	vLogicSniffSetLED( pxCurrentTCB->uxTCBNumber, 1 );

#if 0
	serial_printf("(T%d) ", pxCurrentTCB->uxTCBNumber);
#endif

/* This example does not make use of the hook return value so just returns 0 in every case. */
	return 0;
}
