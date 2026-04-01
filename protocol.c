#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "protocol.h"

static int is_valid_mode(const char *mode) {
  return (strcmp(mode, "IDLE") == 0 || strcmp(mode, "ACTIVE") == 0 ||
          strcmp(mode, "SERVICE") == 0 || strcmp(mode, "SAFE") == 0);
}

void handle_read_status(char *response, size_t response_size,
                        module_state_t *state) {
  snprintf(response, response_size,
           "OK:READ_STATUS:module_id=%d,mode=%s,heartbeat=%d,uptime=%d,fault_"
           "active=%d\n",
           MODULE_ID, state->current_mode, state->heartbeat, state->uptime,
           state->fault_active);
}

void handle_read_fault(char *response, size_t response_size,
                       module_state_t *state) {
  snprintf(response, response_size,
           "OK:READ_FAULT:module_id=%d,fault_active=%d,fault_code=%d\n",
           MODULE_ID, state->fault_active, state->fault_code);
}

void handle_set_mode(const char *request, char *response, size_t response_size,
                     module_state_t *state) {

  const char *mode_value = request + 9;

  if (is_valid_mode(mode_value)) {
    strncpy(state->current_mode, mode_value, sizeof(state->current_mode) - 1);
    state->current_mode[sizeof(state->current_mode) - 1] = '\0';

    snprintf(response, response_size, "OK:SET_MODE:mode=%s\n",
             state->current_mode);
  } else {
    snprintf(response, response_size, "ERR:SET_MODE:invalid_mode\n");
  }
}

void handle_inject_fault(const char *request, char *response,
                         size_t response_size, module_state_t *state) {

  const char *code_str = request + 13; // "INJECT_FAULT:" = 13 chars

  int code = atoi(code_str);

  if (code <= 0) {
    snprintf(response, response_size, "ERR:INJECT_FAULT:invalid_fault_code\n");
    return;
  }

  state->fault_active = 1;
  state->fault_code = code;

  snprintf(response, response_size,
           "OK:INJECT_FAULT:fault_active=1,fault_code=%d\n", code);
}

void handle_clear_fault(char *response, size_t response_size,module_state_t *state) {

  state->fault_active = 0;
  state->fault_code = 0;

  snprintf(response, response_size,
           "OK:CLEAR_FAULT:fault_active=0,fault_code=0\n");
}

void handle_reset(char *response,size_t response_size, module_state_t *state) {

  strcpy(state->current_mode, "IDLE");
  state->fault_active = 0;
  state->fault_code = 0;
  state->heartbeat=0;
  state->uptime=0;

  snprintf(response, response_size,
           "OK:RESET_MODULE:module_id=%d,mode=%s,heartbeat=%d,uptime=%d,fault_active=%d\n",
           MODULE_ID, state->current_mode, state->heartbeat, state->uptime,
           state->fault_active);
}

void build_response(const char *request, char *response, size_t response_size,
                    module_state_t *state) {

  if (strcmp(request, "READ_STATUS") == 0) {

    handle_read_status(response, response_size, state);

  } else if (strcmp(request, "READ_FAULT") == 0) {

    handle_read_fault(response, response_size, state);

  }

  else if (strncmp(request, "SET_MODE:", 9) == 0) {
    handle_set_mode(request, response, response_size, state);

  } else if (strncmp(request, "INJECT_FAULT:", 13) == 0) {
    handle_inject_fault(request, response, response_size, state);
  }

else if (strncmp(request, "CLEAR_FAULT", 12) == 0) {
  handle_clear_fault(response, response_size, state);
  }

  else if (strcmp(request, "RESET_MODULE") == 0) {
    handle_reset(response, response_size, state);
  }

  else {
    snprintf(response, response_size, "ERR:UNKNOWN_COMMAND\n");
  }
}
