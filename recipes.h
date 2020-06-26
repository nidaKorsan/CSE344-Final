/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>
#include<string.h>
#include <semaphore.h>
#include <sys/stat.h>
#include<sys/types.h>
#include <sys/mman.h>
#include<signal.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <syslog.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "myGraph.h"

#define UNLOCK(f) { lock.l_type = F_UNLCK;\
            if (fcntl(f, F_SETLKW, &lock) == -1){\
                printf("Cannot unlock the file.\n");} }
#define LOCK(f) { lock.l_type = F_WRLCK;\
            if (fcntl(f, F_SETLKW, &lock) == -1){\
                printf("Cannot lock the file.\n");} }
#define BACKLOG 50
#define PORT "44444"

typedef struct{
    int fin;
    int fout;
    char *inputPath;
    char *outputPath;
    char* port;
    int threadNum;
    int maxThreadNum;
}mainArgs;


int callSigAction();
void handler(int signalNumber);
void exitGracefully();
int daemonBorn();
int readArgumentsServer(int argc, char *argv[], mainArgs *margs);
int readFromFile(int fin, int choice, graph_t *graph, int *maxNum);//if choice is 0, find the max node num; if 1, add edges to graph
int initSocket(char* portNum);
