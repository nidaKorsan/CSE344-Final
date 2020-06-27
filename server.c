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
#define CLOSEF(f) {if (close(f) == -1) {printf("Error while closing the file.\n");}}

int main(int argc, char *argv[]){
    //TODO uncomment this beauty
    /*myfd = open("/tmp/MyUniqueName", O_CREAT|O_EXCL);
    if ( myfd < 0 )
    {
        write(2, "I am already running!\n", 23);
        exit(1);
    }*/
    time(&ltime);
    memset(&shared, 0, sizeof(sharedAmong_t));
    int temp, lfd, ret, cfd;
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
    if(readFromFile(margs.fin, 0, NULL, &temp, NULL)){
        return -1;
    }
    graph_t *graph = createGraph(++temp);
    if(readFromFile(margs.fin, 1, graph, &temp, &totalTime)){
        return -1;
    }
    CLOSEF(margs.fin);
    printServerInfo(margs, *graph, totalTime);
    if((lfd = initSocket(margs.port)) == -1){
        return -1;//TODO CLOSECALLS
    }
    //initiliaze thread id array and index of it
    shared.idIndex = 0;
    shared.socketId = -1;
    shared.thread_id = malloc(sizeof(pthread_t) * (margs.threadNum + 1));//+1 because there's also a pooler thread
    shared.graph = graph;
    pthread_mutex_init(&shared.assignWorkMutex,NULL);
    pthread_mutex_init(&shared.loadFactorMutex,NULL);
    pthread_cond_init(&shared.assignWorkCond, NULL);
    pthread_cond_init(&shared.noWorkCond, NULL);
    pthread_cond_init(&shared.loadFactorCond, NULL);

    if((ret = pthread_create(&shared.thread_id[shared.idIndex++], NULL, poolerThreadAct, &margs)) != 0){
        printf("Error pthread_create %s\n", strerror(ret));
        raise(SIGINT);
    }
    //close(lfd);
 
    //printGraph(graph);
    //loop for threads
    printf("Now going in big loop...\n");
    while(1){
        cfd = accept(lfd, NULL, NULL);
        if(cfd == -1) {
            printf("Error accept() %s\n", strerror(errno));
            //continue;
        }
        //assign the new socket id to a suaitable thread
        MLOCK_INT(&shared.assignWorkMutex);
        while(shared.socketId != -1)
            pthread_cond_wait(&shared.noWorkCond, &shared.assignWorkMutex);
        printf("giving to function...\n");
        shared.socketId = cfd;
        pthread_cond_signal(&shared.assignWorkCond);
        MUNLOCK_INT(&shared.assignWorkMutex);

    }
    if((ret = pthread_join(shared.thread_id[0], NULL)) != 0){
        printf("Error pthread_join %s\n", strerror(ret));
        raise(SIGINT);
    }
    destroyGraph(graph);
    return 0;
}