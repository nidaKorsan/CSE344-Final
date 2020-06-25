/**********************************************
; *  344 Sytem Programming FINAL              *
; *  Spring 2020                              *
; *  Author: Nida Korsan                      *
; *********************************************/
#include "recipes.h"

int main(int argc, char *argv[]){
    int cfd;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        printf("%s server-host [sequence-len]\n", argv[0]);
    /* Call getaddrinfo() to obtain a list of addresses that
    we can try connecting to */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    if (getaddrinfo(argv[1], PORT, &hints, &result) != 0)
        exit(-1);
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
    /* Send requested sequence length, with terminating newline */
    char buffer[60];
    if (read(cfd, buffer, 30) == -1)
        printf("Partial/failed write (reqLenStr)\n");
    printf("Buffer is : %s\n", buffer);
    if (write(cfd, "im the client\n", 16) == -1)
        printf("Partial/failed write (reqLenStr)\n");

    /* Read and display sequence number returned by server */

    close(cfd); 
    exit(EXIT_SUCCESS);    
    return 0;
}