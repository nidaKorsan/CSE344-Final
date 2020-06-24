/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/
#include"recipes.h"

int daemonBorn(){
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
    if ((chdir("/")) < 0) {// Change to root directory
        printf("Error during chdir %s\n",strerror(errno));
        return -1;
    }
    int i;
    //Close all of the open file descriptors (all possible ones)
    for (i = sysconf(_SC_OPEN_MAX); i >= 0; close (i--));
    int fd;
    close(STDIN_FILENO); //Reopen standard fd's to /dev/null
    fd = open("/dev/null", O_RDWR);
    if (fd != STDIN_FILENO) // 'fd' should be 0 
        return -1;
    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
        return -1;
    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
        return -1;
    return 0;
}

int readArguments(int argc, char *argv[], mainArgs *margs){
    int opt;
    char *inputFilePath = NULL;
    char *outputFilePath = NULL;
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
                inputFilePath = optarg;
                flag_i = 1;
                break;
            case 'p':
                if(flag_p){
                    printf("-p option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -p parameter is already given give error*/
                margs->port = atoi(optarg);
                flag_p = 1;
                break;
            case 'o':
                if(flag_o){
                    printf("-o option is already given.\nUsage case : ./server -i pathToFile -p PORT -o pathToLogFile -s 4 -x 24\n");
                    return -1;
                } /*If -o parameter is already given give error*/
                outputFilePath = optarg;
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

    if ((margs->fin = open(inputFilePath, O_RDWR, 0666)) == -1){
        printf("Error while opening the input file: %s\n", inputFilePath);
        return -1;
    }

    if ((margs->fout = open(outputFilePath,O_RDWR | O_CREAT, 0666)) == -1){
        close(margs->fin);
        printf("Error while opening the output file: %s\n", outputFilePath);
        return -1;
    }
    return 0;
}
