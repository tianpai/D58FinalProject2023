/* Created by Tianpai Zhang Nov 13, 2023
 * For CSCD58 final project
 */

#ifndef ENCRYPT_H
#define ENCRYPT_H

/* Ubuntu
 * 'sudo apt-get install libssl-dev'
 *
 * Fedora
 *  sudo dnf install openssl-devel
 * */
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>

/* Usage example
encrypt(plaintext, strlen((char *)plaintext), key, iv, ciphertext);
decrypt(ciphertext, sizeof(ciphertext), key, iv, decryptedtext);

the plaintext
*/

void decrypt(const uint8_t *ciphertext, int ciphertext_len, uint8_t *plaintext);
void encrypt(const uint8_t *plaintext, int plaintext_len, uint8_t *ciphertext);
#endif /* -- ENCRYPT_H -- */
