#include <arpa/inet.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "host_info.h"
#include "protocol.h"

void getNetworkInfo(const char *iface, char *ipAddr, char *macAddr);

HostInfo hosts[] = {
    {"h1", H1_IP, H1_MAC}, {"h2", H2_IP, H2_MAC}, {"h3", H3_IP, H3_MAC},
    {"h4", H4_IP, H4_MAC}, {"h5", H5_IP, H5_MAC}, {"h6", H6_IP, H6_MAC},
    {"h7", H7_IP, H7_MAC}, {NULL, NULL, NULL},
};

/**
 * Parse MAC address from string to uint8_t
 * @param eth: pointer to the uint8_t array to store the MAC address
 * @param mac_addr: string of the MAC address
 */
void parse_mac_addr(uint8_t eth[ETHER_ADDR_LEN], const char *mac_addr) {
  for (int i = 0; i < ETHER_ADDR_LEN; i++) {
    unsigned int byte;
    sscanf(mac_addr + (i * 3), "%2x", &byte);
    eth[i] = (uint8_t)byte;
  }
}

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
 * Parse MAC address from array of uint8_t to String
 * @param str_mac_addr: pointer to the string variable to store the MAC address
 * @param mac_addr: MAC address in the form: array of uint8_t
 */
void parse_mac_addr_to_str(char *str_mac_addr,
                           uint8_t mac_addr[ETHER_ADDR_LEN]) {
  sprintf(str_mac_addr, "%02x", mac_addr[0]);
  for (int i = 1; i < ETHER_ADDR_LEN; i++) {
    sprintf(str_mac_addr + (i * 3) - 1, ":%02x", mac_addr[i]);
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
  sprintf(str_ip_addr, "%d.%d.%d.%d", (ip_addr >> 24), (ip_addr >> 16) % 256,
          (ip_addr >> 8) % 256, ip_addr % 256);
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

/**
 * @param host_name: the name of the host
 * @return host_mac: the MAC address of the host
 * @NOTE: used to pass MAC address to create_packet() function
 */
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
