/* Created by In Kim Nov 14, 2023
 * CSCD58 final project
 * Simple VPN
 */

/* Importing the libraries needed */
#include "client.h"
#include "protocol.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int create_client_socket()
{
  int client_fd;

  /* Creating a socket */
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }

  return client_fd;
}

int connect_to_server(int client_fd)
{
  struct sockaddr_in serv_addr;

  /* Setting up server address */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  /* Convert IPv4 address from text to binary form */
  // SERV_ADDRESS is defined? did u mean SERV_IP_ADDRESS?
  if (inet_pton(AF_INET, SERV_IP_ADDRESS, &serv_addr.sin_addr) <= 0)
  {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  /* Connect to server */
  if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0)
  {
    printf("\nConnection Failed \n");
    return -1;
  }

  return 0;
}

int main(int argc, char const *argv[])
{
  /* Create client socket */
  int client_fd = create_client_socket();

  if (client_fd == -1)
  {
    return -1;
  }

  /* Connect client to server */
  if (connect_to_server(client_fd) == -1)
  {
    close(client_fd);
    return -1;
  }

  /* Perform operations with the client socket as needed */

  /* Close the client socket */
  close(client_fd);
  return 0;
}
