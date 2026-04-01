#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>
#include "state.h"

void build_response(const char *request,char *response,size_t response_size,module_state_t *state);
void handle_read_status(char *response, size_t response_size, module_state_t *state);
void handle_read_fault(char *response, size_t response_size, module_state_t *state);
void handle_set_mode(const char *request, char *response, size_t response_size, module_state_t *state);
void handle_inject_fault(const char *request, char *response,size_t response_size, module_state_t *state);
void handle_reset(char *response, size_t response_size, module_state_t *state);
void handle_clear_fault(char *response, size_t response_size,module_state_t *state);

#endif