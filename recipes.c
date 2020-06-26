/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/
#include"recipes.h"

int callSigAction(){
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if((sigfillset(&sa.sa_mask) == -1) || (sigaction(SIGINT, &sa, NULL) == -1))
        return -1;
    return 0;
}

void handler(int signalNumber){
    switch(signalNumber){
        case SIGINT:
           //Do the exit
            printf("Exiting gracefully\n");
            exitGracefully();
            exit(EXIT_SUCCESS);
    }
}

void exitGracefully(){

}

int daemonBorn(mainArgs *margs){
    pid_t sid;
    switch (fork()) { //Become background process 
        case -1:
            printf("Error during fork for server %s\n", strerror(errno));
            return -1;
        case 0: break; //Child falls through; adopted by init
        default: _exit(EXIT_SUCCESS);//parent terminates and shell prompt is back
    }
    //start a new session and free itself of any association with a controlling terminal
    if((sid = setsid()) == -1){
        printf("Error during setid %s\n", strerror(errno));
    }
    switch (fork()) {//Ensure we are not session leader 
        case -1:// thanks to 2nd fork, there is no way of acquiring a tty
            printf("Error during fork for server %s\n", strerror(errno));
            return -1;
        case 0: break;
        default: _exit(EXIT_SUCCESS);
    }
    umask(0);//Clear file mode creation mask
    //TODO may uncomment this later
    /*if ((chdir("/")) < 0) {// Change to root directory
        printf("Error during chdir %s\n",strerror(errno));
        return -1;
    }*/
    int i;
    //Close all of the open file descriptors (all possible ones)
    for (i = sysconf(_SC_OPEN_MAX); i >= 0; close (i--));
    //open input file and output log file
    if((margs->fin = open(margs->inputPath, O_RDWR)) == -1){
        printf("Error while opening file %s\n", strerror(errno));
        return -1;
    }
    if((margs->fout = open(margs->outputPath, O_RDWR | O_CREAT)) == -1){
        printf("Error while opening file %s\n", strerror(errno));
        close(margs->fin);
        return -1;
    }
    /*char lokum[25]; strcpy(lokum,"lokumgib"); 
    write(margs->fout, lokum, strlen(lokum));*/
    //sleep(6);
    return 0;
}

//for reading command line arguments
int readArgumentsServer(int argc, char *argv[], mainArgs *margs){
    int opt;
    int flag_i = 0, flag_o = 0, flag_p = 0, flag_s = 0, flag_x = 0;
    //./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24
    if(argc != 11) {
        printf("Wrong argument count. There should be exactly 11.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
        return -1;
    }
    while((opt = getopt(argc, argv, "i:p:o:s:x:")) != -1)
    {
        switch(opt)
        {
            case 'i':
                if (flag_i){
                    printf("-i option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -i parameter is already given give error*/
                margs->inputPath = optarg;
                flag_i = 1;
                break;
            case 'p':
                if(flag_p){
                    printf("-p option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -p parameter is already given give error*/
                margs->port = optarg;
                flag_p = 1;
                break;
            case 'o':
                if(flag_o){
                    printf("-o option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -o parameter is already given give error*/
                margs->outputPath = optarg;
                flag_o = 1;
                break;
            case 's':
                if(flag_s){
                    printf("-s option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -s parameter is already given give error*/
                if((margs->threadNum = atoi(optarg)) < 2){
                    printf("-s argument cannot be less than 2.\n");
                    return -1;
                }
                flag_s = 1;
                break;
            case 'x':
                if(flag_x){
                    printf("-x option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -x parameter is already given give error*/
                margs->maxThreadNum = atoi(optarg);
                flag_x = 1;
                break;                                
            case '?':
                printf("Unknown option\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                return -1;
        default:
            break;
        }
    }
    return 0;
}


//Reads from input file
int readFromFile(int fin, int choice, graph_t *graph, int *maxNum){
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    int bytesRead = 0, num = -1, maxNodeNum = -1, flag = 0;//flag 0 means source
    char buffer[2] = "", numBuf[15] = "";
    edge_t tempEdge; tempEdge.dest = -1; tempEdge.src = -1;
    LOCK(fin);
    if(lseek(fin, 0, SEEK_END) == lseek(fin, 0, SEEK_SET)){
        printf("Error, empty input file\n");
        return -1;
    }   
    do{
        while(((bytesRead = read(fin, buffer, 1)) == -1) && (errno == EINTR));
        if(bytesRead < 0){
            printf("Error while reading input file: %s", strerror(errno));
            UNLOCK(fin);
            return -1;
        }
        if(*buffer == '#'){//if the read byte is #, means comment, go to the next line
            while(*buffer != 10 && bytesRead != 0){
                *buffer = '\0';
                while(((bytesRead = read(fin, buffer, 1)) == -1) && (errno == EINTR));
                if(bytesRead < 0){
                    printf("Error while reading input file: %s", strerror(errno));
                    UNLOCK(fin);
                    return -1;
                }
                printf("%s", buffer);
            }
        }
        else if(isdigit(*buffer) != 0){//if read byte is a numeric character
            strcpy(numBuf, buffer);
            while(*buffer != 9 && *buffer != 10 && bytesRead != 0){//while read byte is not tab or newline
                *buffer = '\0';
                while(((bytesRead = read(fin, buffer, 1)) == -1) && (errno == EINTR));
                if(bytesRead < 0){
                    printf("Error while reading input file: %s", strerror(errno));
                    UNLOCK(fin);
                    return -1;
                }                    
                if(*buffer != 9 && *buffer != 10 && bytesRead != 0){
                    strcat(numBuf, buffer);
                    printf("buffer issss %s\n",buffer);
                }
            }
            num = atoi(numBuf);
            *numBuf = '\0';
            if(!flag){tempEdge.src = num; flag = 1;}
            else {tempEdge.dest = num; flag = 0;}
            if(num > maxNodeNum && !choice) {
                maxNodeNum = num;
            }
        }
        if(choice && !flag && tempEdge.dest != -1 && tempEdge.src != -1){
            addEdge(graph, tempEdge);
        }
        *buffer = '\0';
    }while(bytesRead != 0);
    *maxNum = maxNodeNum;
    return 0;
}

int initSocket(char* portNum){
    struct sockaddr_storage claddr;
    int lfd, cfd, optval;
    socklen_t addrlen;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    #define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        exit(-1);
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
 /* Wildcard IP address; service name is numeric */
    if (getaddrinfo(NULL, portNum, &hints, &result) != 0)
        exit(-1);
    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (lfd == -1)
        continue; /* On error, try next address */

        if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        exit(-1);
        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
        break; /* Success */
        /* bind() failed: close this socket and try next address */
        close(lfd);
    }
    if (rp == NULL)
        printf("Could not bind socket to any address");
    if (listen(lfd, BACKLOG) == -1)
        exit(-1);
    addrlen = sizeof(struct sockaddr_storage);
    cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
    if (cfd == -1) {
        perror("accept");
        //continue;
    }
    close(lfd);
    if (getnameinfo((struct sockaddr *) &claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
    else
        snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
    printf("Connection from %s\n", addrStr);
    if (write(cfd, "Im sending client smt\n", 24) == -1)
        printf("Error write\n");
    char buffer[50];
    if(read(cfd, buffer, 30) == -1)
        printf("Error read\n");
    printf("In server %s\n", buffer); 
    if (close(cfd) == -1) /* Close connection */
        perror("close");
    return 0;
}