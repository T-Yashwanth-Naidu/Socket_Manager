#ifndef STATE_H
#define STATE_H

/* Module state structure */
typedef struct {
    char current_mode[32];
    int heartbeat;
    int uptime;
    int fault_active;
    int fault_code;
    int reset_state;
} module_state_t;

/* Initialize module state to default values */
void init_module_state(module_state_t *state);

#endif