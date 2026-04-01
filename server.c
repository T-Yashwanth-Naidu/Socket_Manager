#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "server.h"

int create_server_socket(void) {
    int server_fd;
    int listen_status;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) != 1) {
        perror("inet_pton failed");
        close(server_fd);
        return -1;
    }

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        return -1;
    }

    listen_status = listen(server_fd, BACKLOG);
    if (listen_status == -1) {
        perror("listen failed");
        close(server_fd);
        return -1;
    }

    printf("Server listening on 127.0.0.1:%d\n", SERVER_PORT);
    return server_fd;
}

int accept_client(int server_fd) {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1) {
        perror("accept failed");
        return -1;
    }

    printf("Client connected, fd=%d\n", client_fd);
    return client_fd;
}

ssize_t receive_message(int client_fd, char *buff, size_t buff_size) {
    ssize_t recv_status;

    recv_status = recv(client_fd, buff, buff_size - 1, 0);
    if (recv_status == -1) {
        perror("recv failed");
        return -1;
    }

    if (recv_status == 0) {
        return 0;
    }

    buff[recv_status] = '\0';

    while (recv_status > 0 &&
           (buff[recv_status - 1] == '\n' || buff[recv_status - 1] == '\r')) {
        buff[recv_status - 1] = '\0';
        recv_status--;
    }

    return recv_status;
}

ssize_t send_message(int client_fd, const char *response) {
    ssize_t send_status;

    send_status = send(client_fd, response, strlen(response), 0);
    if (send_status == -1) {
        perror("send failed");
    }

    return send_status;
}