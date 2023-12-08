#ifndef HOST_INFO_H
#define HOST_INFO_H

#include "protocol.h"
#include <stdint.h>

#define H1_IP "10.0.0.1"
#define H1_MAC "36:16:d6:40:1a:34"
#define H2_IP "10.0.0.2"
#define H2_MAC "b2:4b:96:10:9d:42"
#define H3_IP "10.0.0.3"
#define H3_MAC "ce:1e:82:45:b5:a6"
#define H4_IP "10.0.0.4"
#define H4_MAC "f6:ba:3a:3e:94:8a"
#define H5_IP "10.0.0.5"
#define H5_MAC "7e:3d:80:0c:e2:2a"
#define H6_IP "10.0.0.6"
#define H6_MAC "8a:fb:2b:2e:41:1c"
#define H7_IP "10.0.0.7"
#define H7_MAC "26:15:a3:cd:fd:8a"

typedef struct {
  const char *name;
  const char *ip;
  const char *mac;
} HostInfo;

void parse_mac_addr(uint8_t eth[ETHER_ADDR_LEN], const char *mac_addr);
void parse_ip_addr(uint32_t *ip, const char *ip_addr);

const char *get_host_ip(const char *host_name);
const char *get_host_mac(const char *host_name);

#endif // HOST_INFO_H
