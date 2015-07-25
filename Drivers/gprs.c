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
static char gprs_buf[GPRS_BUF_SIZE] = {0};
static uint16 gprs_len = 0;

GPRS_ST st_gprs;


uint8 GPRS_send(char *str,...)
{
	va_list arg_ptr;
	uint32 i,len;
	memset(gprs_buf,0,sizeof(gprs_buf));
	va_start(arg_ptr, str);
	len = vsprintf(gprs_buf,(const char *)str,arg_ptr);
	va_end(arg_ptr);
	UART_clear();
	UART_putStr(gprs_buf,len);
	print_gprs("GPRS_Send[%d]:%s\r\n",len,gprs_buf);
	return 1;
}

//返回 读取数据长度
uint16 GPRS_readAll(uint8 *buf,uint32 timeout,uint16 subTimeout)
{
	uint16 index = 0;
	Timer.gprs_uart_timeout = (timeout > 10) ? timeout / 10 : 1;
	print_gprs("GPRS_Recv:");
	while(Timer.gprs_uart_timeout){
		if(UART_isNotEmpty()){
			 buf[index++] =  UART_getchar();
			 print_gprs("%c",buf[index - 1]);
			 Timer.gprs_recv_over = (subTimeout > 50) ? subTimeout / 10 : 5;	 
		}
		else{
			msleep(10);
		}

		if(index > 0 && Timer.gprs_recv_over == 0){ //接收完成
			print_gprs("\r\n");
			return index;
		}
	}
	print_gprs("\r\n");
	return index;

}


//指定收入字符串 1正确 0不正确
uint8 GPRS_recvByStr(char *str,uint32 timeout,uint32 subTimeout){

	char buf[64] = {0};
	uint8 res;
	uint16 len;
	
	len = GPRS_readAll(buf,timeout,subTimeout);
	if(len > 0){
		if(strstr(buf,str) == NULL){
			return 0;
		}
		else{
			return 1;
		}	
	}
	else{
		return 0;
	}	
}




//0超时 1成功 2错误
uint8 GPRS_recv(char *buf,uint16 *len,uint32 timeout)
{
	*len = GPRS_readAll(buf,timeout,500);
	if(*len == 0){
		print_gprs("GPRS:GPRS_TIMEOUT!!!!\r\n");
		return GPRS_TIMEOUT;
	}

	if(strstr(buf,"\r\nOK\r\n") != NULL){
		print_gprs("GPRS:OK!!!!\r\n");
		return GPRS_OK;
	}
	
    if(strstr(buf,"\r\nERROR\r\n") != NULL){
		print_gprs("GPRS:ERROR!!!!\r\n");
		return GPRS_ERROR;
	}

	if(strstr(buf,"+CME ERROR:") != NULL){
		print_gprs("GPRS:+CME ERROR:!!!!\r\n");
		return GPRS_CME_ERROR;
	}

	return GPRS_OTHER; 

}

// 0 超时 1成功 2错误
uint8 GPRS_sendAT(int8 *cmd,uint8 len,uint32 timeout)
{
	uint8 res;
	GPRS_send(cmd);
	memset(gprs_buf,0,sizeof(gprs_buf));
	res = GPRS_recv(gprs_buf,&gprs_len,timeout);
	return res;
}



/*********************************************************************************************************
** Function name:       GPRS_AT_CSQ
** Descriptions:        查询模块无线信号 
** input parameters:    无
** output parameters:   无
** Returned value:      0表示通信故障 31表示信号很好  1-30
*********************************************************************************************************/
uint8 GPRS_AT_CSQ(void)
{
	uint8 res,rssi;
	char *p,*q,buf[10] = {0};
	res = GPRS_sendAT("AT+CSQ\r",sizeof("AT+CSQ\r"),1000);
	if(res == GPRS_OK){
		p = strstr((char *)gprs_buf,"+CSQ:");
		if(p == NULL){
			return 0;
		}
		p = p + 5 + 1; // "+CSQ:" + " "
	    q = strstr((char *)p,",");
		if(q == NULL){
			return 0;
		}
		memset(buf,0,sizeof(buf));
		strncpy(buf,p,q - p);
		rssi = atoi(buf);
		print_gprs("GPRS:rssi = %s %d\r\n",buf,rssi);
		return rssi;
	}
	else{
		print_gprs("GPRS:'AT+CSQ' Fail\r\n");
		return 0;	
	}
		  	
}




