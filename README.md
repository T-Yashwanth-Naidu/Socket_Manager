Build a Socket Manager module in C on Linux that maintains multiple TCP connections, accepts requests from different internal modules, and sends them based on priority without letting one slow connection block the others.

Constraints:

- multiple concurrent TCP connections
- 3 priority levels
- thread-safe request submission
- reconnect on failure
- graceful shutdown
- handle partial send/receive
- avoid blocking design



Build: `make`  
Run: `make run`  
Clean: `make clean`  