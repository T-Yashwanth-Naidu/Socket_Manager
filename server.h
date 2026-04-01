#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>
#include <sys/types.h>

/* Create TCP server socket, bind to localhost, and start listening */
int create_server_socket(void);

/* Accept one incoming client connection */
int accept_client(int server_fd);

/* Receive one client request into buffer and trim trailing newline */
ssize_t receive_message(int client_fd, char *buff, size_t buff_size);

/* Send one response string to the connected client */
ssize_t send_message(int client_fd, const char *response);

#endif