CC = gcc #compiler
objects = server client
all : $(objects)

%.o : %.c recipes.h myGraph.h 
	$(CC) -c $< -Wall -o $@ -pthread -lrt

server: server.o recipes.o myGraph.o 
	$(CC) $^ -o $@ -Wall -pthread -lrt
client: client.o recipes.o myGraph.o 
	$(CC) $^ -o $@ -Wall -pthread -lrt
	
clean:
	rm  *.o