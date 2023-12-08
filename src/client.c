/* Created by In Kim Nov 14, 2023
 * CSCD58 final project
 * Simple VPN
 */

/* Importing the libraries needed */
#include "client.h"
#include "host_info.h"
#include "packet.h"
#include "protocol.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>       // TODO: remove - used for testing
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int create_client_socket() {
  int client_fd;

  /* Creating a socket */
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  return client_fd;
}

int connect_to_server(int client_fd) {
  struct sockaddr_in serv_addr;

  /* Setting up server address */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  /* Convert IPv4 address from text to binary form */
  if (inet_pton(AF_INET, SERV_IP_ADDRESS, &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  /* Connect to server */
  if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    printf("\nConnection Failed \n");
    return -1;
  }
  printf("Connected to server\n");

  return 0;
}

/**
 * commandline arguements:
 * - client_host_name: the client's host name on Mininet
 * - server_host_name: the VPN server's host name on Mininet
 */
int main(int argc, char const *argv[]) {
  /* read commandline arguements */
  if (argc != 3) {
    printf("Unexpected number of arguements. \
    Enter arguements: client_host_name VPNserver_host_name");
  }
  const char *client_host_name = argv[1];
  const char *server_host_name = argv[2];

  /* get IP and MAC addresses of hosts */
  const char *client_ip = get_host_ip(client_host_name);
  const char *client_mac = get_host_mac(client_host_name);
  const char *server_ip = get_host_ip(server_host_name);
  const char *server_mac = get_host_mac(server_host_name);

  /* Test IP & MAC address parsing */
  uint8_t mac_addr[] = {0, 0, 0, 0, 0, 0};
  uint32_t ip_addr = 0;
  parse_mac_addr(mac_addr, "36:16:d6:40:1a:34");
  parse_ip_addr(&ip_addr, "10.0.0.1");

  char *mac_addr_str = malloc(3*ETHER_ADDR_LEN*sizeof(char));
  char *ip_addr_str = malloc(3*ETHER_ADDR_LEN*sizeof(char));
  parse_mac_addr_to_str(mac_addr_str, mac_addr);
  parse_ip_addr_to_str(ip_addr_str, ip_addr);

  printf("------------------------------------\n");
  printf("[ parse testing ]\n");

  for(int i = 0; i < ETHER_ADDR_LEN; i++) {
    printf("%d, ", mac_addr[i]);
  }
  printf("%s\n", mac_addr_str);

  printf("%d ", ip_addr);
  printf("%s\n", ip_addr_str);

  free(ip_addr_str);
  free(mac_addr_str);

  /* Create client socket */
  // int client_fd = create_client_socket();

  // if (client_fd == -1) {
  //   return -1;
  // }

  /* Connect client to server */
  // if (connect_to_server(client_fd) == -1) {
  //   close(client_fd);
  //   return -1;
  // }


  uint8_t *packet =
      create_packets(client_mac, client_ip, server_mac, server_ip,
                     ip_protocol_tcp, "payload", tcp_flag_syn);

  // for(int i = 0; i < 68; i++) {
  //   printf("%s ", packet + i);
  // }

  // printf("packet size: %d\n", get_packet_size(ip_protocol_tcp,
  // strlen("payload")));

  print_packet(packet);

  /* Send packet to server */
  // send_packet_vpn()

  /* Close the client socket */
  // close(client_fd);
  return 0;
}
