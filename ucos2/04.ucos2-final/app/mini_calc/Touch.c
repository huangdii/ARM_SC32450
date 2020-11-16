/*******************************************************************************
**	File:
**
**		Touch.c
**
**  Contents:
**		uC/OS-II programming
**
**	History:
**		Date        Name              Description
**		------------------------------------------------------------------------
**
*******************************************************************************/

#include "includes.h"
#include "2440addr.h"
#include "option.h"
#include "macro.h"
#include "ui.h"
#include "lcddrv.h"

#define PATTERN_TOUCH ((1<<7)|(1<<6)|(0<<5)|(1<<4))

// 터치패드가 눌림을 알수 있는 값
volatile int Touch_pressed = 0;
// ADC 값
volatile int ADC_x=0, ADC_y=0;
// Calibration 정보 저장 값
volatile int Cal_x1, Cal_y1, Cal_x2, Cal_y2; 
// 좌표 변환 값
volatile int Touch_x, Touch_y;
// Calibration 완료 값
volatile unsigned int Touch_config=0;

void Touch_Isr_Init(void (*fp)(void));
void Touch_ISR_Enable(int enable);

// 세마포어
extern OS_EVENT *TOUCH_SEM;
// button key for calculator {x,y,width,height}
extern const unsigned int TouchKey_pTable[][4];

//
// TOUCH 초기화
//
void Touch_Isr_Init(void (*fp)(void))
{
	rADCDLY=(50000);	
	// Enable Prescaler,Prescaler,AIN5/7 fix (MUX don't care),Normal,Disable read start,No operation
	rADCCON = (1<<14)+(39<<6)+(0<<3)+(0<<2)+(0<<1)+(0);	

	// Wait Down,YM:GND,YP:Hi-z,XM:Hi-z,XP:DIS,XP pullup En,Normal,Waiting for interrupt mode     
	rADCTSC=(0<<8)|PATTERN_TOUCH|(0<<3)|(0<<2)|(3);

	pISR_ADC = (unsigned int)fp;
}

//
// TOUCH 인터럽트 처리 루틴
//
void ISR_TOUCH(void)
{
	unsigned int temp = rINTMSK;
  	rINTMSK  = BIT_ALLMSK;	
  	
	rINTSUBMSK|=(BIT_SUB_ADC|BIT_SUB_TC);
	
	// Touch UP
	if(rADCTSC&0x100)
	{
		rADCTSC&=0xff;
		Touch_pressed = 0;
	}
	// Touch Down
	else 
	{
		rADCTSC=(0<<8)|PATTERN_TOUCH|(1<<3)|(1<<2)|(0);
		// SADC_ylus Down,Don't care,Don't care,Don't care,Don't care,XP pullup Dis,Auto,No operation
		rADCCON|=0x1;
		while(rADCCON & 0x1);
		while(!(0x8000&rADCCON));
		ADC_x=(int)(0x3ff&rADCDAT0);
		ADC_y=(int)(0x3ff&rADCDAT1);
		// Touch calibration complete
		if(Touch_config)
		{
			Touch_y=(ADC_y-Cal_y1)*(LCD_YSIZE-10)/(Cal_y2-Cal_y1)+5;
			Touch_x=(ADC_x-Cal_x2)*(LCD_XSIZE-10)/(Cal_x1-Cal_x2)+5;
			Touch_x=LCD_XSIZE-Touch_x;
			if(Touch_x<0) Touch_x=0;
			if(Touch_x>=LCD_XSIZE) Touch_x=LCD_XSIZE-1;
			if(Touch_y<0) Touch_y=0;
			if(Touch_y>=LCD_YSIZE) Touch_y=LCD_YSIZE-1;
		}
		// before calibration		
		else
		{
			Touch_x = ADC_x;
			Touch_y = ADC_y;
		}

		rADCTSC=(1<<8)|PATTERN_TOUCH|(0<<3)|(0<<2)|(3);
		// SADC_ylus Up,Don't care,Don't care,Don't care,Don't care,XP pullup En,Normal,Waiting mode
		Touch_pressed = 1;

		// 세마포어 전달(INPUT_SEM)
		OSSemPost(TOUCH_SEM);
	}
	rSUBSRCPND|=BIT_SUB_TC;
	rINTSUBMSK = (unsigned int)(~(BIT_SUB_TC));
	rSRCPND = (unsigned int)BIT_ADC;
	rINTPND = (unsigned int)BIT_ADC;  
	
	rINTMSK = temp;
}

