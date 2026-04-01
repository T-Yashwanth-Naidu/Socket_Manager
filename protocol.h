#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>
#include "state.h"

/* Parse one client request, update module state if needed, and build response */
void build_response(const char *request, char *response, size_t response_size,
                    module_state_t *state);

#endif