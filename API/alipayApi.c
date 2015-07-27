#include "..\config.h"
#include "alipayApi.h"
#include "..\json\md5.h"


#define ALI_DEBUG
#ifdef ALI_DEBUG
#define print_ali(...)	Trace(__VA_ARGS__)
#else
#define print_ali(...)
#endif





struct MAP_ST{
	char *key;
	char *value;
	struct MAP_ST *next;
};


typedef struct MAP_ST ALI_MAP;

static char ali_buf[1024] = {0};

const char *gatewayStr = "https://mapi.alipay.com/gateway.do?";
const char *charset = "utf-8";
const char *seller_email = "2544282805@qq.com";
const char *partner = "2088711021642556";
const char *ali_key = "q4qsttz5r4grwnr5y5h9shjzvjfxejco";
const char *str_precreate = "alipay.acquire.precreate";
const char *str_query = "alipay.acquire.query";    
    
    
static ALI_MAP head = {NULL,NULL,NULL};    


void *ALI_mapFree(ALI_MAP *head)
{
	ALI_MAP *p,*q;
	uint8 i = 0;
	p = head->next;
	while(p != NULL){
		q = p->next;
		print_ali("ALI_mapFree[%d]:key=%s value=%s\r\n",i++,p->key,p->value);
		free(p->key);
		free(p->value);
		free(p);
		p = q;
	}
	
	head->next = NULL;	
}    
    
ALI_MAP *ALI_mapInsert(char *key,char *value)
{
	uint8 i = 0;
	uint16 klen,vlen;
	ALI_MAP *p,*q,*t;
	char *k,*v;
	if(key == NULL || value == NULL){
		print_ali("ALI_mapInsert:NULL\r\n");
		return NULL;
	}

	klen = strlen(key);
	vlen = strlen(value);

	k = malloc(klen + 1);
	if(k == NULL){
		print_ali("ALI_mapInsert:malloc(klen + 1) NULL %d %d\r\n",klen);
		return 0;
	}
	v = malloc(vlen + 1);
	if(k == NULL){
		print_ali("ALI_mapInsert:malloc(vlen + 1) NULL %d %d\r\n",vlen);
		free(k);
		return 0;
	}

	t = malloc(sizeof(ALI_MAP));
	if(t == NULL){
		print_ali("ALI_mapInsert:malloc(sizeof(ALI_MAP)) NULL %d %d\r\n");
		free(k);free(v);
		return 0;
	}

	memset(k,0,klen + 1);
	memset(v,0,vlen + 1);

	strcpy(k,key);
	strcpy(v,value);

	t->key = k;
	t->value = v;
	t->next = NULL;

	p = &head;
	i = 0;
	while(p->next != NULL){
		q = p->next;
		p = q;
		i++;
	}

	p->next = t;

	print_ali("ALI:insert[%d]:key=%s,value=%s\r\n",i,t->key,t->value);
	return t;
	
}


char *ALI_createLinkString(ALI_MAP *head)
{
	ALI_MAP *p,*q;
	uint16 len;
	memset(ali_buf,0,sizeof(ali_buf));
	p = head;

	while(p->next != NULL){
		q = p->next;
		p = q;
	   	strcat(ali_buf,q->key);
		strcat(ali_buf,"=");
		strcat(ali_buf,q->value);
		strcat(ali_buf,"&");
	}
	len = strlen(ali_buf);
	if(len > 0){ //去掉最後一個&字符
	   ali_buf[len - 1] = '\0';
	}
	return 	ali_buf;
}


//添加MD5校验
char *ALI_buildRequestMysign(ALI_MAP *head)
{
	uint8 len,i,buf[16] = {0};
	char *md5,*list;
	list = ALI_createLinkString(head);
	strcat(list,ali_key);
	MDString(list,buf);

	md5 = malloc(36);
	memset(md5,0,sizeof(36));
	for(i = 0;i < 16;i++){
		sprintf(&md5[i * 2],"%02d",buf[i]);
	}
	md5[32] = '\0';
	

	return md5;
}




uint8 ALI_trade(ORDER_ST *order)
{
	struct MAP_ST *p;
	uint16 len;
	uint8 res;
	char *key,*value,*text;
	
	cJSON *root,*item,*arr;


	p = ALI_mapInsert("service",(char *)gatewayStr);
	p = ALI_mapInsert("partner",(char *)partner);
	p = ALI_mapInsert("seller_email",(char *)seller_email);
	p = ALI_mapInsert("_input_charset",(char *)charset);
	p = ALI_mapInsert("product_code","QR_CODE_OFFLINE");
	

	arr = cJSON_CreateArray();
	if(arr == NULL){
		print_ali("cJSON_CreateArray == NULL");
		return 0;
	}
	item = cJSON_CreateObject();
	if(item == NULL){
		print_ali("cJSON_CreateObject == NULL");
		cJSON_Delete(arr);
		return 0;
	}

	cJSON_AddItemToArray(arr,item);

    cJSON_AddStringToObject(item,"goodsName","Yibao");
    cJSON_AddStringToObject(item,"quantity","1");
	cJSON_AddStringToObject(item,"price","0.01");
	text = cJSON_Print(arr);
	cJSON_Delete(arr);
	print_ali("text=%s\r\n",text);

	p = ALI_mapInsert("total_fee","0.01");
	p = ALI_mapInsert("out_trade_no","yoc201507281214");
	p = ALI_mapInsert("subject","subject");
	p = ALI_mapInsert("goods_detail",text);
	free(text);


	text = ALI_buildRequestMysign(&head);

	//签名结果与签名方式加入请求提交参数组中
	p = ALI_mapInsert("sign",text);
	p = ALI_mapInsert("sign_type","MD5");
    text = ALI_createLinkString(&head);
	ALI_mapFree(&head);
	len = strlen(text);
	print_ali("ALI_trade:order=");
	Uart0PutStr(text,len);
	print_ali("\r\n");


	res = GPRS_httpPost(text,"https://mapi.alipay.com:80/gateway.do?_input_charset=utf-8");
	if(res == 0)
		print_ali("GPRS_http:FAIL GPRS_http:FAIL GPRS_http:FAIL GPRS_http:FAILres=%d\r\n\r\n",res);


	
}
