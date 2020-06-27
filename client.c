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
    printf("Im in client print\n");
        return -1;
    }   
    clock_t start_t, end_t;
    start_t = clock();
    printf("sart is %ld\n",start_t);
    double tot;
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
        if(*lil != '!')
            strcat(buffer, lil); 
        ++bufi;
    }while(*lil != '!');
    close(cfd); 
    time(&ltime);
    end_t = clock();
    printf("end)t is %ld\n",end_t);
    tot = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    if(!strcmp("NO PATH", buffer)){
        printf("%s : Server’s response (%d): NO PATH, arrived in %.4fseconds, shutting down\n", strtok(ctime_r(&ltime, buf), "\n"),pid, tot);
    }
    else{
        printf("%s : Server’s response to (%d): %s, arrived in %.4fseconds.\n", strtok(ctime_r(&ltime, buf), "\n"),pid, buffer, tot);
    }
    exit(EXIT_SUCCESS);    
    return 0;
}