/*********************************************************************************************************
** Function name:       GPRS_AT_CREG
** Descriptions:        查询当前网络注册状态
** input parameters:    无
** output parameters:   无
** Returned value:      0表示没注册  1表示注册
*********************************************************************************************************/
uint8 GPRS_AT_CREG(void)
{
	uint8 res;
	char *p;
	res = GPRS_sendAT("AT+CREG?\r",sizeof("AT+CREG?\r"),1000);
   	if(res == GPRS_OK){
		 p = strstr((char *)gprs_buf,",");
		 if(p == NULL){
		 	return 0;
		 }
		 p = p + 1; //过略','
		 if(*p == '1' || *p == '5'){
		 	print_gprs("GPRS:AT+CREG? = OK\r\n");
		 	return 1;
		 }
		 else{
		 	return 0;
		 }
	}
	else{
		return 0;
	}


}

/*********************************************************************************************************
** Function name:       GPRS_createProfile
** Descriptions:        创建Profile连接
** input parameters:    无
** output parameters:   无
** Returned value:      0失败  1成功
*********************************************************************************************************/
uint8 GPRS_createProfile(void)
{
	uint8 res;
	res = GPRS_sendAT("AT^SICS=0,conType,GPRS0\r",sizeof("AT^SICS=0,conType,GPRS0\r"),1000);
   	if(res != GPRS_OK){
	    print_gprs("GPRS:'AT^SICS=0,conType,GPRS0' ERR!!!\r\n");
		return 0;
	}

	res = GPRS_sendAT("AT^SICS=0,apn,cmnet\r",sizeof("AT^SICS=0,apn,cmnet\r"),1000);
   	if(res != GPRS_OK){
	    print_gprs("GPRS:'AT^SICS=0,apn,cmnet' ERR!!!\r\n");
		return 0;
	}


	res = GPRS_sendAT("AT^SICS=0,user,yoc\r",sizeof("AT^SICS=0,user,yoc\r"),1000);
   	if(res != GPRS_OK){
	    print_gprs("GPRS:'AT^SICS=0,user,yoc' ERR!!!\r\n");
		return 0;
	}

	res = GPRS_sendAT("AT^SICS=0,passwd,123\r",sizeof("AT^SICS=0,passwd,123\r"),1000);
   	if(res != GPRS_OK){
	    print_gprs("GPRS:'AT^SICS=0,passwd,123' ERR!!!\r\n");
		return 0;
	}
	print_gprs("GPRS:GPRS_createProfile OK!\r\n");
	return 1;

}


/*********************************************************************************************************
** Function name:       GPRS_createProfileServer
** Descriptions:        创建Profile服务  此处建立TCP 套接字
** input parameters:    无
** output parameters:   无
** Returned value:      0失败  1成功
*********************************************************************************************************/
uint8 GPRS_createProfileServer(void)
{
	uint8 res;
	res = GPRS_sendAT("AT^SISS=1,srvType,socket\r",sizeof("AT^SISS=1,srvType,socket\r"),1000);
   	if(res != GPRS_OK){
	    print_gprs("GPRS:'AT^SISS=1,srvType,socket' ERR!!!\r\n");
		return 0;
	}

	res = GPRS_sendAT("AT^SISS=1,conId,0\r",sizeof("AT^SISS=1,conId,0\r"),1000);
   	if(res != GPRS_OK){
	    print_gprs("GPRS:'AT^SISS=1,conId,0' ERR!!!\r\n");
		return 0;
	}

	res = GPRS_sendAT("AT^SISS=1,address,\"socktcp://117.27.89.53:10001\"\r",sizeof("AT^SISS=1,address,\"socktcp://117.27.89.53:10001\"\r"),1000);
   	if(res != GPRS_OK){
	    print_gprs("GPRS:'AT^SISS=1,address,\"socktcp://117.27.89.53:10001\"' ERR!!!\r\n");
		return 0;
	}

	print_gprs("GPRS:GPRS_createProfileServer OK!\r\n");
	return 1;
}

