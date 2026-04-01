#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>
#include <sys/types.h>

int create_server_socket(void);
int accept_client(int server_fd);
ssize_t receive_message(int client_fd, char *buff, size_t buff_size);
ssize_t send_message(int client_fd, const char *response);

#endif