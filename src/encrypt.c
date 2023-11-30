/* Created by Tianpai Zhang 2023-11-21
 * SIMPLE VPN CSCD58 final project
 */


/* NOTE: 
 *  1. ensure that error handling is comprehensive and robust
 *  2. This file assumes the necessary cerfiticate and key files are set up 
 *  3. SSL methods may have some compatibility issues 
 *  4. Some more configuration may be required
 */


#include encrypt.h

/* The methods can be changed according to final 
 * compatibility requirements
 */
#define SSL_CLIENT_METHOD SSLv23_client_method()
#define SSL_SERVER_METHOD SSLv23_server_method()


/* @brief: Initialize the SSL library
 * @param: None
 * @return: None
 */
void init_ssl_lib()
{
  SSL_load_error_strings();         
  OpenSSL_add_ssl_algorithms();     
}


/* @brief: Cleanup the SSL library
 * @param: None
 * @return: None
 */
void cleanup_ssl_lib()
{
  EVP_cleanup();     
}


/* @brief: Create a SSL context for the server
 * @param: None
 * @return: SSL_CTX * - SSL context
 */
SSL_CTX *create_ssl_server_context()
{
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  method = SSL_SERVER_METHOD
  ctx = SSL_CTX_new(method);

  if (!ctx) 
  {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  return ctx;
}


/* @brief: Create a SSL context for the client
 * @param: None
 * @return: SSL_CTX * - SSL context
 */
SSL_CTX *create_ssl_client_context()
{
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  method = SSL_CLIENT_METHOD
  ctx = SSL_CTX_new(method);

  if (!ctx) 
  {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  return ctx;
}


/* @brief: Configure the SSL context
 * @param: SSL_CTX *ctx - SSL context
 * @param: char *cert_path - path to the certificate file
 * @param: char *key_file - path to the key file
 * @return: None
 */
void configure_context(SSL_CTX *ctx, char *cert_path, char *key_file)
{
  /* cerfiticate file */
  int cfile = SSL_CTX_use_certificate_file(ctx, cert_path, SSL_FILETYPE_PEM);
  int privkey = SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM);
  
  if (cfile <= 0 || privkey <= 0) 
  {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  /* verify private key */ 
  if (!SSL_CTX_check_private_key(ctx)) 
  {
    fprintf(stderr, "Private key does not match the public certificate\n");
    exit(EXIT_FAILURE);
  }
}



/* @brief: Create a SSL connection
 * @param: int sockfd - socket file descriptor
 * @param: SSL_CTX *ctx - SSL context
 * @return: SSL * - SSL connection
 *          NULL - if error
 *          0    - if success
 */
SSL *ssl_connect(int sockfd, SSL_CTX *ctx)
{
  SSL *ssl; 
  SSL_set_fd(ssl, sockfd);
  if (ssl_connect(ssl) <= 0) 
  {
    ERR_print_errors_fp(stderr);
    SSL_free(ssl);
    return NULL;
  }
  return ssl;
}


/* @brief: Accept a SSL connection
 * @param: SSL_CTX *ctx - SSL context
 * @return: SSL * - SSL connection 
 *          0  - if success
 *          -1 - if error
 */
int ssl_accept_connection(SSL_CTX *ssl)
{
  if (SSL_accept(ssl) <= 0) 
  {
    ERR_print_errors_fp(stderr);
    return -1;
  }
  return 0;
}


/* @brief: Shutdown a SSL connection
 * @param: SSL *ssl - SSL connection
 * @return: None
 */
void ssl_shutdown(SSL *ssl)
{
  SSL_shutdown(ssl);
  SSL_free(ssl);
}