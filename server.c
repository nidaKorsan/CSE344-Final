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
    if(readArguments(argc, argv, &margs)){
        return -1;
    }
    if(daemonBorn(&margs)){
        return -1;
    }
    readFromFile(margs.fin);
    initSocket(margs.port);
    return 0;
}