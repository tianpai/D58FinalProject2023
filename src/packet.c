
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "encap.h"
#include "packet.h"
#include "protocol.h"
#include "utils.h"
#include "host_info.h"

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

void set_tcp(tcp_hdr_t *tcp, uint8_t flags) {
  tcp->src_port = PORT;
  tcp->dst_port = 80; /* PORTS might need to be changed later depending on
                         destination's c code */
  tcp->flags = flags;
  tcp->tcp_sum = cksum(tcp, sizeof(tcp_hdr_t));
}

static inline ethernet_hdr_t *get_eth_hdr(uint8_t *packet_start) {
  return (ethernet_hdr_t *)packet_start;
}

static inline gre_hdr_t *get_gre_hdr(uint8_t *packet_start) {
  return (gre_hdr_t *)(packet_start + sizeof(ethernet_hdr_t));
}

static inline ip_hdr_t *get_ip_hdr(uint8_t *packet_start) {
  return (ip_hdr_t *)(packet_start + sizeof(ethernet_hdr_t) +
                      sizeof(gre_hdr_t));
}

static inline tcp_hdr_t *get_tcp_hdr(uint8_t *packet_start) {
  return (tcp_hdr_t *)(packet_start + sizeof(ethernet_hdr_t) +
                       sizeof(gre_hdr_t) + sizeof(ip_hdr_t));
}

static inline uint8_t *get_payload(uint8_t *packet_start) {
  return (packet_start + sizeof(ethernet_hdr_t) +
          sizeof(gre_hdr_t) + sizeof(ip_hdr_t) + sizeof(tcp_hdr_t));
}

/* Not finished; Might be due for major (possibly delete later) */
uint8_t *create_packets(const char *eth_dest, const char *ip_dest, 
                        uint8_t ip_protocol, uint8_t *payload, 
                        unsigned int payload_size, uint8_t flags) {
  /*
   * NOTE:
   * Create a new packet and set it up
   * The new packet should look like this:
   *  --------------------------------------------------------
   * |ethernet header|gre header|ip header|tcp header|payload |
   * --------------------------------------------------------
   */

  /*Change name of interface depending on fit */
  // char *eth_host = get_mac_address(VIR_IF);
  // char *ip_host = get_ip_address(VIR_IF);
  char *eth_host = get_host_mac("h1");
  char *ip_host = get_host_ip("h1");


  uint8_t packet_size = get_packet_size(ip_protocol, payload_size);

  uint8_t *new_packet = (uint8_t *)calloc(packet_size, sizeof(uint8_t));

  ethernet_hdr_t *new_eth = get_eth_hdr(new_packet);
  set_eth(new_eth, eth_dest, eth_host);
  packet_encapsulate(new_packet);

  ip_hdr_t *new_ip = get_ip_hdr(new_packet);
  set_ip(new_ip, ip_dest, ip_host, ip_protocol, payload_size);

  /* ==== For now, just assume the new packet is TCP ==== */
  tcp_hdr_t *new_tcp = get_tcp_hdr(new_packet);
  set_tcp(new_tcp, flags);

  return new_packet;
}

int send_packet_vpn(uint8_t *packet_to_send, size_t packet_size,
                    uint8_t ip_protocol, uint8_t payload_size) {

  size_t pack_len = (size_t)get_packet_size(ip_protocol, payload_size);
  if (send(PORT, packet_to_send, pack_len, 0)) {
    return -1;
  }

  return 0;
}

int cli_rec_pkt_vpn() {}

int serv_rec_pkt_vpn() {}

int handle_packet() {}


void print_eth_header(uint8_t *packet) {
  ethernet_hdr_t *eth_header = get_eth_hdr(packet);
  printf("------------------------------------\n");
  printf("Ethernet header\n");

  printf("ether_dhost:\t%s\n", eth_header->ether_dhost);
  printf("ether_shost:\t%s\n", eth_header->ether_shost);
  printf("ether_type:\t%d\n", eth_header->ether_type);
}

void print_gre_header(uint8_t *packet) {
  gre_hdr_t *gre_header = get_gre_hdr(packet);
  printf("------------------------------------\n");
  printf("GRE header\n");

  printf("c:\t%d\n", gre_header->c);
  printf("protocol:\t%d\n", gre_header->protocol);
  printf("gre_sum:\t%d\n", gre_header->gre_sum);
}

void print_tcp_header(uint8_t *packet) {
  tcp_hdr_t *tcp_header = get_tcp_hdr(packet);
  printf("------------------------------------\n");
  printf("TCP header\n");

  printf("src_port: %d\n", tcp_header->src_port);
  printf("dst_port: %d\n", tcp_header->dst_port);
  printf("seq_num: %d\n", tcp_header->seq_num);
  printf("ack_num: %d\n", tcp_header->ack_num);
  printf("data_offset: %d\n", tcp_header->data_offset);
  printf("flags: %d\n", tcp_header->flags);
  printf("window: %d\n", tcp_header->window);
  printf("tcp_sum: %d\n", tcp_header->tcp_sum);
  printf("urgent_pointer: %d\n", tcp_header->urgent_pointer);
}

void print_ip_header(uint8_t *packet) {
  ip_hdr_t *ip_header = get_ip_hdr(packet);
  printf("------------------------------------\n");
  printf("IP header\n");

  printf("ip_tos: %d\n", ip_header->ip_tos);
  printf("ip_len: %d\n", ip_header->ip_len); 
  printf("ip_id: %d\n", ip_header->ip_id);
  printf("ip_off: %d\n", ip_header->ip_off);
  printf("ip_ttl: %d\n", ip_header->ip_ttl);
  printf("ip_p: %d\n", ip_header->ip_p);
  printf("ip_sum: %d\n", ip_header->ip_sum);
  printf("ip_src: %d\n", ip_header->ip_src);
  printf("ip_dst: %d\n", ip_header->ip_dst);
}

void print_payload(uint8_t *packet) {
  uint8_t *payload = get_payload(packet);
  printf("------------------------------------\n");
  printf("Payload:\n %s", payload);
}

/**
 * print packet
 */
void print_packet(uint8_t *packet) {
  print_eth_header(packet);
  print_gre_header(packet);
  print_ip_header(packet);
  print_tcp_header(packet);
  print_payload(packet);
}