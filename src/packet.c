
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "encap.h"
#include "protocol.h"
#include "utils.h"

#define PACKET_SIZE(header)                                                    \
  (uint8_t)(payload_size + sizeof(header) + sizeof(gre_hdr_t) +                \
            sizeof(ip_hdr_t) + sizeof(ethernet_hdr_t));

/* @brief: Gets the size of the packet
 * @param: uint8_t - packet size
 * @param: uint8_t - IP protocol code
 * @param: uint8_t - payload size
 * @return: uint8_t - packet size (in bytes), -1 if error
 */
uint8_t get_packet_size(uint8_t ip_protocol, uint8_t payload_size) {
  if (ip_protocol == ip_protocol_tcp) {
    return PACKET_SIZE(tcp_hdr_t);
  } else if (ip_protocol == ip_protocol_icmp) {
    return PACKET_SIZE(icmp_hdr_t);
  } else {
    fprintf(stderr, "-- Invalid ip protocol code of %u --\n",
            (unsigned int)ip_protocol);
  }
  return -1;
}

void set_eth(ethernet_hdr_t *eth, char *eth_dest, char *eth_host) {
  memcpy(eth->ether_dhost, eth_dest, sizeof(uint8_t) * ETHER_ADDR_LEN);
  memcpy(eth->ether_shost, eth_host, sizeof(uint8_t) * ETHER_ADDR_LEN);
  eth->ether_type = htons(ethertype_ipv4);
}

void set_ip(ip_hdr_t *ip, char *ip_dest, char *ip_host, uint8_t ip_protocol,
            uint8_t payload_size) {
  ip->ip_v = IP_VERSION;
  /* No need for ip_hl since the ip header size is fixed */
  /* No need for ip_tos since it's not used */
  ip->ip_len = htons(payload_size + sizeof(ip_hdr_t));
  /* No need for ip_id, ip_off since fragmentation not used */
  ip->ip_p = ip_protocol;
  ip->ip_src = (uint32_t)strtoul(ip_host, NULL, 10);
  ip->ip_dst = (uint32_t)strtoul(ip_dest, NULL, 10);
  ip->ip_sum = 0;
  ip->ip_sum = cksum(ip, sizeof(ip_hdr_t));
}

/* Not finished; Might be due for major (possibly delete later) */
int create_packets(char *eth_dest, char *ip_dest, uint8_t ip_protocol,
                   uint8_t payload, unsigned int payload_size) {

  /* Getting MAC and IP address for source */
  char *eth_host =
      get_mac_address("eth0"); /* Change name of interface depending on fit */
  char *ip_host =
      get_ip_address("eth0"); /* Change name of interface depending on fit */

  uint8_t packet_size = get_packet_size(ip_protocol, payload_size);

  /* Allocating memory for a new packet and setting it up */
  uint8_t *new_packet = (uint8_t *)malloc(packet_size);
  memset(new_packet, 0, sizeof(uint8_t) * (packet_size));

  /* Setting ethernet header info */
  ethernet_hdr_t *new_eth = (ethernet_hdr_t *)new_packet;
  set_eth(new_eth, eth_dest, eth_host);

  /* Encapsulating the packet with GRE */
  packet_encapsulate(new_packet);

  /* Setting IP header info. */
  ip_hdr_t *new_ip =
      (ip_hdr_t *)(new_packet + sizeof(ethernet_hdr_t) + sizeof(gre_hdr_t));
  set_ip(new_ip, ip_dest, ip_host, ip_protocol, payload_size);

  /* Checking if new packet is TCP */
  if (ip_protocol == ip_protocol_tcp) {
    tcp_hdr_t *new_tcp = (tcp_hdr_t *)(new_packet + sizeof(ethernet_hdr_t) +
                                       sizeof(gre_hdr_t) + sizeof(ip_hdr_t));
    new_tcp->src_port = PORT;
    new_tcp->dst_port = 80; /* PORTS might need to be changed later */
    /* Rest of TCP headers will need to be finished later */
    /* The new packet is ICMP header */
  } else {
    icmp_hdr_t *new_icmp = (icmp_hdr_t *)(new_packet + sizeof(ethernet_hdr_t) +
                                          sizeof(gre_hdr_t) + sizeof(ip_hdr_t));
  }
  return -1; /* for now */
}
