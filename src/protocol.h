/* Created by Tianpai Zhang
 * Date: Nov 13, 2023
 */

/* Every function or struct should be defined within
 * the namespace of protocol.
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* write anything after this line */

#ifdef _LINUX_
#include <stdint.h>
#endif /* _LINUX_ */

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#ifndef IP_MAXPACKET
#define IP_MAXPACKET 65535
#endif

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1
#endif

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 2
#endif

#ifndef PORT
#define PORT 17350 /* Port number for server */
#endif

/* NEED TO CHANGE THE SERV_IP_ADDRESS ACCORDING TO SERVER IP ADDRESS */
#ifndef SERV_IP_ADDRESS
#define SERV_IP_ADDRESS "127.0.0.1"
#endif

/* NEED TO CHANGE THE SERV_ETH_ADDRESS ACCORDING TO SERVER IP ADDRESS */
#ifndef SERV_ETH_ADDRESS
#define SERV_ETH_ADDRESS
#endif

#ifndef __BYTE_ORDER
#ifdef _CYGWIN_
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif
#ifdef _LINUX_
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif
#ifdef _SOLARIS_
#define __BYTE_ORDER __BIG_ENDIAN
#endif
#ifdef _DARWIN_
#define __BYTE_ORDER __BIG_ENDIAN
#endif
#endif
#define ICMP_DATA_SIZE 28

/* Structure of a TCP header, stripped down to its most basic use */
struct tcp_hdr {
  uint16_t src_port;
  uint16_t dst_port;
  uint32_t seq_num;
  uint32_t ack_num;
  unsigned int data_offset : 4;
  uint8_t flags;
  uint16_t window;
  uint16_t tcp_sum;
  uint16_t urgent_pointer;
} __attribute__((packed));
typedef struct tcp_hdr tcp_hdr_t;

/* Structure of a generic ICMP header */
struct icmp_hdr {
  uint8_t icmp_type;
  uint8_t icmp_code;
  uint16_t icmp_sum;
  uint32_t unused;
  uint8_t data[ICMP_DATA_SIZE];
} __attribute__((packed));
typedef struct icmp_hdr icmp_hdr_t;

/*
 * Structure of an internet header, naked of options.
 */
struct ip_hdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  unsigned int ip_hl : 4; /* header length */
  unsigned int ip_v : 4;  /* version */
#elif __BYTE_ORDER == __BIG_ENDIAN
  unsigned int ip_v : 4;  /* version */
  unsigned int ip_hl : 4; /* header length */
#else
#error "Byte ordering not specified "
#endif
  uint8_t ip_tos;          /* type of service */
  uint16_t ip_len;         /* total length*/
  uint16_t ip_id;          /* identification */
  uint16_t ip_off;         /* fragment offest field */
  uint8_t ip_ttl;          /* time to live */
  uint8_t ip_p;            /* protocol */
  uint16_t ip_sum;         /* checksum */
  uint32_t ip_src, ip_dst; /* source and dest. */
} __attribute__((packed));
typedef struct ip_hdr ip_hdr_t;

/*
 *  Ethernet packet header prototype.
 */
struct ethernet_hdr {
#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6
#endif
  uint8_t ether_dhost[ETHER_ADDR_LEN]; /* destination ethernet address */
  uint8_t ether_shost[ETHER_ADDR_LEN]; /* source ethernet address */
  uint16_t ether_type;                 /* packet type ID */
} __attribute__((packed));
typedef struct ethernet_hdr ethernet_hdr_t;

/* Using standard GRE packet header; made C field from 1-bit to 16 bit,
 * removed reserved 0s and version field as they are 15 bits altogether
 * and is always set to 0, according to the standard. Decided to put
 * the 15 bit into good use and made C from unsigned type to uint16_t.
 * Also removed reserved 1 as it is not used for our purpose.
 */
struct gre_hdr {
  uint16_t c;        /* Checking if checksum used */
  uint16_t protocol; /* Ether protocol type */
  uint16_t checksum; /* Checksum*/
} __attribute__((packed));
typedef struct gre_hdr gre_hdr_t;

enum ip_protocol {
  ip_protocol_icmp = 0x0001,
  ip_protocol_tcp = 0x0006,
};

enum ethertype_code {
  ethertype_arp = 0x0806,
  ethertype_ipv4 = 0x0800,
  ethertype_ipv6 = 0x86DD,
};

/* anything should before this line */
#endif /* -- PROTOCOL_H -- */
