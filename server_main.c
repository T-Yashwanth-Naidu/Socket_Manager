#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "protocol.h"
#include "server.h"
#include "state.h"

int main(void) {
  int server_fd;
  int client_fd;
  ssize_t recv_status;
  ssize_t send_status;

  /* Tracks last second when heartbeat was incremented */
  static time_t last_hb_time = 0;

  /* Reference time used to compute module uptime */
  time_t module_start_time = time(NULL);

  char request_buffer[BUFFER_SIZE];
  char response_buffer[RESPONSE_SIZE];
  module_state_t state;

  /* Initialize simulated module state */
  init_module_state(&state);
  last_hb_time = module_start_time;

  /* Create, bind, and listen on the TCP server socket */
  server_fd = create_server_socket();
  if (server_fd == -1) {
    return EXIT_FAILURE;
  }

  /* Keep server alive and accept clients one after another */
  while (1) {
    client_fd = accept_client(server_fd);
    if (client_fd < 0) {
      continue;
    }

    /* Serve requests from the currently connected client */
    while (1) {
      time_t now = time(NULL);

      /* Update time-based simulated module state */
      state.uptime = (int)(now - module_start_time);

      if (now != last_hb_time) {
        state.heartbeat++;
        last_hb_time = now;
      }

      /* Read one request from the connected client */
      recv_status =
          receive_message(client_fd, request_buffer, sizeof(request_buffer));
      if (recv_status == -1) {
        close(client_fd);
        close(server_fd);
        return EXIT_FAILURE;
      }

      /* Exit client session loop if client closes connection */
      if (recv_status == 0) {
        printf("Client disconnected\n");
        break;
      }

      printf("Received: %s\n", request_buffer);

      /* Parse request, update module state if needed, and build response */
      build_response(request_buffer, response_buffer, sizeof(response_buffer),
                     &state);

      /* If RESET_MODULE was requested, restart timing from current moment */
      if (state.reset_state) {
        now = time(NULL);
        module_start_time = now;
        last_hb_time = now;
        state.reset_state = 0;
      }

      /* Send protocol response back to client */
      send_status = send_message(client_fd, response_buffer);
      if (send_status == -1) {
        close(client_fd);
        close(server_fd);
        return EXIT_FAILURE;
      }

      printf("Sent: %s", response_buffer);
    }

    close(client_fd);
  }

  close(server_fd);
  return EXIT_SUCCESS;
}