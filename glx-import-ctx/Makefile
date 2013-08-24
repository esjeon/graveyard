
FLAGS := -Wall -ggdb -O0 $(shell pkg-config --libs --cflags gl x11)

all: server client

server: server.c
	clang $(FLAGS) server.c -o server

client: client.c
	clang $(FLAGS) client.c -o client

.PHONY: clean
clean:
	rm server
	rm client
