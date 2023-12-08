#include "host_info.h"
#include <stdlib.h>
#include <string.h>

HostInfo hosts[] = {
    {"h1", H1_IP, H1_MAC}, {"h2", H2_IP, H2_MAC}, {"h3", H3_IP, H3_MAC},
    {"h4", H4_IP, H4_MAC}, {"h5", H5_IP, H5_MAC}, {"h6", H6_IP, H6_MAC},
    {"h7", H7_IP, H7_MAC}, {NULL, NULL, NULL},
};

const char *get_host_ip(char *host_name) {
  int i = 0;
  while (hosts[i].name != NULL) {
    if (strcmp(hosts[i].name, host_name) == 0) {
      return hosts[i].ip;
    }
    i++;
  }
  return NULL;
}

const char *get_host_mac(char *host_name) {
  int i = 0;
  while (hosts[i].name != NULL) {
    if (strcmp(hosts[i].name, host_name) == 0) {
      return hosts[i].mac;
    }
    i++;
  }
  return NULL;
}
