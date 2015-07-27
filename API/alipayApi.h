#ifndef _ALIPAY_API_H_
#define _ALIPAY_API_H_

#include "..\Drivers\board.h"




typedef struct {
	uint8 type;

}ORDER_ST;


uint8 ALI_trade(ORDER_ST *order);

#endif

/**************************************End Of File*******************************************************/
