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

/**
 * Parse IP address from string to uint32_t
 * @param ip: pointer to the uint32_t variable to store the IP address
 * @param ip_addr: string of the IP address
 *  @NOTE:  this input ip is not the ip struct, but
 *          a pointer to ip_src or ip_dst in the ip struct
 */
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

/**
 * Parse IP address from string to uint32_t
 * @param ip: pointer to the uint32_t variable to store the IP address
 * @param ip_addr: string of the IP address
 *  @NOTE:  this input ip is not the ip struct, but
 *          a pointer to ip_src or ip_dst in the ip struct
 */
void parse_ip_addr_to_str(char *str_ip_addr, uint32_t ip_addr) {
  sprintf(str_ip_addr, "%d.%d.%d.%d", 
          (ip_addr >> 24), (ip_addr >> 16) % 256,
          (ip_addr >> 8) % 256, ip_addr % 256
         );
}

/**
 * @param host_name: the name of the host
 * @return host_ip: the IP address of the host
 * @NOTE: used to pass IP address to create_packet() function
 */
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