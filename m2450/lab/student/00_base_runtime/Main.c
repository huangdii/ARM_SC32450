#include <stdio.h>
#include "2450addr.h"
#include "my_lib.h"
#include "macro.h"
#include <math.h> //to use Root sqaure

void MMU_Init(void);
double mysqrt(unsigned int src); //define sqrt function  
// 아이콘 그림들의 지정
#include "./button/RUN_ER.H"  
#include "./button/RUN_EP.H"

// 불러올 그래픽 화일을 지정
#include "./Images/myimg.h"
#include "./Images/myimg1.h"
#include "./Images/myimg2.h" //틀린그림찾기
#include "./Images/circle.h"  // red circle1
#include "./Images/circle1.h"  // red circle2
#include "./Images/circle2.h"  // all red circle

// Color Define
// [D15-D11]RED, [D10-D6]GREEN, [D5-D1]BLUE
#define BLACK	0x0000
#define WHITE		0xfffe
#define BLUE		0x003e
#define GREEN	0x07c0
#define RED		0xf800
#define YELLOW	0xffc0

// ISR Functions declaration

#define LCD_XSIZE 		(480)	
#define LCD_YSIZE 		(272)

// Functions Declaration

void HW_Initial(void);
void Show_Welcome(char * msg);

// Global Variables Declaration

// 터치패드가 눌림을 알수 있는 값
volatile int Touch_pressed = 0;
// ADC 값
volatile int ADC_x=0, ADC_y=0;

// Calibration 정보 저장 값
volatile int Cal_x1=848;
volatile int Cal_y1=656;
volatile int Cal_x2=186;
volatile int Cal_y2=349; 

// 좌표 변환 값
volatile int Touch_x, Touch_y;
// Calibration 완료 값
volatile unsigned int Touch_config=1;

void Touch_ISR(void) __attribute__ ((interrupt ("IRQ")));

