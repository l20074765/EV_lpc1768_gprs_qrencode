/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           gprs.h
** Last modified Date:  2015-08-04
** Last Version:         
** Descriptions:        MG323 GPRS驱动程序文件                  
**------------------------------------------------------------------------------------------------------
** Created by:          yanbo 
** Created date:        2013-01-04
** Version:              
** Descriptions:        The original version       
********************************************************************************************************/

#include "..\config.h"
#include "gprs.h"


#define GPRS_DEBUG
#ifdef GPRS_DEBUG
#define print_gprs(...)	Trace(__VA_ARGS__)
#else
#define print_gprs(...)
#endif



typedef void (*FUN_uartClear)(void);
typedef void (*FUN_UartPutStr)(unsigned char const *, unsigned int );
typedef unsigned char (*FUN_UartBuffIsNotEmpty)(void);
typedef unsigned char (*FUN_UartGetCh)(void);



static FUN_uartClear UART_clear = uart3_clr_buf;
static FUN_UartPutStr UART_putStr = Uart3PutStr;
static FUN_UartBuffIsNotEmpty UART_isNotEmpty = Uart3BuffIsNotEmpty;
static FUN_UartGetCh UART_getchar = Uart3GetCh;

#define GPRS_BUF_SIZE	1024
static uint8 gprs_buf[GPRS_BUF_SIZE] = {0};
static uint16 gprs_len = 0;

uint8 GPRS_send(int8 *cmd,uint16 len)
{
	memset(gprs_buf,0,sizeof(gprs_buf));
	UART_clear();
	strncpy((int8 *)gprs_buf,cmd,len);
	strcat((int8 *)gprs_buf,"\r");
	UART_putStr(gprs_buf,len+1);
	
	print_gprs("GPRS_Send[%d]:%s\r\n",len+1,gprs_buf);

	return 1;
}


//0超时 1成功 2错误
uint8 GPRS_recv(uint8 *buf,uint16 *len,uint32 timeout)
{
	uint8 ch,tbuf[10] = {0},in = 0,res,start = 0;
	uint16 i = 0;
	Timer.gprs_uart_timeout = (timeout > 10) ? timeout / 10 : 1;
	in = 0;
	start = 0;
	print_gprs("GPRS_Recv:");
	while(Timer.gprs_uart_timeout){
		if(UART_isNotEmpty()){
			ch = UART_getchar();
			buf[i++] = ch;
			*len = i;
			if(start == 1){
				tbuf[in++] = ch;	
				if(in > 10){in = 0;start = 0;}
			}
			print_gprs("%c",ch);
			if(i >= 2){
				if(buf[i - 2] == '\r' && buf[i - 1] == '\n'){ //前两个字符为 CR LF
					if(start == 0){  //开始接收关键字符
						start = 1;
						in = 0;
					}
					else{
						start = 0;
						res = strncmp((const char *)tbuf,"OK",2);
						if(res == 0){
							print_gprs("GPRS:OK!!!!\r\n");
							return GPRS_OK;
						}

						res = strncmp((const char *)tbuf,"ERROR",5);
						if(res == 0){
							print_gprs("GPRS:ERROR!!!!\r\n");
							return GPRS_ERROR;
						}	
					}	 
				}	
			}
		}
		else{
			msleep(10);
		}

	}
	print_gprs("GPRS:GPRS_TIMEOUT!!!!\r\n");
	return GPRS_TIMEOUT;
}

// 0 超时 1成功 2错误
uint8 GPRS_sendAT(int8 *cmd,uint8 len,uint32 timeout)
{
	uint8 res;
	GPRS_send(cmd,len);
	memset(gprs_buf,0,sizeof(gprs_buf));
	res = GPRS_recv(gprs_buf,&gprs_len,timeout);
	return res;
}












/**************************************End Of File*******************************************************/
