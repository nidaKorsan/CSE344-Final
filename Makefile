CC = gcc #compiler
objects = server client
all : $(objects)
server:
	$(CC) -o server -Wall server.c recipes.c -pthread -lrt
client:
	$(CC) -o client -Wall client.c -pthread -lrt
clean:
	rm $(objects)