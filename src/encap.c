/* Created by In Kim Nov 23, 2023
 * CSCD58 final project
 * Simple VPN
 */ /* Importing the libraries needed */
#include <sys/ioctl.h>
#include <sys/socket.h>

#ifdef __linux__
#include <linux/if.h>
#elif __APPLE__
#include <net/if.h>
#endif

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "encap.h"
#include "protocol.h"
#include "utils.h"

void packet_encapsulate(uint8_t *packet) {
  /* Type casting to access GRE header */
  gre_hdr_t *enc_gre_hdr = (gre_hdr_t *)(packet + sizeof(ethernet_hdr_t));

  /* Assigning values to GRE */
  enc_gre_hdr->c = 1;
  /* Assuming we only use IPv4 */
  enc_gre_hdr->protocol = htons(ethertype_ipv4);
  enc_gre_hdr->gre_sum = 0;
  enc_gre_hdr->gre_sum = cksum(enc_gre_hdr, sizeof(struct gre_hdr));
}

/* Erase this in future if this doesn't work with mininet*/
char *get_ip_address(const char *interface) {
  int n;
  struct ifreq ifr;
  char *ip_str = NULL;

  n = socket(AF_INET, SOCK_DGRAM, 0);
  if (n == -1) {
    perror("Socket creation failed");
    return NULL;
  }

  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);

  if (ioctl(n, SIOCGIFADDR, &ifr) == -1) {
    perror("IOCTL failed");
    close(n);
    return NULL;
  }

  close(n);

  ip_str = strdup(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
  if (ip_str == NULL) {
    perror("Strdup failed");
    return NULL;
  }

  return ip_str;
}

/* Erase this in future if this doesn't work with mininet */
char *get_mac_address(const char *interface) {
  struct ifreq s;
  int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
  char *mac_str = NULL;

  if (fd == -1) {
    perror("Socket creation failed");
    return NULL;
  }

  strncpy(s.ifr_name, interface, sizeof(s.ifr_name));
  if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
    mac_str = malloc(6); // MAC address length (17) + null terminator (1)
    if (mac_str == NULL) {
      perror("Malloc failed");
      close(fd);
      return NULL;
    }

    sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            (unsigned char)s.ifr_addr.sa_data[0],
            (unsigned char)s.ifr_addr.sa_data[1],
            (unsigned char)s.ifr_addr.sa_data[2],
            (unsigned char)s.ifr_addr.sa_data[3],
            (unsigned char)s.ifr_addr.sa_data[4],
            (unsigned char)s.ifr_addr.sa_data[5]);
  } else {
    perror("IOCTL failed");
    close(fd);
    return NULL;
  }

  close(fd);
  return mac_str;
}
