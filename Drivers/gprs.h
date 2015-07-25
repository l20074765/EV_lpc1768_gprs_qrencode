/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           gprs.h
** Last modified Date:  2015-08-04
** Last Version:         
** Descriptions:        MG323 GPRS驱动程序头文件                  
**------------------------------------------------------------------------------------------------------
** Created by:          yanbo 
** Created date:        2013-01-04
** Version:              
** Descriptions:        The original version       
********************************************************************************************************/


#ifndef _GPRS_H_
#define _GPRS_H_
#include "..\Drivers\board.h"


#define GPRS_TIMEOUT			0
#define GPRS_OK					1
#define GPRS_ERROR				2
#define GPRS_CME_ERROR			3
#define GPRS_OTHER				0xFF	

typedef struct {
	uint8 type;

}GPRS_ST;



uint8 GPRS_sendAT(int8 *cmd,uint8 len,uint32 timeout);
uint8 GPRS_AT_CSQ(void);
void GPRS_task(void);
#endif


/**************************************End Of File*******************************************************/
