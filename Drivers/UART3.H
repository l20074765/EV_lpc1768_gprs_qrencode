/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           Uart3.h
** Last modified Date:  2013-01-04
** Last Version:         
** Descriptions:        Uart3驱动程序相关函数声明                      
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-04
** Version:             V0.1 
** Descriptions:        The original version
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
********************************************************************************************************/
#ifndef __UART3_H 
#define __UART3_H
#include "..\Drivers\board.h"

#define	UART3_BUF_SIZE		1024
typedef struct {
	char recv[UART3_BUF_SIZE];
	char send[UART3_BUF_SIZE];
	uint16 rlen;
	uint16 slen;
}UART3_MSG;

extern UART3_MSG uart3_data;

void InitUart3(void);
void Uart3IsrHandler(void);
void Uart3PutChar(unsigned char ch);
void uart3_clr_buf(void);
void Uart3PutStr(unsigned char const *Str, unsigned int Len);
unsigned char Uart3BuffIsNotEmpty(void);
unsigned char Uart3GetCh(void);
unsigned char Uart3Read(unsigned char *buf, unsigned char len); 
uint16 uart3_recvBytes(void);
UART3_MSG *uart3_getPackge(void);
void uart3_start(void);
void uart3_stop(void);
#endif
/**************************************End Of File*******************************************************/
