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

typedef struct{
    int fin;
    int fout;
    char *inputPath;
    char *outputPath;
    int port;
    int threadNum;
    int maxThreadNum;
}mainArgs;

typedef struct{
    int dest;//destination node
    int src;//source node
}edge_t;//src->dest

typedef struct node{
    int vertex;
    struct node *next;
}node_t;

typedef struct{
    int numVertice;//number of vertices
    int numEdge;//number of edges
    node_t **adjLists;//unknown number of vertices, adjaceny lists list
}graph_t;

int callSigAction();
void handler(int signalNumber);
void exitGracefully();
int daemonBorn();
int readArguments(int argc, char *argv[], mainArgs *margs);
node_t* createNode(int v);
graph_t* createAGraph(int vertices);
void addEdge(graph_t* graph, edge_t newEdge);
void destroyGraph(graph_t *g);