/*******************************************************************************
**	File:
**
**		TaskInput.c
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
#include "ui.h"

extern sIMAGEINFO calcImg;

// Touch 센서 성능 최적화(캘리브레이션)
#define  KEY_SCAN_TASK_PRIO         TASK_1_PRIO      /* Set priority of keyboard scan task                      */

void Touch_Calibration(void);

// Key 함수 원형
INT8U  KeyGetKey(INT16U to);
void  KeyInit (int prio);

// TOUCH GET KEY
int TOUCHGetKey(int orig_x, int orig_y);

// 세마포어
extern OS_EVENT *TOUCH_SEM;
extern OS_EVENT *INPUT_SEM;
extern OS_EVENT *LCD_SEM;

// 광역 키코드 버퍼
int KeyCode;

const unsigned int 
TouchKeyMatrix[]={
	POS_C,0xFF,0xFF,0xFF,POS_RESULT,0xFF,POS_0,POS_PLUSMINUS,0xFF,POS_PLUS,
	POS_BACKSPACE,POS_1,POS_2,POS_3,POS_MINUS,0xFF,POS_4,POS_5,POS_6,POS_MUL,
	POS_CE,POS_7,POS_8,POS_9,POS_DIV
};

//
// 태스크 'TaskInput'
//
void TaskInput(void *pdata)
{
   	char button, touchkey;
   	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	// 터치 패드 캘리브레이션
	Touch_Calibration();

	// KEY PORT 초기화
	KeyInit(KEY_SCAN_TASK_PRIO);

	// 세마포어 전달(LCD_SEM)
	OSSemPost(LCD_SEM);
		
	for(;;) {
	//
	// timed 세마포어 대기(from TOUCH_ISR)
		OSTimeDly (10);	// 100msec
		
//		button= KeyGetKey(10);
		button= 0xFF;
		// 세마포어 확인(from TOUCH_ISR)
		if(OSSemAccept(TOUCH_SEM))
		{
			touchkey= TOUCHGetKey(calcImg.orig_x, calcImg.orig_y);
			if(touchkey != 0xFF)
			{
				KeyCode = touchkey;
				OSSemPost(INPUT_SEM);
				Uart_Printf("SCANKEY[TOUCH] = %d\n", KeyCode);
				continue;
			}
		}

		if(button != 0xFF)
		{
			// 세마포어 전달
			KeyCode = TouchKeyMatrix[button];
			if(KeyCode == 0xFF)
			{
				continue;
			}
			OSSemPost(INPUT_SEM);
			Uart_Printf("SCANKEY[BUTTON] = %d\n", button);
		}
	}
}

/*-----------------------------------------------------------------------------
 * Program : TaskInput.C
-----------------------------------------------------------------------------*/
