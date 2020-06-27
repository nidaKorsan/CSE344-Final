/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/
#include "recipes.h"

int main(int argc, char *argv[]){
    mainArgsClient margs;
    if(readArgumentsClient(argc, argv, &margs)){
        return -1;
    }

    int cfd;
    if((cfd = clientConnect(&margs)) == -1){
    printf("Im in client print\n");
        return -1;
    }

    /* Send requested sequence length, with terminating newline */
    char buffer[128];
    if (read(cfd, buffer, 30) == -1)
        printf("Partial/failed write (reqLenStr)\n");
    sprintf(buffer,"%d %d\n", margs.src, margs.dest);
    printf("Buffer is : %s\n", buffer);
    if (write(cfd, buffer, strlen(buffer)) == -1)
        printf("Client write to server failed\n");

    /* Read and display sequence number returned by server */

    close(cfd); 
    exit(EXIT_SUCCESS);    
    return 0;
}