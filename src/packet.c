
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
  } else {
    fprintf(stderr, "-- Invalid ip protocol code of %u --\n",
            (unsigned int)ip_protocol);
  }
  return -1;
}

static inline void free_packet(uint8_t *packet) {
  free(packet);
}

/* ===================================================================*/
/* below are functions to set up each header                          */
/* ===================================================================*/

void set_eth(ethernet_hdr_t *eth, const char *eth_dest, const char *eth_host) {
  const char *eth_dest_addr = get_host_mac(eth_dest);
  const char *eth_host_addr = get_host_mac(eth_host);

  parse_mac_addr(eth->ether_dhost, eth_dest_addr);
  parse_mac_addr(eth->ether_shost, eth_host_addr);
  
  eth->ether_type = htons(ethertype_ipv4);
}

void set_ip(ip_hdr_t *ip, const char *ip_dest, const char *ip_host,
            uint8_t ip_protocol, uint8_t payload_size) {
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

/* @brief: Sets the payload msg
 * @param: const char * - payload msg
 * @return: uint8_t - packet size (in bytes), -1 if error
 */

/* ===================================================================*/
/* Below are functions to get the starting position of each header    */
/* ===================================================================*/

static inline ethernet_hdr_t *get_eth_hdr(uint8_t *packet_start) {
  return (ethernet_hdr_t *)packet_start;
}

static inline ip_hdr_t *get_ip_hdr(uint8_t *packet_start) {
  return (ip_hdr_t *)(packet_start + sizeof(ethernet_hdr_t) +
                      sizeof(gre_hdr_t));
}

static inline gre_hdr_t *get_gre_hdr(uint8_t *packet_start) {
  return (gre_hdr_t *)(packet_start + sizeof(ethernet_hdr_t));
}

static inline tcp_hdr_t *get_tcp_hdr(uint8_t *packet_start) {
  return (tcp_hdr_t *)(packet_start + sizeof(ethernet_hdr_t) +
                       sizeof(gre_hdr_t) + sizeof(ip_hdr_t));
}

static inline char *get_payload(uint8_t *packet_start) {
  return (char *)(packet_start + sizeof(ethernet_hdr_t) +
                       sizeof(gre_hdr_t) + sizeof(ip_hdr_t)
                       + sizeof(tcp_hdr_t));
}

/* ===================================================================*/
/* Below are functions to create a packet                             */
/* ===================================================================*/

uint8_t *create_packets(const char *eth_src, const char *ip_src,
                        const char *eth_dest, const char *ip_dest,
                        uint8_t ip_protocol, const char *payload,
                        uint8_t flags) {
/*
 * NOTE:
 * Create a new packet and set it up
 * The new packet should look like this:
 *  --------------------------------------------------------
 * |ethernet header|gre header|ip header|tcp header|payload |
 * --------------------------------------------------------
 */
  int payload_size = MAX_PAYLOAD_SIZE;

  uint8_t packet_size = get_packet_size(ip_protocol, payload_size);

  uint8_t *new_packet = (uint8_t *)calloc(packet_size, sizeof(uint8_t));

  ethernet_hdr_t *new_eth = get_eth_hdr(new_packet);
  set_eth(new_eth, eth_dest, eth_src);
  packet_encapsulate(new_packet);

  ip_hdr_t *new_ip = get_ip_hdr(new_packet);
  set_ip(new_ip, ip_dest, ip_src, ip_protocol, payload_size);

  /* ==== For now, just assume the new packet is TCP ==== */
  tcp_hdr_t *new_tcp = get_tcp_hdr(new_packet);
  set_tcp(new_tcp, flags);

  char *new_payload = get_payload(new_packet);
  strncpy(new_payload, payload, strlen(payload));
  new_payload[MAX_PAYLOAD_SIZE - 1] = '\0';

  return new_packet;
}

/**
 * send vpn encrypted packet
 * 
 * 
 */
int send_free_packet(int sockfd, uint8_t *packet_to_send, size_t packet_size,
                    uint8_t ip_protocol, uint8_t payload_size)
{

  size_t pack_len = (size_t)get_packet_size(ip_protocol, payload_size);
  if (send(sockfd, packet_to_send, pack_len, 0)) {
    return -1;
  }

  free_packet(packet_to_send);

  return 0;
}



int cli_rec_from_serv() { return -1; }

uint8_t *serv_rec_from_cli(int sockfd) 
{
  size_t pkt_size = sizeof(ethernet_hdr_t) + sizeof(ip_hdr_t) + sizeof(tcp_hdr_t) + MAX_PAYLOAD_SIZE;
  uint8_t *new_rec_pkt = (uint8_t *)calloc(pkt_size, sizeof(uint8_t));
  if (recv(sockfd, new_rec_pkt, pkt_size, 0) == -1) {
    return NULL;
  }

  return new_rec_pkt;
}

int serv_handle_pkt() { return -1; }

/* ===================================================================*/
/* Below are functions that prints packet infomation                  */
/* ===================================================================*/

void print_eth_header(uint8_t *packet) {
  ethernet_hdr_t *eth_header = get_eth_hdr(packet);
  printf("------------------------------------\n");
  printf("<<< Ethernet header >>>\n");

  printf("ether_dhost:\t");
  for(int i = 0; i < ETHER_ADDR_LEN; i++) {
    printf("%x:", eth_header->ether_dhost[i]);
  }
  printf("\b\n");

  printf("ether_shost:\t");
  for(int i = 0; i < ETHER_ADDR_LEN; i++) {
    printf("%x:", eth_header->ether_shost[i]);
  }
  printf("\b\n");

  printf("ether_type:\t%d\n", eth_header->ether_type);
}

void print_gre_header(uint8_t *packet) {
  gre_hdr_t *gre_header = get_gre_hdr(packet);
  printf("------------------------------------\n");
  printf("<<< GRE header >>>\n");

  printf("c:\t%d\n", gre_header->c);
  printf("protocol:\t%d\n", gre_header->protocol);
  printf("gre_sum:\t%d\n", gre_header->gre_sum);
}

void print_tcp_header(uint8_t *packet) {
  tcp_hdr_t *tcp_header = get_tcp_hdr(packet);
  printf("------------------------------------\n");
  printf("<<< TCP header >>>\n");

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
  printf("<<< IP header >>>\n");

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
  char *payload = get_payload(packet);
  printf("------------------------------------\n");
  fwrite(payload, sizeof(char), MAX_PAYLOAD_SIZE, stdout);

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
