
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

#define VIR_IF "eth0"

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

inline ethernet_hdr_t *get_eth_hdr(uint8_t *packet_start) {
  return (ethernet_hdr_t *)packet_start;
}

inline ip_hdr_t *get_ip_hdr(uint8_t *packet_start) {
  return (ip_hdr_t *)(packet_start + sizeof(ethernet_hdr_t) +
                      sizeof(gre_hdr_t));
}

inline tcp_hdr_t *get_tcp_hdr(uint8_t *packet_start) {
  return (tcp_hdr_t *)(packet_start + sizeof(ethernet_hdr_t) +
                       sizeof(gre_hdr_t) + sizeof(ip_hdr_t));
}

/* Not finished; Might be due for major (possibly delete later) */
int create_packets(char *eth_dest, char *ip_dest, uint8_t ip_protocol,
                   uint8_t *payload, unsigned int payload_size, uint8_t flags) {
  /*
   * NOTE:
   * Create a new packet and set it up
   * The new packet should look like this:
   *  --------------------------------------------------------
   * |ethernet header|gre header|ip header|tcp header|payload |
   * --------------------------------------------------------
   */

  /*Change name of interface depending on fit */
  char *eth_host = get_mac_address(VIR_IF);
  char *ip_host = get_ip_address(VIR_IF);

  uint8_t packet_size = get_packet_size(ip_protocol, payload_size);

  /* why not use calloc? calloc set the memory to 0 automatically */
  /*
   * uint8_t *new_packet = (uint8_t *)malloc(packet_size);
   * memset(new_packet, 0, sizeof(uint8_t) * (packet_size));
   */
  uint8_t *new_packet = (uint8_t *)calloc(packet_size, sizeof(uint8_t));

  ethernet_hdr_t *new_eth = get_eth_hdr(new_packet);
  set_eth(new_eth, eth_dest, eth_host);
  packet_encapsulate(new_packet);

  ip_hdr_t *new_ip = get_ip_hdr(new_packet);
  set_ip(new_ip, ip_dest, ip_host, ip_protocol, payload_size);

  /* ==== For now, just assume the new packet is TCP ==== */
  tcp_hdr_t *new_tcp = get_tcp_hdr(new_packet);
  new_tcp->src_port = PORT;
  new_tcp->dst_port = 80; /* PORTS might need to be changed later */
  new_tcp->flags = flags;
  new_tcp->tcp_sum = cksum(new_tcp, sizeof(tcp_hdr_t));

  return -1; /* for now */
}
