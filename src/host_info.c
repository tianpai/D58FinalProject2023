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
#define HOST_BUFFER_SIZE 1025

/** @brief Get the network information of the interface
 *  @param iface: the name of the interface
 *  @param ipAddr: the IP address of the interface
 *
 *  @NOTE Read the IP address of the interface in the form of string
 * and assign it to ipAddr
 */
void read_ip_from_iface(const char *iface, char *ipAddr) {
  struct ifaddrs *ifaddr, *ifa;
  int family, s;
  char host[HOST_BUFFER_SIZE];

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    exit(EXIT_FAILURE);
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;

    family = ifa->ifa_addr->sa_family;

    if (strcmp(ifa->ifa_name, iface) == 0 &&
        (family == AF_INET || family == AF_INET6)) {
      s = getnameinfo(ifa->ifa_addr,
                      (family == AF_INET) ? sizeof(struct sockaddr_in)
                                          : sizeof(struct sockaddr_in6),
                      host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if (s != 0) {
        printf("getnameinfo() failed: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
      }
      strcpy(ipAddr, host);
      break;
    }
  }

  freeifaddrs(ifaddr);
}

/**
 * Parse IP address from string to uint32_t
 * @param ip: string of the IP address
 * @returns uint32_t of the IP address
 */

uint32_t parse_ip_addr(const char *ip) {
  uint32_t val = 0;
  int octet = 0;
  const char *start = ip;

  for (int i = 0; i < 4; i++) {
    while (*start != '.' && *start != '\0') {
      octet = octet * 10 + (*start++ - '0');
    }
    if (*start == '.') {
      start++;
    }
    val = (val << 8) | octet;
    octet = 0;
  }

  return val;
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