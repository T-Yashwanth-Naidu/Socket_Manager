#ifndef STATE_H
#define STATE_H

typedef struct {
    char current_mode[32];
    int heartbeat;
    int uptime;
    int fault_active;
    int fault_code;
} module_state_t;

void init_module_state(module_state_t *state);

#endif