/*******************************************************************************
**	File:
**
**		Buzzer.c
**
**  Contents:
**		uC/OS-II programming
**
**	History:
**		Date        Name              Description
**		------------------------------------------------------------------------
**
*******************************************************************************/

#include "2440addr.h"
#include "option.h"
#include "macro.h"

// for GPH10
void Buzzer_Init(void);
void Buzzer_Beep(int tone, int duration);

//
// 부저 초기화
//
void Buzzer_Init(void)
{
	// Buzzer = GPB3
	rGPBCON &= ~(0x3 << 6);
	rGPBCON |= 0x1<<6;
	rGPBDAT |= (0x1<<3);
}

//
// 비프 음 발생
//
void Buzzer_Beep(int tone, int duration)
{
	unsigned int temp;

	for( ;(unsigned)duration > 0; duration--)
	{
		rGPBDAT &= ~(0x1<<3);
		for(temp = 0 ; temp < (unsigned)tone; temp++);
		rGPBDAT |= (0x1<<3);
		for(temp = 0 ; temp < (unsigned)tone; temp++);
	}
}

/*-----------------------------------------------------------------------------
 * Program : Buzzer.C
-----------------------------------------------------------------------------*/
