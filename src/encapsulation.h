/* Created by In Kim Nov 23, 2023
 * CSCD58 final project
 * Simple VPN
 */

#ifndef ENCAPSULATION_H
#define ENCAPSULATION_H

#include "protocol.h"

/* Params: Takes in a memory address of ip_packet pointer,
 * encapsulates it, and returns the same memory address with
 * the info filled.
 */
uint8_t *packet_encapsulate(uint8_t *packet);

/* Takes in a network interface name and finds its corresp.
 * MAC address. If this doesn't work for Mininet, just hardcode
 * the client MAC and IP address.
 */
char *get_mac_address(const char* interface);

/* Takes in a newwork interface name and finds its corresp.
 * IP address. If this doesn't work for Mininet, just hardcode
 * the client MAC and IP address.
 */
char* get_ip_address(const char* interface);
/* Takes in the ethernet and IP destination, the ethertype,
 * the IP protocol, the payload and its size, and then creates
 * a packet with all of the necessary info.
 */
int create_packets(char *eth_dest, char *ip_dest, char *eth_type, uint8_t ip_protocol, uint8_t payload, unsigned int payload_size);

#endif /* -- ENCAPSULATION_H -- */