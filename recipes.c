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