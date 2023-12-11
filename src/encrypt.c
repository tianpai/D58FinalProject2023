/* Created by Tianpai Zhang 2023-11-21
 * SIMPLE VPN CSCD58 final project
 */

/* NOTE:
 *  1. ensure that error handling is comprehensive and robust
 *  2. This file assumes the necessary cerfiticate and key files are set up
 *  3. SSL methods may have some compatibility issues
 *  4. Some more configuration may be required
 */

/* NOTE: For how to use SSL in client and server, please refer to the
 * documentation
 */

#include <openssl/aes.h>
#include <openssl/evp.h>

#include "encrypt.h"

/* key to encrypt the plaintext */
unsigned char *key = (unsigned char *)"14159265358979323846264338327950";
/* initialization vector */
unsigned char *iv = (unsigned char *)"1300192787661119";

/**
 * @brief encrypt the plaintext
 * @param plaintext: the plaintext to be encrypted
 * @param plaintext_len: the length of the plaintext
 * @param key: the key used to encrypt
 * @param iv: the iv used to encrypt
 * @param ciphertext: the ciphertext after encryption
 * @return void
 */
void encrypt(const unsigned char *plaintext, int plaintext_len,
             unsigned char *ciphertext) {
  EVP_CIPHER_CTX *ctx;
  int len;
  int ciphertext_len;

  // Create and initialize the context
  ctx = EVP_CIPHER_CTX_new();

  // Initialize the encryption operation
  EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

  // Provide the message to be encrypted
  EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
  ciphertext_len = len;

  // Finalize the encryption
  EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
  ciphertext_len += len;

  EVP_CIPHER_CTX_free(ctx);
}

/**
 * @brief decrypt the ciphertext
 * @param ciphertext: the ciphertext to be decrypted
 * @param ciphertext_len: the length of the ciphertext
 * @param key: the key used to decrypt
 * @param iv: the iv used to decrypt
 * @param plaintext: the plaintext after decryption
 * @return void
 */
void decrypt(const unsigned char *ciphertext, int ciphertext_len,
             unsigned char *plaintext) {
  EVP_CIPHER_CTX *ctx;
  int len;
  int plaintext_len;

  // Create and initialize the context
  ctx = EVP_CIPHER_CTX_new();

  // Initialize the decryption operation
  EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

  // Provide the message to be decrypted
  EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
  plaintext_len = len;

  // Finalize the decryption
  EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
  plaintext_len += len;

  EVP_CIPHER_CTX_free(ctx);
}
