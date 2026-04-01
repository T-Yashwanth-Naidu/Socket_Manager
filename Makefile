CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = diag_server

SRCS = server_main.c server.c protocol.c state.c
HDRS = server.h protocol.h state.h
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: server_main.c common.h server.h protocol.h state.h
	$(CC) $(CFLAGS) -c server_main.c

server.o: server.c common.h server.h
	$(CC) $(CFLAGS) -c server.c

protocol.o: protocol.c common.h protocol.h state.h
	$(CC) $(CFLAGS) -c protocol.c

state.o: state.c state.h
	$(CC) $(CFLAGS) -c state.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)