/*********************************************************************************************************
** Function name:       GPRS_AT_SISO
** Descriptions:        打开连接
** input parameters:    无
** output parameters:   无
** Returned value:      0失败  1成功
*********************************************************************************************************/
uint8 GPRS_AT_SISO(uint8 ch)
{
	uint8 res;
	uint16 ln;
	char buf[20] = {0};
	char *p;
	ln = sprintf(buf,"AT^SISO=%d\r",ch);
	GPRS_send(buf);
	gprs_len = GPRS_readAll(gprs_buf,5000,1000);
   	if(gprs_len == 0){
	    print_gprs("GPRS:'AT^SISO=1' ERR!!!\r\n");
		return 0;
	}

	p = strstr(gprs_buf,"OK");
	if(p == NULL){
		print_gprs("GPRS:'AT^SISO=1' ERR!!!\r\n");
		return 0;
	}
	else{
		print_gprs("GPRS:GPRS_AT_SISO OK!\r\n");
		return 1;
	}
}

/*********************************************************************************************************
** Function name:       GPRS_tcpWrite
** Descriptions:        TCP套接字发送数据
** input parameters:    无
** output parameters:   无
** Returned value:      0失败  1成功
*********************************************************************************************************/
uint8 GPRS_tcpWrite(const char *data,uint16 len,uint8 ch)
{
	uint8 buf[64] = {0},ln,res;
	uint16 rlen = 0;
	memset(buf,0,sizeof(buf));
	ln = sprintf((char *)buf,"AT^SISW=%d,%d\r",ch,len);
	GPRS_send((char *)buf);
	memset(buf,0,sizeof(buf));
	ln = sprintf((char *)buf,"^SISW: %d,%d,%d",ch,len,len);
	res = GPRS_recvByStr(buf,5000,1000);
	if(res != 1){return 0;}
	
	res = GPRS_send((char *)data);
	rlen = GPRS_readAll(gprs_buf,10000,8000);
	if(rlen == 0){return 0;}
	else{
		memset(buf,0,sizeof(buf));
		ln = sprintf((char *)buf,"^SISW: %d,1",ch);
		if(strstr(gprs_buf,buf) == NULL){
			return 0;
		}

		memset(buf,0,sizeof(buf));
		ln = sprintf((char *)buf,"^SISR: %d, 1",ch);
		if(strstr(gprs_buf,buf) == NULL){
			return 0;
		}
	}

	memset(buf,0,sizeof(buf));
	ln = sprintf((char *)buf,"AT^SISR=%d,1500\r",ch);
	res = GPRS_send((char *)buf);
	rlen = GPRS_readAll(gprs_buf,10000,5000);

	return 1;
}


void GPRS_task(void)
{
	uint8 res;
	char buf[256] = {0};
	static flag = 0;
	if(flag == 0){flag = 1;GPRS_createProfile();GPRS_createProfileServer();}
	
	
	GPRS_AT_SISO(1);
	msleep(500);
	memset(buf,0,sizeof(buf));
	strcat(buf,"POST /yv/api/vmcCheckin HTTP/1.1\r\n");
	strcat(buf,"Accept:*/*\r\n");
	strcat(buf,"Accept-Language: zh-cn\r\n");
	strcat(buf,"Host: 117.27.89.53\r\n");
	strcat(buf,"Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(buf,"Content-Length: 48\r\n");
	strcat(buf,"\r\n");
	strcat(buf,"param={'vmc_no':'ev0001','vmc_auth_code':'0001'}");
	//strcat(buf,"\r\n");
	//strncat(gprs_buf,"\0",sizeof("\0"));
	print_gprs("GPRS:TCPSendLen=%d\r\n",strlen(buf));
	GPRS_tcpWrite(buf,strlen(buf),1);	 
	msleep(5000);
}








/**************************************End Of File*******************************************************/
