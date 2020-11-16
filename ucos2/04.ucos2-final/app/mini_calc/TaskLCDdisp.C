/*******************************************************************************
**	File:
**
**		TaskLCDdisp.c
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
#include "macro.h"
#include "ui.h"
#include "lcddrv.h"

// 불러올 그래픽 화일을 지정
#include ".\images\J01.H"
#include ".\images\J02.H"

// Color Define
// [D15-D11]RED, [D10-D6]GREEN, [D5-D1]BLUE
#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800
#define YELLOW	0xffc0

// ISR Functions declaration

__irq void Touch_ISR(void);

extern unsigned int (* Fb_ptr)[LCD_XSIZE/2];

void Buzzer_Beep(int tone, int duration);

/* ...........................................................................
 *
 * 이벤트 컨트롤 블럭 정의
 * ===================
 */
// 세마포어
extern OS_EVENT *INPUT_SEM;
extern OS_EVENT *LCD_SEM;

// 메시지 큐 'LCD'
extern OS_EVENT *qLCD_id;	/* Message Queue Pointer */

// 전자계산기 메인 화면
void displayCalcMainTitle(void);

//
// 태스크 'TaskLCDdisp'
//
void TaskLCDdisp(void *pdata)
{
   	INT8U err;
	LCD_DISPLAY_MSG* p;	// message buffer

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

// 세마포어 대기(from TaskInput)
	OSSemPend(LCD_SEM, 0, &err);
 
// 전자계산기 메인 화면
	displayCalcMainTitle();
	
	for(;;) {
		// 메시지 큐 대기
		p = OSQPend(qLCD_id, 0, &err);
		if (err) Uart_Printf("[TaskLCDdisp]OSQPend error found, code[0x%X]\n",err);
		Uart_Printf("[TaskLCDdisp]GetMessage . . .%d\n",p->command);

		// 이벤트 핸들러
		if(p->command == UI_BUTTON_PRESSED)
		{
			// BEEP('click') SOUND
			Buzzer_Beep(100+11*100, 100);

			Lcd_Draw_BUTTON(p->param);
			Lcd_Display_Frame_Buffer(0);
		}
		else if(p->command == UI_LCDPRINTF)
		{
			Lcd_Printf(p->x, p->y, p->Color, p->bgColor, 1,1, p->msg);
		}
	}
}

//
// 전자계산기 메인 화면
//
void displayCalcMainTitle(void)
{
	int xtmp, ytmp;

	Lcd_Clr_Screen(WHITE);
	Lcd_Select_Frame_Buffer(0);
	Lcd_Get_Info_BMP(&xtmp, &ytmp, j01);
	Uart_Printf("0: %x\n", Fb_ptr); 

	Lcd_Draw_BMP((LCD_XSIZE-320)/2, (LCD_YSIZE-240)/2, j01);

	Lcd_Get_Info_BMP(&xtmp, &ytmp, j02);
	Lcd_GetINFO_BMP(0, 0, j02);  

//	sprintf(s, "uC/OS-II V%1d.%02d", OSVersion() / 100, OSVersion() % 100);
//	Lcd_Printf(26*8, 14*16, BLACK, WHITE, 1,1, s);

	Lcd_Select_Frame_Buffer(0);
	Lcd_Display_Frame_Buffer(0);
	Lcd_Wait_Blank();
}

/*-----------------------------------------------------------------------------
 * Program : TaskLCDdisp.C
-----------------------------------------------------------------------------*/
