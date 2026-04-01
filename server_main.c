#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "common.h"
#include "protocol.h"
#include "server.h"
#include "state.h"

int main(void) {

  
  int server_fd;
  int client_fd;
  ssize_t recv_status;
  ssize_t send_status;
  static time_t last_hb_time=0;
  time_t start_time = time(NULL);

  char request_buffer[BUFFER_SIZE];
  char response_buffer[RESPONSE_SIZE];
  module_state_t state;

  init_module_state(&state);

  server_fd = create_server_socket();
  if (server_fd == -1) {
    return EXIT_FAILURE;
  }

  client_fd = accept_client(server_fd);
  if (client_fd == -1) {
    close(server_fd);
    return EXIT_FAILURE;
  }

  while (1) {
    time_t now = time(NULL);
    state.uptime = (int)(now-start_time);

    if (now != last_hb_time) {
      state.heartbeat++;
      last_hb_time = now;
    }

    recv_status =
        receive_message(client_fd, request_buffer, sizeof(request_buffer));
    if (recv_status == -1) {
      close(client_fd);
      close(server_fd);
      return EXIT_FAILURE;
    }

    if (recv_status <= 0)
      break;

    if (recv_status == 0) {
      printf("Client disconnected\n");
      break;
    }

    printf("Received: %s\n", request_buffer);

    build_response(request_buffer, response_buffer, sizeof(response_buffer),
                   &state);

    send_status = send_message(client_fd, response_buffer);
    if (send_status == -1) {
      close(client_fd);
      close(server_fd);
      return EXIT_FAILURE;
    }

    printf("Sent: %s", response_buffer);
  }

  close(client_fd);
  close(server_fd);
  return EXIT_SUCCESS;
}