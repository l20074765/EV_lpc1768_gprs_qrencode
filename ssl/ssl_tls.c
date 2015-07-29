
#include <string.h>
#include <stdlib.h>

#include "ssl_tls.h"



#define SSL_BUFFER_LEN (SSL_MAX_CONTENT_LEN + 512)

int ssl_init( ssl_context *ssl )
{
	int len = 	SSL_BUFFER_LEN;
	memset( ssl, 0, sizeof( ssl_context ) );
	ssl->in_ctr = (unsigned char *) malloc( len );
	ssl->in_hdr = ssl->in_ctr +  8;
	ssl->in_msg = ssl->in_ctr + 13;
	if( ssl->in_ctr == NULL ){
	SSL_DEBUG_MSG( 1, ( "malloc(%d bytes) failed", len ) );
		return( 1 );
	}
	ssl->out_ctr = (unsigned char *) malloc( len );
    ssl->out_hdr = ssl->out_ctr +  8;
    ssl->out_msg = ssl->out_ctr + 13;

	if( ssl->out_ctr == NULL )
    {
        SSL_DEBUG_MSG( 1, ( "malloc(%d bytes) failed", len ) );
        free( ssl-> in_ctr );
        return( 1 );
    }

	memset( ssl-> in_ctr, 0, SSL_BUFFER_LEN );
    memset( ssl->out_ctr, 0, SSL_BUFFER_LEN );
	 ssl->hostname = NULL;
    ssl->hostname_len = 0;


	 md5_starts( &ssl->fin_md5  );
	 sha1_starts( &ssl->fin_sha1 );



	 return 0;
}



/*
 * SSL set accessors
 */
void ssl_set_endpoint( ssl_context *ssl, int endpoint )
{
    ssl->endpoint   = endpoint;
}

void ssl_set_authmode( ssl_context *ssl, int authmode )
{
    ssl->authmode   = authmode;
}


void ssl_set_rng( ssl_context *ssl,
                  int (*f_rng)(void *),
                  void *p_rng )
{
    ssl->f_rng      = f_rng;
    ssl->p_rng      = p_rng;
}

/**************************************End Of File*******************************************************/
