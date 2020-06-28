#**********************************************
# *  344 Sytem Programming FINAL              *
# *  Spring 2020                              *
# *  Author: Nida Korsan                      *
# *********************************************/

CC = gcc #compiler
objects = server client
all : $(objects)

%.o : %.c recipes.h dataStructures.h 
	$(CC) -c $< -Wall -o $@ -pthread -lrt

server: server.o recipes.o dataStructures.o 
	$(CC) $^ -o $@ -Wall -pthread -lrt
client: client.o recipes.o dataStructures.o 
	$(CC) $^ -o $@ -Wall -pthread -lrt

clean:
	rm  *.o