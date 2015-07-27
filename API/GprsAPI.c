#include "..\config.h"
#define GPRS_API_DEBUG
#ifdef GPRS_API_DEBUG
#define print_gpi(...)	Trace(__VA_ARGS__)
#else
#define print_gpi(...)
#endif


void GPRS_task(void)
{
	uint8 res;
	GPRS_IP ip;
	ip.port = 10001;
	ip.ch = 1;
	strcpy(ip.ip,"117.27.89.53");
	

	ALI_trade(NULL);

	msleep(5000);

	
	
#if 0
	//GPRS_getIP("http://117.27.89.53:10001/yv/api/vmcCheckin",&ip);

	GPRS_createTcp(&ip);
	
	GPRS_AT_SISC(1);
	msleep(500);
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
#endif
}



/**************************************End Of File*******************************************************/
