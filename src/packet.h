#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <unistd.h>

/* Takes in the ethernet and IP destination, the ethertype,
 * the IP protocol, the payload and its size, and then creates
 * a packet with all of the necessary info.
 */
int create_packets(char *eth_dest, char *ip_dest, uint8_t ip_protocol,
                   uint8_t *payload, unsigned int payload_size, uint8_t flags);
#endif /* -- PACKET_H -- */
