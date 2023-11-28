#define _POSIX_C_SOURCE 201712L
#define MAXLINE 8192
typedef struct sockaddr SA;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

float get_data(){
    
}
 

int open_listenfd(char *port) {
    struct addrinfo hints, *listp, *p;
    int listenfd, optval = 1;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_socktype = SOCK_STREAM; /* Accept connect. */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* …on any IP addr */
    hints.ai_flags |= AI_NUMERICSERV; /* …using numeric port no. */
    getaddrinfo(NULL, port, &hints, &listp);

    /* Walk the list for one that we can bind to */
    for(p = listp; p; p = p->ai_next) {
        /* Createa a socket descriptor*/
        if((listenfd= socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            continue; /* Socket failed, try the next*/
        }

        /* Eliminates "Address already in use" errorfrom bind */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));

        /* Bindthe descriptor to the address*/
        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
            break; /* Success */
        }
        
        close(listenfd); /* Bind failed, try the next */
    }

    // clean up
    freeaddrinfo(listp);
    if (!p) {  /* No address worked */
        return-1;
    }

    /* Make it a listening socket ready to accept conn. requests */
    if (listen(listenfd, 1) < 0) {
        close(listenfd);
        return -1;
    }

    return listenfd;
}

void echo(int connfd) {
    size_t n;
    char buf[MAXLINE];
    while((n = read(connfd, buf, MAXLINE)) != 0) {
        printf("server received %d bytes\n", (int)n);

        char *tmp = strtok(buf, " ");
        if (tmp == NULL) {
            // printf("MAIN 2");
            continue;
        }
        //tokenize input
        char *args[50];
        int num_args = 0;
        while (tmp != NULL) {
            // printf("MAIN 3");
            args[num_args] = strdup(tmp);
            num_args += 1;
            tmp = strtok(NULL, " ");
        }

        for(int i = 0; i < num_args; i++){
            printf("%s", args[i]);
            write(connfd, args[i], n);
        }
    }
}

int main(int argc, char **argv) {
    //! PORT IS THE LAST ARGUMENT IN ARGV, ALL CSV FILES ARE BEFORE

    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; /* Enough room for any addr */
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    printf("opening port\n");
    listenfd = open_listenfd(argv[1]);
    printf("listenfd: %d \n", listenfd);
    if (listenfd < 0) {
        fprintf(stderr, "Error opening listening socket\n");
        exit(1);
    }

    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
        if (connfd < 0) {
            fprintf(stderr, "Error accepting connection\n");
            continue;  // Continue to the next iteration of the loop
        }

        getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);

        echo(connfd);  // Handle the connection

        close(connfd);
    }

    exit(0);
}