void Touch_ISR_Enable(int enable)
{	
	enable? (rINTMSK &= (unsigned int)(~(BIT_ADC))) : (rINTMSK |= (unsigned int)(BIT_ADC));
	enable? (rINTSUBMSK &= (unsigned int)(~(BIT_SUB_TC))) : (rINTSUBMSK |= (unsigned int)(BIT_SUB_TC));	
}

//
// Touch 센서 성능 최적화(캘리브레이션)
//
void Touch_Calibration(void)
{
	INT8U err;
	
	Uart_Printf("\n터치 Calibration\n");
	
	Touch_ISR_Enable(1);   //
    	Lcd_Clr_Screen(WHITE);	     
  
	Lcd_Hline(5,0,10-1,BLUE);
	Lcd_Vline(5,0,10-1,BLUE);
	Lcd_Printf(10,40, BLUE, WHITE, 1,1,"Press the cross icon");
//	Timer0_Delay(200);

	//
	// timed 세마포어 대기(from TOUCH_ISR)
	OSSemPend(TOUCH_SEM, 0, &err);
	Cal_x1=ADC_x;  //
	Cal_y1=ADC_y;  //
	Uart_Printf("\nADC_x:%d, ADC_y:%d", ADC_x, ADC_y); 
	Touch_pressed = 0;
	
	Lcd_Hline(5,0,10-1,WHITE);
	Lcd_Vline(5,0,10-1,WHITE);
	Lcd_Printf(10,40, WHITE, WHITE, 1,1,"Press the cross icon");     
	
	Lcd_Hline(LCD_YSIZE-5,LCD_XSIZE-0,LCD_XSIZE-10+1,BLUE);
	Lcd_Vline(LCD_XSIZE-5,LCD_YSIZE-0,LCD_YSIZE-10+1,BLUE);
	Lcd_Printf(80,200, BLUE, WHITE, 1,1,"Press the cross icon");
//	Timer0_Delay(200);
	//
	// timed 세마포어 대기(from TOUCH_ISR)
	OSSemPend(TOUCH_SEM, 0, &err);
	Cal_x2=ADC_x;  //
	Cal_y2=ADC_y;  // 
	Uart_Printf("\nADC_x:%d, ADC_y:%d", ADC_x, ADC_y); 
	Touch_pressed = 0;  
	
	Lcd_Hline(LCD_YSIZE-5,LCD_XSIZE-0,LCD_XSIZE-10+1,WHITE);
	Lcd_Vline(LCD_XSIZE-5,LCD_YSIZE-0,LCD_YSIZE-10+1,WHITE);
	Lcd_Printf(80,200, WHITE, WHITE, 1,1,"Press the cross icon");

       Touch_config=1;  //

	Lcd_Printf(0,100,BLUE,GREEN,1,1,"터치를 누르면 COM1으로 위치 표시");   
	Lcd_Printf(0,140,BLUE,GREEN,1,1,"키보드 누르면 종료");  
	
	Uart_Printf("\n터치패널을 누르면 위치가 표시됩니다.\n");
	Uart_Printf("키보드를 누르면 다음동작으로 넘어갑니다.\n");
}

//
// TOUCH GET KEY
//
int TOUCHGetKey(int orig_x, int orig_y)
{
	int i;
	int xx, yy;
	int x,y;
	
	x= Touch_x;
	y= Touch_y;	
	xx = x-orig_x;
	yy = y-orig_y;
	
	for(i=0; i<POS_MAX; i++)
	{
		if((xx >= TouchKey_pTable[i][0]) && (xx <= (TouchKey_pTable[i][0] + TouchKey_pTable[i][2])))
		{
			if((yy >= TouchKey_pTable[i][1]) && (yy <= (TouchKey_pTable[i][1] + TouchKey_pTable[i][3])))
				return i;
		}
	}	
	return -1;
}

/*-----------------------------------------------------------------------------
 * Program : Touch.C
-----------------------------------------------------------------------------*/
