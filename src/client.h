/* Created by In Kim Nov 14, 2023
 * CSCD58 final project
 * Simple VPN
 */

#ifndef CLIENT_H
#define CLIENT_H

#define PORT 17350          

#define LOCALHOST "127.0.0.1"

/* Following functions are used to create the client socket as well
 * as connecting to server
 */
int create_client_socket();
int connect_to_server(int client_fd);

#endif /* -- CLIENT_H -- */