void Touch_ISR()  //interrupt service routine // detect finger 
{
	rINTSUBMSK |= (0x1<<9);
	rINTMSK1 |= (0x1<<31);	
	
	/* TO DO: Pendng Clear on Touch */	
	rSUBSRCPND = (0x1<<9);  //pick3
	rSRCPND1 = (0x1<<31);   
	rINTPND1 = (0x1<<31);
	
	// Touch UP    // when user detach finger from screen
	if(rADCTSC&0x100)
	{
		rADCTSC&=0xff;
		Touch_pressed = 0;
	}
	// Touch Down  // when user puts finger on screen
	else 
	{
		rADCTSC=(0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(1<<3)|(1<<2)|(0);
		// SADC_ylus Down,Don't care,Don't care,Don't care,Don't care,XP pullup Dis,Auto,No operation
		rADCCON|=0x1;   //start ad converter
		while(rADCCON & 0x1); // run until 0 
		while(!(0x8000&rADCCON));
		ADC_x=(int)(0x3ff&rADCDAT0);
		ADC_y=(int)(0x3ff&rADCDAT1);
		// Touch calibration complete
		if(Touch_config)
		{
			//calculate real coordinate of touch point usig quan
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
			Touch_x = ADC_x;   // save AD converted value  
			Touch_y = ADC_y;
		}
   // TOUCH UP CODE // Waiting interrupt mode & Normal ADC conversion
		rADCTSC=(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
		// SADC_ylus Up,Don't care,Don't care,Don't care,Don't care,XP pullup En,Normal,Waiting mode
		Touch_pressed = 1; 
	}

	rINTSUBMSK &= ~(0x1<<9);
	rINTMSK1 &= ~(0x1<<31);
}
// Lower Layer Functions

void HW_Initial(void)
{
	// 필요한 초기화 루틴들을 기입한다
	Exception_Init();
	Uart_Init(115200);	
//	Led_Init();
//	Buzzer_Init();

	Timer0_Init();
	Graphic_Init(); // graphic controller h/w initializ3
	
	Touch_Isr_Init(Touch_ISR); // interrupt service function
	MMU_Init();
}

void Show_Welcome(char * msg)
{
	int a, j; 
	
	Uart_Printf("\n%s\n", msg);
	
	for(a=0;a<2;a++)
	{
		Led_Display(7);
		for(j=0; j<0x0ffff; j++);
		Led_Display(0);
		for(j=0; j<0x0ffff; j++);
	}
}

void main(void)
{
	int xtmp, ytmp;
	int xtmp2, ytmp2;
	//char* myPtr;
	int cnt1 = 0; //count 2 spots
	int cnt2 = 0; //count 2 spots
	
	char* myPtr;
	long* p1;
	char* p2=(char*)0x34000000;
	long temp;
	p1=p2;

	Uart_Init(115200);

	HW_Initial();

/* malloc test */
	myPtr= malloc(4096);
	*myPtr= 'W';
/* printf test */
	printf("c=%c\n", *myPtr);
	free(myPtr);

	temp = *p1;
	
	Show_Welcome("LCD & Touch Test");

	Lcd_Clr_Screen(BLACK);
	Lcd_Select_Frame_Buffer(0);
	Lcd_Get_Info_BMP(&xtmp, &ytmp, MYIMG);
	Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), MYIMG); 
	Lcd_Printf(10,254, YELLOW, BLACK, 1,1,"If you want to have fun with BTS picture, then press any key on terminal");
//double buffering
	Lcd_Select_Frame_Buffer(1);
	Lcd_Clr_Screen(BLUE);
	Lcd_Get_Info_BMP(&xtmp, &ytmp, MYIMG1);
	Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), MYIMG1);  
	Lcd_Printf(10,254, YELLOW, BLACK, 1,1,"If you want to have fun with BTS picture, then press any key on terminal");

	Uart_Printf("\nIf you want to have fun with BTS picture, then press any key!! \n");

	do
	{
		Lcd_Wait_Blank();
		Lcd_Display_Frame_Buffer(0);
		Timer0_Delay(1000);
		Lcd_Wait_Blank();
		Lcd_Display_Frame_Buffer(1);
		Timer0_Delay(1000);
	}while(!Uart_Get_Pressed());	
	
	Uart_Printf("\n터치2 Calibration\n");
	
	Touch_ISR_Enable(1);   //
    	Lcd_Clr_Screen(WHITE);	     
