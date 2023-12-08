#include "host_info.h"
#include "protocol.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HostInfo hosts[] = {
    {"h1", H1_IP, H1_MAC}, {"h2", H2_IP, H2_MAC}, {"h3", H3_IP, H3_MAC},
    {"h4", H4_IP, H4_MAC}, {"h5", H5_IP, H5_MAC}, {"h6", H6_IP, H6_MAC},
    {"h7", H7_IP, H7_MAC}, {NULL, NULL, NULL},
};

void parse_mac_addr(uint8_t eth[ETHER_ADDR_LEN], const char *mac_addr) {
  for (int i = 0; i < ETHER_ADDR_LEN; i++) {
    unsigned int byte;
    sscanf(mac_addr, "%2x", &byte);
    eth[i] = (uint8_t)byte;
  }
}

void parse_ip_addr(uint32_t *ip, const char *ip_addr) {
  unsigned int bytes[4];
  if (sscanf(ip_addr, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2],
             &bytes[3]) == 4) {
    *ip = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
  } else {
    fprintf(stderr, "Error: when parsing IP address of hosts");
    fprintf(stderr, "Invalid IP address: %s\n", ip_addr);
  }
}

const char *get_host_ip(const char *host_name) {
  int i = 0;
  while (hosts[i].name != NULL) {
    if (strcmp(hosts[i].name, host_name) == 0) {
      return hosts[i].ip;
    }
    i++;
  }
  return NULL;
}

const char *get_host_mac(const char *host_name) {
  int i = 0;
  while (hosts[i].name != NULL) {
    if (strcmp(hosts[i].name, host_name) == 0) {
      return hosts[i].mac;
    }
    i++;
  }
  return NULL;
}
