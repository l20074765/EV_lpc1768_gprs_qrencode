/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        主程序文件                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"

/*********************************************************************************************************
  全局变量
*********************************************************************************************************/


RTC_DATE RTCData;
RTC_DATE vmRTC;//更新屏幕的时间结构体

TIMER Timer;





/*********************************************************************************************************
  ucOS-II系统参数
*********************************************************************************************************/
static OS_STK MainTaskStk[2048];



/*********************************************************************************************************
** Function name:     	DispVersionText
** Descriptions:	    显示版本号，等待设备初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void DispVersionText()
{	
//	LCDPutLOGOBmp(24,LINE2,SystemPara.Logo);
//	LCDNumberFontPrintf(40,LINE15,2,"version%d.%02d",2,10);	
}

/*********************************************************************************************************
** Function name:       mainTask
** Descriptions:        主任务
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void MainTask(void *pvData)
{

	pvData = pvData;
	InitKeyboard();//初始化拨码(不在这里做的话，硬币器拨码就需要拨了，不然机器蓝屏)
	InitUart0();
	InitUart1();
	InitUart2();
	InitUart3();
    InitBuzzer();
    LCDInit();
	InitTimer(0,240000);
	InitRtc();	
	InitAT45DB();
	InitKeyboard();	
	
	while(1){
		LCDClrScreen();	   //清屏
		
		OSTimeDly(100);
		
		QR_draw(50,"http://www.easivend.com.cn");
		OSTimeDly(1000);
	}
	
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        用户程序入口函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main(void)
{
    targetInit();																
    pinInit();      															                                                                    
    OSInit();                                                                                                       
	OSTaskCreateExt(MainTask,(void *)0,&MainTaskStk[sizeof(MainTaskStk)/4 - 1],7,7,&MainTaskStk[0],256,(void *)0,OS_TASK_OPT_STK_CHK);
    OSStart();
	return 0;
}
/**************************************End Of File*******************************************************/
