CC = gcc #compiler
objects = server client
all : $(objects)
server:
	$(CC) -o server -Wall server.c recipes.c myGraph.c -pthread -lrt
client:
	$(CC) -o client -Wall client.c recipes.c myGraph.c -pthread -lrt
clean:
	rm $(objects)