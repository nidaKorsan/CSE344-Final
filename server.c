/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/

//TODO
//Daemon creation
//Client creation
//Port connection etc.
//Design general system structure

#include "recipes.h"

int main(int argc, char *argv[]){
    
    printf("Hello %d\n", _SC_OPEN_MAX);
    //Do the read ma man
    mainArgs margs;
    if(readArgumentsServer(argc, argv, &margs)){
        return -1;
    }
    if(daemonBorn(&margs)){
        return -1;
    }
    int temp;
    readFromFile(margs.fin, 0, NULL, &temp);
    printf("my max num is %d\n", temp);
    graph_t *graph = createGraph(++temp);
    readFromFile(margs.fin, 1, graph, &temp);
    printGraph(graph);
    initSocket(margs.port);
    destroyGraph(graph);
    return 0;
}