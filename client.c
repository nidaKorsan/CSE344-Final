/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/
#include "recipes.h"

int main(int argc, char *argv[]){
    mainArgsClient margs;
    time_t ltime;
    char buf[50];
    int pid = getpid();
    if(readArgumentsClient(argc, argv, &margs)){
        return -1;
    }
    time(&ltime);
    printf("%s : Client (%d) connecting to %s:%s\n",strtok(ctime_r(&ltime, buf), "\n"), pid, margs.ipAdress,margs.port);
    int cfd;
    if((cfd = clientConnect(&margs)) == -1){
        return -1;
    }
    struct timespec start, finish;
    clock_gettime(CLOCK_REALTIME, &start);
    time(&ltime);
    printf("%s : Client (%d) connected and requesting a path from node %d to %d\n",strtok(ctime_r(&ltime, buf), "\n"), pid, margs.src, margs.dest);
    // Send source and destination, with terminating newline
    time(&ltime);
    if (write(cfd, &margs.src, sizeof(margs.src)) == -1)
        printf("%s : Client write to server failed\n",strtok(ctime_r(&ltime, buf), "\n"));
    if (write(cfd, &margs.dest, sizeof(margs.dest)) == -1)
        printf("%s : Client write to server failed\n",strtok(ctime_r(&ltime, buf), "\n"));
    int max = 1024; 
    char *buffer = malloc(sizeof(char) * max);
    memset(&buffer, 0, sizeof(*buffer));
    int bytesRead, bufi = 0;
    char lil[2] = "";
    // Read and display the string returned by server
    do{
        *lil = '\0';
        while(((bytesRead = read(cfd, lil, 1)) == -1) && (errno == EINTR));
        if(bytesRead < 0){
            printf("%s : Error while read %s", strtok(ctime_r(&ltime, buf), "\n"), strerror(errno));
            free(buffer);
            buffer = NULL;
            return -1;
        }
        if(bufi >= max)
            buffer = realloc(buffer, (max *= 2) * sizeof(char));
        ++bufi;
        if(*lil != '\n')
            strcat(buffer, lil);
    }while(*lil != '\n');
    clock_gettime(CLOCK_REALTIME, &finish); 
    time(&ltime);
    long seconds = finish.tv_sec - start.tv_sec;
    long ns = finish.tv_nsec - start.tv_nsec;
    if(!strcmp("NO PATH\n", buffer)){
        printf("%s : Server’s response (%d): NO PATH, arrived in %.3fseconds, shutting down\n", strtok(ctime_r(&ltime, buf), "\n"),pid, (double)seconds + (double)ns/(double)1000000000);
    }
    else{
        printf("%s : Server’s response to (%d): %s, arrived in %.3fseconds.\n", strtok(ctime_r(&ltime, buf), "\n"),pid, buffer, (double)seconds + (double)ns/(double)1000000000);
    }
    exit(EXIT_SUCCESS);    
    return 0;
}