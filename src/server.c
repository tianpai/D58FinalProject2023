/* Created by In Kim Nov 14, 2023
 * CSCD58 final project
 * Simple VPN
 */

/* Importing the libraries needed */

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "host_info.h"
#include "decap.h"
#include "encap.h"
#include "encrypt.h"
#include "packet.h"
#include "protocol.h"
#include "server.h"
#include "utils.h"

int create_server_socket() {
  int server_fd;
  struct sockaddr_in address;
  int opt = 1;

  /* Creating socket file descriptor */
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  /* Attacking socket */
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  /* Assigning the socket to IP and port */
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  /* Binding socket */
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  /* wait for incoming connections from clients */
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  return server_fd;
}

int accept_client_connection(int server_fd) {
  struct sockaddr_in address;
  int new_socket;
  int addrlen = sizeof(address);

  // Accepting the client connection
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  return new_socket;
}

int main(int argc, char const *argv[]) {
  if (argc != 3) {
    printf("Unexpected number of arguements. \
    Enter arguements: VPNserver_host_name");
    return 0;
  }
  const char *server_host_name = argv[1];

  /* get IP addresses of hosts */
  const char *server_ip = get_host_ip(server_host_name);
  if (server_ip == NULL) {
    printf("Invalid server name.\n");
    return 0;
  }

  int server_fd = create_server_socket();

  int new_socket = accept_client_connection(server_fd);

  uint8_t *rec_packet = NULL;
  rec_packet = serv_rec_from_cli(new_socket);
  if (rec_packet == NULL) {
    fprintf(stderr, "Error occured when rec packet from client via socket.\n");
    return 0;
  }

  print_packet(rec_packet);
  uint32_t client_ip = 0;
  save_client_ip(&client_ip, rec_packet);
  uint8_t *fixed_pkt = serv_handle_pkt(rec_packet, server_ip);
  printf("---------------------------------------\n---Testing for server-handled packet:\n");
  print_packet(fixed_pkt);
  free(rec_packet);

  /* closing the connected socket */
  close(new_socket);
  /* Shutting down the server */
  shutdown(server_fd, SHUT_RDWR);

  return 0;
}
