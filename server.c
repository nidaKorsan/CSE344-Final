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
    
    int temp, ret; 
    double totalTime;
    printf("Hello %d\n", _SC_OPEN_MAX);
    //Do the read ma man
    mainArgsServer margs;
    if(readArgumentsServer(argc, argv, &margs)){
        return -1;
    }
    if(daemonBorn(&margs)){
        return -1;
    }
    if(readFromFile(margs.fin, 0, NULL, &temp, NULL));
    graph_t *graph = createGraph(++temp);
    if(readFromFile(margs.fin, 1, graph, &temp, &totalTime)){
        return -1;
    }
    printServerInfo(margs, *graph, totalTime);
    if((ret = initSocket(margs.port)) == -1){
        return -1;
    }
    //printGraph(graph);
    //loop for threads
    while(1){

    }
    char *retBfs = bfsSearch(graph, 0, 158);
    printf("Path is %s\n",retBfs);
    free(retBfs);
    destroyGraph(graph);
    return 0;
}