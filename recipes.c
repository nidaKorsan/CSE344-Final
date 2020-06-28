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
    //unlink("/tmp/MyUniqueName");close(myfd);
}

int daemonBorn(mainArgsServer *margs){
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
    margs->fout = open(margs->outputPath, O_RDWR | O_CREAT);
    if (margs->fout != STDIN_FILENO) /* 'fd' should be 0 */
    return -1;
    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
    return -1;
    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
    return -1;
    if((margs->fin = open(margs->inputPath, O_RDWR)) == -1){
        printf("Error while opening file %s\n", strerror(errno));
        return -1;
    }

    /*char lokum[25]; strcpy(lokum,"lokumgib"); 
    write(margs->fout, lokum, strlen(lokum));*/
    //sleep(6);
    return 0;
}

//for reading command line arguments
int readArgumentsServer(int argc, char *argv[], mainArgsServer *margs){
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

//for reading command line arguments
int readArgumentsClient(int argc, char *argv[], mainArgsClient *margs){
    int opt;
    int flag_a = 0, flag_p = 0, flag_s = 0, flag_d = 0;
    //./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24
    if(argc != 9) {
        printf("Wrong argument count. There should be exactly 9.\nUsage case : ./client -a 127.0.0.1 -p PORT -s 768 -d 979\n");
        return -1;
    }
    while((opt = getopt(argc, argv, "a:p:s:d:")) != -1)
    {
        switch(opt)
        {
            case 'a':
                if (flag_a){
                    printf("-a option is already given.\nUsage case : ./client -a 127.0.0.1 -p PORT -s 768 -d 979\n");
                    return -1;
                } /*If -a parameter is already given give error*/
                margs->ipAdress = optarg;
                //printf("reading %s %s\n",optarg, margs->ipAdress);
                flag_a = 1;
                break;
            case 'p':
                if(flag_p){
                    printf("-p option is already given.\nUsage case : ./client -a 127.0.0.1 -p PORT -s 768 -d 979\n");
                    return -1;
                } /*If -p parameter is already given give error*/
                margs->port = optarg;
                flag_p = 1;
                break;
            case 's':
                if(flag_s){
                    printf("-s option is already given.\nUsage case : ./client -a 127.0.0.1 -p PORT -s 768 -d 979\n");
                    return -1;
                } /*If -s parameter is already given give error*/
                margs->src = atoi(optarg);
                flag_s = 1;
                break;
            case 'd':
                if(flag_d){
                    printf("-d option is already given.\nUsage case : ./client -a 127.0.0.1 -p PORT -s 768 -d 979\n");
                    return -1;
                } /*If -d parameter is already given give error*/
                margs->dest = atoi(optarg);
                flag_d = 1;
                break;                                
            case '?':
                printf("Unknown option\nUsage case : ./client -a 127.0.0.1 -p PORT -s 768 -d 979\n");
                return -1;
        default:
            break;
        }
    }
    return 0;
}

//Reads from input file and adds edges to the graph if asked
int readFromFile(int fin, int choice, graph_t *graph, int *maxNum, double *tot){
    clock_t start_t, end_t;
    start_t = clock();
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
        if(choice && !flag && tempEdge.dest != -1 && tempEdge.src != -1 && bytesRead != 0){
            //printf("%d	%d\n",tempEdge.src,tempEdge.dest);
            addEdge(graph, tempEdge);
        }
        *buffer = '\0';
    }while(bytesRead != 0);
    if(choice){
        end_t = clock();
        *tot = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    }
    UNLOCK(fin);
    *maxNum = maxNodeNum;
    return 0;
}

int initSocket(char* portNum){
    int lfd, optval;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    #define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
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
            return -1;
        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break; /* Success */
        /* bind() failed: close this socket and try next address */
        close(lfd);
    }
    if (rp == NULL)
        printf("Could not bind socket to any address\n");
    if (listen(lfd, BACKLOG) == -1)
        return -1;

    return lfd;
}

void printServerInfo(mainArgsServer margs,graph_t graph, double totalTime){
    time(&ltime);
    char buf[50];
    printf("%s : Executing with parameters:\n\
%s : -i %s\n\
%s : -p %s\n\
%s : -o %s\n\
%s : -s %d\n\
%s : -x %d\n\
%s : Loading graph...\n\
%s : Graph loaded in %.3f seconds with %d nodes and %d edges.\n",strtok(ctime_r(&ltime, buf), "\n"), \
    strtok(ctime_r(&ltime, buf), "\n"), margs.inputPath,\
    strtok(ctime_r(&ltime, buf), "\n"),margs.port, \
    strtok(ctime_r(&ltime, buf), "\n"), margs.outputPath, \
    strtok(ctime_r(&ltime, buf), "\n"),margs.threadNum, \
    strtok(ctime_r(&ltime, buf), "\n"), margs.maxThreadNum,\
    strtok(ctime_r(&ltime, buf), "\n"),strtok(ctime_r(&ltime, buf), "\n")\
    , totalTime, graph.numVertice, graph.numEdge);
}

int clientConnect(mainArgsClient *margs){
    int cfd;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    /* Call getaddrinfo() to obtain a list of addresses that
    we can try connecting to */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    //printf("client connect, %s\n", margs->ipAdress);
    if (getaddrinfo(margs->ipAdress, margs->port, &hints, &result) != 0)
        return -1;
    /* Walk through returned list until we find an address structure
    that can be used to successfully connect a socket */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cfd == -1)
            continue; /* On error, try next address */
        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1){
            break; /* Success */
        }
        printf("Error connect  %s\n", strerror(errno));
        /* Connect failed: close this socket and try next address */
        close(cfd);
    }
    if (rp == NULL){
        printf("Could not connect socket to any address\n");
        return -1;
    }
    freeaddrinfo(result);
    return cfd;  
}

