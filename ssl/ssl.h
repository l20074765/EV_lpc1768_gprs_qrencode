#ifndef _SSL_H_
#define _SSL_H_
#include "..\Drivers\board.h"
#include "md5.h"
#include "sha1.h"


#define SSL_DEBUG_MSG(...) //Trace(__VA_ARGS__)


/*
 * Various constants
 */
#define SSL_MAJOR_VERSION_3             3
#define SSL_MINOR_VERSION_0             0   /*!< SSL v3.0 */
#define SSL_MINOR_VERSION_1             1   /*!< TLS v1.0 */
#define SSL_MINOR_VERSION_2             2   /*!< TLS v1.1 */

#define SSL_IS_CLIENT                   0
#define SSL_IS_SERVER                   1
#define SSL_COMPRESS_NULL               0

#define SSL_VERIFY_NONE                 0
#define SSL_VERIFY_OPTIONAL             1
#define SSL_VERIFY_REQUIRED             2

#define SSL_MAX_CONTENT_LEN         512





typedef struct _ssl_session ssl_session;
typedef struct _ssl_context ssl_context;
/*
 * This structure is used for session resuming.
 */
struct _ssl_session
{
//    time_t start;               /*!< starting time      */
    int cipher;                 /*!< chosen cipher      */
    int length;                 /*!< session id length  */
    unsigned char id[32];       /*!< session identifier */
    unsigned char master[48];   /*!< the master secret  */
    ssl_session *next;          /*!< next session entry */
};


struct _ssl_context
{


    /*
     * Miscellaneous
     */
    int state;                  /*!< SSL handshake: current state     */

    int major_ver;              /*!< equal to  SSL_MAJOR_VERSION_3    */
    int minor_ver;              /*!< either 0 (SSL3) or 1 (TLS1.0)    */

    int max_major_ver;          /*!< max. major version from client   */
    int max_minor_ver;          /*!< max. minor version from client   */

  
    /*
     * Callbacks (RNG, debug, I/O)
     */
    int  (*f_rng)(void *);
    void (*f_dbg)(void *, int, char *);
    int (*f_recv)(void *, unsigned char *, int);
    int (*f_send)(void *, unsigned char *, int);

    void *p_rng;                /*!< context for the RNG function     */
    void *p_dbg;                /*!< context for the debug function   */
    void *p_recv;               /*!< context for reading operations   */
    void *p_send;               /*!< context for writing operations   */

    /*
     * Session layer
     */
    int resume;                         /*!<  session resuming flag   */
    int timeout;                        /*!<  sess. expiration time   */
    ssl_session *session;               /*!<  current session data    */
    int (*s_get)(ssl_context *);        /*!<  (server) get callback   */
    int (*s_set)(ssl_context *);        /*!<  (server) set callback   */

    /*
     * Record layer (incoming data)
     */
    unsigned char *in_ctr;      /*!< 64-bit incoming message counter  */
    unsigned char *in_hdr;      /*!< 5-byte record header (in_ctr+8)  */
    unsigned char *in_msg;      /*!< the message contents (in_hdr+5)  */
    unsigned char *in_offt;     /*!< read offset in application data  */

    int in_msgtype;             /*!< record header: message type      */
    int in_msglen;              /*!< record header: message length    */
    int in_left;                /*!< amount of data read so far       */

    int in_hslen;               /*!< current handshake message length */
    int nb_zero;                /*!< # of 0-length encrypted messages */

    /*
     * Record layer (outgoing data)
     */
    unsigned char *out_ctr;     /*!< 64-bit outgoing message counter  */
    unsigned char *out_hdr;     /*!< 5-byte record header (out_ctr+8) */
    unsigned char *out_msg;     /*!< the message contents (out_hdr+5) */

    int out_msgtype;            /*!< record header: message type      */
    int out_msglen;             /*!< record header: message length    */
    int out_left;               /*!< amount of data not yet written   */
#if 0
    /*
     * PKI layer
     */
    rsa_context *rsa_key;               /*!<  own RSA private key     */
    x509_cert *own_cert;                /*!<  own X.509 certificate   */
    x509_cert *ca_chain;                /*!<  own trusted CA chain    */
    x509_crl *ca_crl;                   /*!<  trusted CA CRLs         */
    x509_cert *peer_cert;               /*!<  peer X.509 cert chain   */
    char *peer_cn;                      /*!<  expected peer CN        */
#endif
    int endpoint;                       /*!<  0: client, 1: server    */
    int authmode;                       /*!<  verification mode       */
	#if 0
    int client_auth;                    /*!<  flag for client auth.   */
    int verify_result;                  /*!<  verification result     */

    /*
     * Crypto layer
     */
     dhm_context dhm_ctx;               /*!<  DHM key exchange        */
#endif
	 sha1_context fin_sha1;              /*!<  Finished SHA-1 checksum */
	 md5_context fin_md5;               /*!<  Finished MD5 checksum   */
    
	

    int do_crypt;                       /*!<  en(de)cryption flag     */
    int *ciphers;                       /*!<  allowed ciphersuites    */
    int pmslen;                         /*!<  premaster length        */
    int keylen;                         /*!<  symmetric key length    */
    int minlen;                         /*!<  min. ciphertext length  */
    int ivlen;                          /*!<  IV length               */
    int maclen;                         /*!<  MAC length              */

    unsigned char randbytes[64];        /*!<  random bytes            */
    unsigned char premaster[256];       /*!<  premaster secret        */

    unsigned char iv_enc[16];           /*!<  IV (encryption)         */
    unsigned char iv_dec[16];           /*!<  IV (decryption)         */

    unsigned char mac_enc[32];          /*!<  MAC (encryption)        */
    unsigned char mac_dec[32];          /*!<  MAC (decryption)        */

    unsigned long ctx_enc[128];         /*!<  encryption context      */
    unsigned long ctx_dec[128];         /*!<  decryption context      */
	
    /*
     * TLS extensions
     */
    unsigned char *hostname;
    unsigned long  hostname_len;

	
};

void ssl_task(void);

#endif

/**************************************End Of File*******************************************************/
