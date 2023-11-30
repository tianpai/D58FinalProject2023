/* Created by Tianpai Zhang Nov 13, 2023
* For CSCD58 final project
*/

#ifndef UTILS_H
#define UTILS_H


uint16_t cksum(const void *_data, int len);
int verify_checksum(const void *buf, int len, uint16_t old_cksum);

/* 
 * The following functions are used to print out the headers of the packets
 * for debugging purposes.
 */

/* functions below are not defined in utils.c
 * implement them if needed
 */


/* 
uint16_t ethertype(uint8_t *buf);
uint8_t ip_protocol(uint8_t *buf);

void print_addr_eth(uint8_t *addr);
void print_addr_ip(struct in_addr address);
void print_addr_ip_int(uint32_t ip);

void print_hdr_eth(uint8_t *buf);
void print_hdr_ip(uint8_t *buf);
void print_hdr_icmp(uint8_t *buf);
void print_hdr_arp(uint8_t *buf);

void print_hdrs(uint8_t *buf, uint32_t length);
*/

#endif  /* -- UTILS_H -- */
