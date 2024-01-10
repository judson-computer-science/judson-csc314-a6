CC = gcc
CFLAGS = -Wall -g

client: calc_client.c
	$(CC) $(CFLAGS) -o client calc_client.c

server: calc_server.c
	$(CC) $(CFLAGS) -o server calc_server.c

clean:
	rm client server
