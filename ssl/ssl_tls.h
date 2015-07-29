#ifndef _SSL_TLS_H_
#define _SSL_TLS_H_
#include "..\Drivers\board.h"
#include "ssl.h"




int ssl_init( ssl_context *ssl );
void ssl_set_endpoint( ssl_context *ssl, int endpoint );
void ssl_set_authmode( ssl_context *ssl, int authmode );
void ssl_set_rng( ssl_context *ssl,
                  int (*f_rng)(void *),
                  void *p_rng );
#endif

/**************************************End Of File*******************************************************/
