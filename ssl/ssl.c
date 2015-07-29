#include "ssl.h"
#include "ssl_tls.h"

#include<stdlib.h> 

ssl_session ssn;
ssl_context ssl;


typedef struct
{
  uint32 State;
}rng_state;

rng_state rngs;

int RandVal(void* arg)
{
	int ret;
	ret = rand();
	return ret;
}

void ssl_task(void)
{
	int ret;
	//1. Start the connection 	TCP/IP connection

	//2.Initialize the SSL context

	ret = ssl_init(&ssl);
	if(ret != 0){
	    /* SSL initialization failed */
	    //printf(" failed \n\r ! ssl_init returned %d\n\r", -ret);
	    goto exit;
    }
  	//printf((" ok\n\r"));

	/* Set the current session as SSL client */
  	ssl_set_endpoint(&ssl, SSL_IS_CLIENT);
   	/* No certificate verification */
  	ssl_set_authmode(&ssl, SSL_VERIFY_NONE);
	/* Set the random number generator callback function */
  	ssl_set_rng(&ssl, RandVal, &rngs); 
exit:

	
}

/**************************************End Of File*******************************************************/
