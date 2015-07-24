#include "..\config.h"
#include "..\qrencode\qrencode.h"


#define LCD_WIDTH	240
#define LCD_HIGHT	128
#define LCD_PAGE_SUM	16
#define LCD_PAGE_SIZE	8

void QR_draw(const unsigned char x,char *text)
{
	unsigned char k,m,x1,y1,x2,y2;
	unsigned char s;
	QRcode *qrcode;
	char *p;
	unsigned char data[LCD_WIDTH] = {0};
	unsigned char i,j;

	LCD_flush(x,124); //»­¾ØÐÎ¿ò
	Trace("QR_draw start:'%s'\r\n",text);
	qrcode = QRcode_encodeData(40,(unsigned char *)text,3,QR_ECLEVEL_M);
	if(qrcode == NULL){
		Trace("QRcode_encodeData:failed!\r\n");
		return;
	}

	s = qrcode->width > 0 ? qrcode->width : 1;
	p = (char *)qrcode->data;
	Trace("QR_draw end:w=%d\r\n",qrcode->width);
	
	for(i = x + 4;i < LCD_WIDTH;i++){
		for(j = 0;j < 4;j++){
			data[i] |= (0x01 << j);
		}
	}
	

	for( y2 = 0,y1 = 4; y2 < s;y2++,y1+=4){
		k = y1 / 8;
		m = y1 % 8;
		for( x2 = 0,x1= x + 4;x2 < s;x2++,x1+=4){
			 if(*p & 0x01){
			 	for(i = 0;i < 4;i++){
					for(j = 0;j < 4;j++){
						data[x1 + i] &= ~(0x01 << (m + j));
					}
				}
			 }
			 else{
			 	for(i = 0;i < 4;i++){
					for(j = 0;j < 4;j++){
						data[x1 + i] |= (0x01 << (m + j));
					}
				}	
			 }
			for(i = 0;i < 4;i++){
				LCDPutCh(x1 + i,k,data[x1 + i]);
			}
			p++;
		}
	}
	QRcode_free(qrcode);
	QRcode_clearCache();
        
}