void* daemonThreadAct(void *arg){
    int id = (int) (long)arg;
    char *cret, buf[50];
    int ret;
    time(&ltime);
    printf("%s : Thread #%d: waiting for connection\n", strtok(ctime_r(&ltime, buf), "\n"), id);
    int socketIdDaemon;
    while(1){
        MLOCK(&shared.assignWorkMutex);
        while(shared.socketId == -1)
            pthread_cond_wait(&shared.assignWorkCond, &shared.assignWorkMutex);
        socketIdDaemon = shared.socketId;
        shared.socketId = -1;
        pthread_cond_signal(&shared.noWorkCond);//signal to the main thread
        MUNLOCK(&shared.assignWorkMutex);

        MLOCK(&shared.loadFactorMutex);
        ++shared.busyTNum;//Increase busy thread count
        time(&ltime);
        printf("%s : A connection has been delegated to thread id #%d, system load %.3f%%\n", strtok(ctime_r(&ltime, buf), "\n"), id, (double)shared.busyTNum / (double)shared.currentThreadCount* 100);
        pthread_cond_signal(&shared.loadFactorCond);//signal to the pooler
        MUNLOCK(&shared.loadFactorMutex);

        int src, dest;
        if(read(socketIdDaemon, &src, sizeof(src)) == -1){
            printf("Error read %s\n", strerror(errno));
            return NULL;
        }
        if(read(socketIdDaemon, &dest, sizeof(dest)) == -1){
            printf("Error read %s\n", strerror(errno));
            return NULL;
        }
        time(&ltime);
        printf("%s : Thread #%d: searching database for a path from node %d to node %d\n",strtok(ctime_r(&ltime, buf), "\n"), id, src, dest);
        //database search
        //if database search is successful
            //print path
        //else
        {
            time(&ltime);
            printf("%s : Thread #%d: no path in database, calculating %d->%d\n",strtok(ctime_r(&ltime, buf), "\n"), id, src, dest);
            cret = bfsSearch(shared.graph, src, dest);
            if(!strcmp("NO PATH", cret)){
                time(&ltime);
                printf("%s : Thread #%d: path not possible from node %d to %d\n", strtok(ctime_r(&ltime, buf), "\n"), id, src, dest);
                if(write(socketIdDaemon, "NO PATH", 8) == -1){
                    printf("Error write %s\n", strerror(errno));
                    return NULL;//raise
                }
            }
            else{
                time(&ltime);
                printf("%s : Thread #%d: path calculated: %s\n",strtok(ctime_r(&ltime, buf), "\n"), id, cret);
                printf("%s : Thread #%d: responding to client and adding path to database\n",strtok(ctime_r(&ltime, buf), "\n"), id);
                //add path to database
                if(write(socketIdDaemon, cret, strlen(cret)) == -1){
                    printf("Error write %s\n", strerror(errno));
                    return NULL;//raise
                }
                if(write(socketIdDaemon, "!", 2) == -1){
                    printf("Error write %s\n", strerror(errno));
                    return NULL;//raise
                }
            }
        }
        MLOCK(&shared.loadFactorMutex);
        --shared.busyTNum;
        MUNLOCK(&shared.loadFactorMutex);
    }
    return NULL;
}

void* poolerThreadAct(void *arg){
    char buf[50];
    mainArgsServer * marg= (mainArgsServer *)arg;
    int ret, optval;
    shared.currentThreadCount = marg->threadNum;
    while( shared.idIndex < marg->threadNum + 1){
        optval = shared.idIndex-1;
        if((ret = pthread_create(&shared.thread_id[shared.idIndex++], NULL, daemonThreadAct, (void *)(long)(optval))) != 0){
            printf("Error pthread_create %s\n", strerror(ret));
            raise(SIGINT);
        }
    }
    printf("%s : A pool of %d threads has been created\n",strtok(ctime_r(&ltime, buf), "\n"), shared.idIndex - 1);
    while(marg->threadNum != marg->maxThreadNum){   
        MLOCK(&shared.loadFactorMutex);
        //if load factor is more than 75%
        while((double)shared.busyTNum / (double)marg->threadNum < 0.75){
            pthread_cond_wait(&shared.loadFactorCond, &shared.loadFactorMutex);
        }
        marg->threadNum = marg->threadNum * 1.25;
        if(marg->threadNum > marg->maxThreadNum)
            marg->threadNum = marg->maxThreadNum;
        shared.currentThreadCount = marg->threadNum;
        shared.thread_id = realloc(shared.thread_id, marg->threadNum * sizeof(pthread_t));
        while( shared.idIndex < marg->threadNum + 1){
            optval = shared.idIndex-1;
            if((ret = pthread_create(&shared.thread_id[shared.idIndex++], NULL, daemonThreadAct, (void *)(long)(optval))) != 0){
                printf("Error pthread_create %s\n", strerror(ret));
                raise(SIGINT);
            }
        }
        printf("%s : System load %.2f%%, pool extended to %d threads\n",strtok(ctime_r(&ltime, buf), "\n"), (double)shared.busyTNum / (double)marg->threadNum, marg->threadNum);
        MUNLOCK(&shared.loadFactorMutex);
    }
    return 0;
}