/*
#if 1 // 캘리브레이션 생략 
  
	Lcd_Hline(5,0,10-1,BLUE);
	Lcd_Vline(5,0,10-1,BLUE);
	Lcd_Put_Pixel(240,136, BLACK);
	Lcd_Put_Pixel(241,136, BLACK);
	Lcd_Put_Pixel(242,136, BLACK);
	Lcd_Put_Pixel(243,136, BLACK);
	Lcd_Put_Pixel(244,136, BLACK);
	Lcd_Put_Pixel(240,135, BLACK);
	Lcd_Put_Pixel(241,135, BLACK);
	Lcd_Put_Pixel(242,135, BLACK);
	Lcd_Put_Pixel(243,135, BLACK);
	Lcd_Put_Pixel(244,135, BLACK);
	Lcd_Put_Pixel(240,137, BLACK);

	Lcd_Printf(10,40, BLUE, WHITE, 1,1,"좌측상단에 보이는 '+' 표시를 누르세요");    
	Timer0_Delay(200);
	while(Touch_pressed == 0); //wait until user puts her finger 
	Uart_Printf("\nADC_x:%d, ADC_y:%d", ADC_x, ADC_y); //show coordinate data on screen  

	Touch_pressed = 0;
	
	Cal_x1=ADC_x;  //upper left-side calibration value
	Cal_y1=ADC_y;  //
	
	Lcd_Hline(5,0,10-1,WHITE);
	Lcd_Vline(5,0,10-1,WHITE);
    	Lcd_Clr_Screen(WHITE);	     

	Lcd_Hline(LCD_YSIZE-5,LCD_XSIZE-0,LCD_XSIZE-10+1,BLUE);
	Lcd_Vline(LCD_XSIZE-5,LCD_YSIZE-0,LCD_YSIZE-10+1,BLUE);
	Lcd_Printf(80,200, BLUE, WHITE, 1,1,"우측하단에 보이는 '+' 표시를 누르세요");
	Timer0_Delay(200);
	while(Touch_pressed == 0);
	Uart_Printf("\nADC_x:%d, ADC_y:%d", ADC_x, ADC_y); 
	Touch_pressed = 0;
	
	Cal_x2=ADC_x;  //lower right-side calibration value
	Cal_y2=ADC_y;  //     
	
	Lcd_Hline(LCD_YSIZE-5,LCD_XSIZE-0,LCD_XSIZE-10+1,WHITE);
	Lcd_Vline(LCD_XSIZE-5,LCD_YSIZE-0,LCD_YSIZE-10+1,WHITE);
    	Lcd_Clr_Screen(WHITE);	     

       Touch_config=1;  // calibration is ended 

  
	Lcd_Printf(0,100,BLUE,WHITE,1,1,"터치패드를 눌러보세요 x,y 좌표정보를 표시해 줍니다");   
	Lcd_Printf(0,140,BLUE,WHITE,1,1,"[터미널 키보드을 아무키나 누르면 종료]");  
	
	Uart_Printf("\n터치패널을 누르면 위치가 표시됩니다.\n");
	Uart_Printf("키보드를 누르면 다음동작으로 넘어갑니다.\n");	 	
	
	do
	{
		if(Touch_pressed == 1)
		{
			Uart_Printf("\nX:%d, Y:%d", Touch_x, Touch_y); 
			Touch_pressed = 0;
		}
	}while(!Uart_Get_Pressed());	
	

	*/
// 	/* Touch-panel Test */
// 	Uart_Printf("\n터치패널 응용 실습\n");
// 	Uart_Printf("그림을 눌러봅시다\n");	
// #endif

/*
	Uart_Printf("\nCal_x1=%d\n", Cal_x1);	
	Uart_Printf("\nCal_x2=%d\n", Cal_x2);	
	Uart_Printf("\nCal_y1=%d\n", Cal_y1);	
	Uart_Printf("\nCal_y2=%d\n", Cal_y2);
	
*/	
//double buffering
  Lcd_Clr_Screen(BLUE);
	Lcd_Select_Frame_Buffer(2);
	Lcd_Get_Info_BMP(&xtmp, &ytmp, MYIMG2);
	Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), MYIMG2);  
	Lcd_Display_Frame_Buffer(2);

//original code start
//	Lcd_Clr_Screen(BLACK);    
	Lcd_Get_Info_BMP(&xtmp, &ytmp, run_er);     

//	Lcd_Printf(10,40, YELLOW, BLACK, 1,1,"Touch the different parts");

