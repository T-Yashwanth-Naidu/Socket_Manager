#ifndef COMMON_H
#define COMMON_H

/* TCP server port used by the diagnostics server */
#define SERVER_PORT 10001

/* Maximum size for one received client request */
#define BUFFER_SIZE 1024

/* Maximum size for one server response string */
#define RESPONSE_SIZE 256

/* Maximum number of pending connections in listen queue */
#define BACKLOG 5

/* Fixed identifier for the simulated diagnostics module */
#define MODULE_ID 1

#endif