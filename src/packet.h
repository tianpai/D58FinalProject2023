#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <unistd.h>

/* Takes in the ethernet and IP destination, the ethertype,
 * the IP protocol, the payload and its size, and then creates
 * a packet with all of the necessary info.
 */
uint8_t *create_packets(const char *eth_src, const char *ip_src, 
                        const char *eth_dest, const char *ip_dest, 
                        uint8_t ip_protocol, const char *payload, 
                        uint8_t flags);

/* Takes in the socket FD, the pointer to packet, size of packet,
 * the protocol of IP, the payload size, and sends the packet through
 * the socket; packet freed after sending. Can be used for client or
 * server to send packet to the other side. Returns 0 on success, -1
 * if error.
 */
int send_and_free_packet_vpn(int sockfd, uint8_t *packet_to_send,
                    uint8_t ip_protocol, uint8_t payload_size);

/* Takes in the socket FD of client and receives the packet sent by
 * the client. Returns the pointer of the packet on success and NULL
 * if error occured.
 */
uint8_t *serv_rec_from_cli(int sockfd);

/* Prints the contents of the packet including all of the info from
 * ethernet header, gre header, ip header, tcp header, and the
 * payload message. 
 */
void print_packet(uint8_t *packet);
#endif /* -- PACKET_H -- */
