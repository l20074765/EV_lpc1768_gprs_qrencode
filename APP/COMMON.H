/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.h
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        系统参数定义及通用函数部分                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#ifndef __COMMON_H 
#define __COMMON_H

#define MAXLANGUAGETYPE 5 //系统支持最大的语言种类，目前暂定5种，中文，英文，土耳其，俄罗斯，葡萄牙





















/******************************************货道参数*****************************************************/
typedef struct	
{
	uint8_t		CrcCheck[2];					//CRC校验
} GOODSCHANNELPARAMETER;
/******************************************交易参数*****************************************************/
typedef struct	
{
	uint8_t		CrcCheck[2];					//CRC校验
} VENDINGPARAMETER;

/******************************************交易记录*****************************************************/
typedef struct	
{
	uint8_t		CrcCheck[2];					//CRC校验
} VENDINGLOG;

/******************************************位图格式*****************************************************/
typedef struct
{
	unsigned char Width;			//位图宽度
	unsigned char Hight;			//位图高度
	const unsigned char *Data;		//位图数据
} BMP;
/******************************************定时器设置*****************************************************/
typedef struct	
{
	uint8_t		PayoutTimer;					//出币等待定时器
	uint8_t		EscrowTimer;					//压抄等待定时器
	uint8_t		DispFreeTimer;					//交易界面空闲界面刷新定时器
	uint8_t		CheckDevStateTimer;				//检测设备故障状态定时器
	uint8_t		GetTubeTimer;					//检测设备故障状态定时器
	uint8_t		ChaxunTimer;					//交易界面输入货道查询定时器
	uint8_t		SaleTimer;					    //交易倒计时定时器
	uint8_t		PCRecTimer;					    //PC串口接收定时器
	uint8_t		ActionPCTimer;					//UBOX的维护模式下Action定时器
	uint8_t		offLinePCTimer;				    //UBOX的离线模式下定时器
	uint8_t     printTimer;                     //打印交易记录计时器
	uint8_t		selTimer;						//选货按键接收定时器

	//add by yoc zhihui 2013.12.16
	uint8_t    	zhihui_recv_timeout;
	uint8_t		zhihui_button_timeout;
	uint8_t     zhihui_trade_time;
	uint8_t		pcm_handle_timeout;	//售货机处理PC请求超时 add by yoc newAisle
	
	uint8_t		getTempTimer;				  //读取温度定时器
	unsigned short gprs_recv_over;
	unsigned int gprs_uart_timeout;

} TIMER;
extern TIMER Timer;



unsigned char XorCheck(unsigned char *pstr,unsigned short len);
unsigned short CrcCheck(unsigned char *msg, unsigned short len);


#endif
/**************************************End Of File*******************************************************/
