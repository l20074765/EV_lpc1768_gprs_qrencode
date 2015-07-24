/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        �������ļ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"

/*********************************************************************************************************
  ȫ�ֱ���
*********************************************************************************************************/


RTC_DATE RTCData;
RTC_DATE vmRTC;//������Ļ��ʱ��ṹ��

TIMER Timer;





/*********************************************************************************************************
  ucOS-IIϵͳ����
*********************************************************************************************************/
static OS_STK MainTaskStk[2048];



/*********************************************************************************************************
** Function name:     	DispVersionText
** Descriptions:	    ��ʾ�汾�ţ��ȴ��豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispVersionText()
{	
//	LCDPutLOGOBmp(24,LINE2,SystemPara.Logo);
//	LCDNumberFontPrintf(40,LINE15,2,"version%d.%02d",2,10);	
}

/*********************************************************************************************************
** Function name:       mainTask
** Descriptions:        ������
** input parameters:    pvData: û��ʹ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void MainTask(void *pvData)
{

	pvData = pvData;
	InitKeyboard();//��ʼ������(�����������Ļ���Ӳ�����������Ҫ���ˣ���Ȼ��������)
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
		LCDClrScreen();	   //����
		
		OSTimeDly(100);
		
		QR_draw(50,"http://www.easivend.com.cn");
		OSTimeDly(1000);
	}
	
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        �û�������ں���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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