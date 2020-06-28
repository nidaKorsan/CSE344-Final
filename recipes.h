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
#include <sys/file.h>
#include<signal.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <syslog.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "dataStructures.h"

#define UNLOCK(f) { lock.l_type = F_UNLCK;\
            if (fcntl(f, F_SETLKW, &lock) == -1){\
                printf("Cannot unlock the file.\n");} }
#define LOCK(f) { lock.l_type = F_WRLCK;\
            if (fcntl(f, F_SETLKW, &lock) == -1){\
                printf("Cannot lock the file.\n");} }
#define MLOCK(mutex){if((ret = pthread_mutex_lock(mutex)) != 0){\
            printf("Error pthread_mutex_lock (florist) %s\n", strerror(ret));\
            return (void *)-1;}}
#define MUNLOCK(mutex){if((ret = pthread_mutex_unlock(mutex)) != 0){\
            printf("Error pthread_mutex_unlock (florist) %s\n", strerror(ret));\
            return (void *)-1;}}   

#define MLOCK_INT(mutex){if((ret = pthread_mutex_lock(mutex)) != 0){\
            printf("Error pthread_mutex_lock (florist) %s\n", strerror(ret));\
            return -1;}}
#define MUNLOCK_INT(mutex){if((ret = pthread_mutex_unlock(mutex)) != 0){\
            printf("Error pthread_mutex_unlock (florist) %s\n", strerror(ret));\
            return -1;}}   
#define BACKLOG 50

typedef struct{
    int fin;
    int fout;
    char *inputPath;
    char *outputPath;
    char *port;
    int threadNum;
    int maxThreadNum;
}mainArgsServer;

typedef struct{
    int src;
    int dest;
    char *ipAdress;
    char *port;
}mainArgsClient;

typedef struct{
    int id;
}daemonThreadArgs;

typedef struct{
    pthread_mutex_t assignWorkMutex, loadFactorMutex;
    pthread_cond_t assignWorkCond, noWorkCond, loadFactorCond;
    pthread_t *thread_id;
    int socketId;
    int idIndex;
    int busyTNum;
    int currentThreadCount;
    graph_t *graph;
    linkedList_t *cache;
}sharedAmong_t;

sharedAmong_t shared;
time_t ltime;
int myfd;


int callSigAction();
void handler(int signalNumber);
void exitGracefully();
int daemonBorn();
int readArgumentsServer(int argc, char *argv[], mainArgsServer *margs);
int readArgumentsClient(int argc, char *argv[], mainArgsClient *margs);
int readFromFile(int fin, int choice, graph_t *graph, int *maxNum, double *tot);//if choice is 0, find the max node num; if 1, add edges to graph
int initSocket(char* portNum);
void printServerInfo(mainArgsServer margs,graph_t graph, double totalTime);
int clientConnect(mainArgsClient *margs);
void* daemonThreadAct(void *arg);
void* poolerThreadAct(void *arg);