//WDT Init(); //revise @ 11:40
//WDT_Start(1);//revise @ 11:40
	for(;;)
	{

	Lcd_Select_Frame_Buffer(2);
	Lcd_Get_Info_BMP(&xtmp, &ytmp, MYIMG2);
	Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), MYIMG2);  
	Lcd_Display_Frame_Buffer(2);
	Lcd_Printf(10,40, YELLOW, BLACK, 1,1,"Touch the different parts");
		//Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), CIRCLE); //Draw icon  

	//WDT Clear();//revise @ 11:40
		for(;;)
		{
			
			if(Touch_pressed == 1)
			{
				// if((Touch_x >= (160 - (xtmp/2))) && (Touch_x <= (160 + (xtmp/2))) && (Touch_y >= (120 - (ytmp/2))) && (Touch_y <= (120 + (ytmp/2))))
				// {
				// 	Uart_Printf("\n Pressed"); 
				// 	Lcd_Printf(80,120, BLACK, RED, 1,1," PUSH ");
				// 	break;
				// }

				if((Touch_x >= (445 - 20)) && (Touch_x <= (445 + 20)) && (Touch_y >= (204 - 20)) && (Touch_y <= (204 + 20)))
				{
					//draw red circle pixelxpixel

					int center_x = 446; // wrong part coordinate
					int center_y = 204;
					int Radius = 30; //radius of red circle
					double xpix, ypix;
					  // Lcd_Clr_Screen(BLACK);  
					  Lcd_Select_Frame_Buffer(3);
						Lcd_Get_Info_BMP(&xtmp, &ytmp, CIRCLE); 
						Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), CIRCLE);  
						Lcd_Display_Frame_Buffer(3);
					// for(xpix=center_x-Radius;xpix<center_x+Radius+1;xpix++)
					// {

					// 	unsigned int temp = Radius*Radius-(xpix-center_x)*(xpix-center_x);
					// 	ypix = center_y +mysqrt(temp);
					// 	Lcd_Put_Pixel(xpix,ypix,RED);
					// 	ypix = center_y -mysqrt(temp);						
					// 	Lcd_Put_Pixel(xpix,ypix,RED);
					// }

					Uart_Printf("\n Correct !!"); 
					Lcd_Printf(200,0, BLACK, RED, 1,1," GOOD JOB ! ");
					Timer0_Delay(1000);
					cnt1 = 5;
					break;
				}

				else if((Touch_x >= (343 - 20)) && (Touch_x <= (343 + 20)) && (Touch_y >= (105 - 20)) && (Touch_y <= (105 + 20)))
				{
					//draw red circle pixelxpixel
					  // Lcd_Clr_Screen(BLACK);  
					  Lcd_Select_Frame_Buffer(4);
						Lcd_Get_Info_BMP(&xtmp, &ytmp, CIRCLE1); 
						Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), CIRCLE1);  
						Lcd_Display_Frame_Buffer(4);
					
					Uart_Printf("\n Correct !!"); 
					Lcd_Printf(200,0, BLACK, RED, 1,1," GOOD JOB ! ");
					Timer0_Delay(1000);
					cnt2 = 7;
					break;
				}

				}
			}
			


		if(cnt1+cnt2 == 12)
			{
					  Lcd_Clr_Screen(BLACK);  
					  Lcd_Select_Frame_Buffer(5);
						Lcd_Get_Info_BMP(&xtmp, &ytmp, CIRCLE2); 
						Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), CIRCLE2);  
						Lcd_Display_Frame_Buffer(5);
					
					Uart_Printf("\n All clear !!"); 
					Lcd_Printf(200,0, BLACK, RED, 1,1," byebye jungkook~ ");
					break;
			}

		// Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), run_ep);    
	
		// for(;;)
		// {
		// 	if(Touch_pressed == 0) // 손 뗏을 떄 
		// 	{
		// 		// Uart_Printf("\n Released"); 
		// 		// Lcd_Printf(80,120, BLACK, BLACK, 1,1," PULL ");
		// 		// break;
		// 		Lcd_Clr_Screen(BLUE);
		// 		Lcd_Select_Frame_Buffer(2);
		// 		Lcd_Get_Info_BMP(&xtmp, &ytmp, MYIMG2);
		// 		Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), MYIMG2);  
		// 		Lcd_Display_Frame_Buffer(2);
		// 		break;
		// 	}
		// }
	}

}


double mysqrt(unsigned int src)
{
    unsigned int NUM_REPEAT = 16;
    unsigned int k;
    double t;
    double buf = (double)src;
    for(k=0,t=buf;k<NUM_REPEAT;k++)
    {
        if(t<1.0)
            break;
        t = (t*t+buf)/(2.0*t);
    }
    return t;
}
