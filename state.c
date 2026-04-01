#include <string.h>
#include "state.h"

/* Initialize module state to default values */
void init_module_state(module_state_t *state) {
    strcpy(state->current_mode, "IDLE");
    state->heartbeat = 10;
    state->uptime = 100;
    state->fault_active = 0;
    state->fault_code = 0;
    state->reset_state = 0;
}