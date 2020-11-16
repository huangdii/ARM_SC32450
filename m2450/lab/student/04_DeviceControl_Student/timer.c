#include "2450addr.h"
#include "option.h"
#include "my_lib.h"

void Timer_Init(void)
{
	/* 
	* 	Timer0 Init 
	* Prescaler value : 255, dead zone length = 0
	* Divider value : 1/16, no DMA mode
	* New frequency : (PCLK/(Prescaler value+1))*Divider value = (66Mhz/(256))*(1/16)
	*				= 16.113Khz(16113Hz)  << clock speed
	*/
 /* TODO : Timer Clock Configure */
	rTCFG0 = (rTCFG0 &~ (0xff<<8))|255;  // prescaler setting
	rTCFG1 = (rTCFG1 &~ (0xf<<8))|3;  // divider settinng
	
	/* TCON설정 :Dead zone disable,  auto reload on, output inverter off
	*  manual update no operation, timer0 stop, TCNTB0=0, TCMPB0 =0
	*/
	rTCON  = (0<<4)|(1<<3)|(0<<2)|(0<<1)|(0);
	rTCNTB0 = 0;
	rTCMPB0 = 0;
 
}

void Timer_Delay(int msec)
{
	/*
	* 1) TCNTB0설정 : 넘겨받는 data의 단위는 msec이다.
	*                  따라서 msec가 그대로 TCNTB0값으로 설정될 수는 없다.
	* 2) manual update후에  timer0를 start시킨다. 
	* 	 note : The bit has to be cleared at next writing.
	* 3) TCNTO0값이 0이 될때까지 기다린다. 	
	*/
 /* TODO : Timer Counter Setting */
	//rTCNTB0 = 0 * msec;
  rTCNTB0 = 16.113 *1000; // 16113Hz/1000(msec)*1000(msec)

	rTCON |= (1<<1)|(0);  //manual update -> When manual update bit is 1, TCNTBn and TCMPBn value are loaded to TCNTn and TCMPn.
	//
	rTCON &= ~(1<<1);  // manual update : make 0 
	
	rTCON |= 1;	 //trigger(timer start!)
	
	while(rTCNTO0 != 0); //rTCNTO0
	
}


//
// Use Timer0 for PC_ElapsedStart
// Timer INPUT clock Resolution 1.5625MHz 
void ElapseTimer_Start(void)
{
 rTCFG0 = (rTCFG0 & ~0xff) | 1;
 rTCFG1 = (rTCFG1 & ~0xf) | 3; 
 
 /* TCON설정 :Dead zone disable,  auto reload on, output inverter off
 *  manual update no operation, timer0 stop, TCNTB0=0, TCMPB0 =0
 */
 rTCON  = (rTCON & ~((1<<4)|(1<<2)|(1<<1)|(1))) | (1<<3);

 rTCNTB0 = 0;
 rTCMPB0 = 0;

 rTCNTB0 = 0xffff; // initial value(65535) for timer//  set MAX value to execute downcounting // 65535 = 1562500 * ??   --> ?? = 65535/1562500 = 0.04sec = 40 msec
 // 40msec time delay avaliable // if you want to estimate broader time range, you should change initial TCNTB value.

 rTCON |= (1<<1)|(0);  //manual update
 rTCON &= ~(1<<1);     // manual update
 rTCON |= 1;  
}

//
// Use Timer0 for PC_ElapsedStart
// Timer Resolution 1.5625MHz
unsigned int ElapseTimer_Stop(void)  // time observation
{
 rTCON &= ~1;    //timer stop

 return (0xffff - rTCNTO0);   // - current timer  = time delay value
 //return (0xffff - rTCNTO0)/1.562500 : sec unit time delay value;
}

