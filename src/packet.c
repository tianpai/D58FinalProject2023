

/* add necessary includes */

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Not finished; Might be due for major (possibly delete later) */
int create_packets(char *eth_dest, char *ip_dest, uint8_t ip_protocol,
                   uint8_t payload, unsigned int payload_size) {
  /* Getting MAC and IP address for source */
  char *eth_host =
      get_mac_address("eth0"); /* Change name of interface depending on fit */
  char *ip_host =
      get_ip_address("eth0"); /* Change name of interface depending on fit */

  uint8_t packet_size = 0;
  /* Checking if the new packet requires TCP header */
  if (ip_protocol == ip_protocol_tcp) {
    /* Setting the new packet size */
    packet_size =
        (uint8_t)(payload_size + sizeof(tcp_hdr_t) + sizeof(gre_hdr_t) +
                  sizeof(ip_hdr_t) + sizeof(ethernet_hdr_t));
    /* Checking if the ne packet requires ICMP header */
  } else if (ip_protocol == ip_protocol_icmp) {
    /* Setting the new packet size */
    packet_size =
        (uint8_t)(payload_size + sizeof(icmp_hdr_t) + sizeof(gre_hdr_t) +
                  sizeof(ip_hdr_t) + sizeof(ethernet_hdr_t));
    /* Invalid packet type. Return -1 */
  } else {
    fprintf(stderr, "-------Invalid ip protocol code of %u -----\n",
            (unsigned int)ip_protocol);
    return -1;
  }

  /* Allocating memory for a new packet and setting it up */
  uint8_t new_packet = (uint8_t)malloc(packet_len);
  memset(new_packet, 0, sizeof(uint8_t) * (packet_size));

  /* Setting ethernet header info */
  ethernet_hdr_t *new_eth = (ethernet_hdr_t *)new_packet;
  memcpy(new_eth->ether_dhost, eth_dest, sizeof(uint8_t) * ETHER_ADDR_LEN);
  memcpy(new_eth->ether_shost, eth_host, sizeof(uint8_t) * ETHER_ADDR_LEN);
  new_eth->ether_type = htons(ethertype_ipv4);

  /* Encapsulating the packet with GRE */
  packet_encapsulate(new_packet);

  /* Setting IP header info. */
  ip_hdr_t *new_ip =
      (ip_hdr_t *)(new_packet + sizeof(ethernet_hdr_t) + sizeof(gre_hdr_t));
  new_ip->ip_v = 4;
  /* No need for ip_hl since the ip header size is fixed */
  /* No need for ip_tos since it's not used */
  new_ip->len = htons(packet_size - sizeof(ethernet_hdr_t));
  /* No need for ip_id, ip_off since fragmentation not used */
  new_ip->ip_p = ip_protocol;
  new_ip->ip_src = (uint32_t)strtoul(ip_host, NULL, 10);
  new_ip->ip_dst = (uint32_t)strtoul(ip_dest, NULL, 10);
  new_ip->ip_sum = 0;
  new_ip->ip_sum = cksum(new_ip, sizeof(sr_ip_hdr_t));

  /* Checking if new packet is TCP */
  if (ip_protocol == ip_protocol_tcp) {
    tcp_hdr_t new_tcp = (tcp_hdr_t *)(new_packet + sizeof(ethernet_hdr_t) +
                                      sizeof(gre_hdr_t) + sizeof(ip_hdr_t));
    new_tcp->src_port = PORT;
    new_tcp->dest_port = 80; /* PORTS might need to be changed later */
    /* Rest of TCP headers will need to be finished later */
    /* The new packet is ICMP header */
  } else {
    icmp_hdr_t new_icmp = (icmp_hdr_t *)(new_packet + sizeof(ethernet_hdr_t) +
                                         sizeof(gre_hdr_t) + sizeof(ip_hdr_t));
  }
}
