
#define _POSIX_C_SOURCE 201712L
#define MAXLINE 8192
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>

#define INP_SIZE 256

int open_clientfd(char *hostname, char *port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV; /* Use numeric port arg */
    hints.ai_flags |= AI_ADDRCONFIG; /* Recommended for connections */
    getaddrinfo(hostname, port, &hints, &listp);


    /* Walk the list for one that we can successfully connect to */
    for(p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor*/
        if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            continue; /* Socket failed, try the next*/ 
        }
        /* Connect to the server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
            break; /* Success */
        }         
        close(clientfd); /* Connect failed, try another */
    }
    /* Clean up */
    freeaddrinfo(listp);
    if (!p){  /* All connects failed */
         return-1;
    } else { /* The last connect succeeded */
        return clientfd;
    }
}

int main(int argc, char **argv) {
    int clientfd;
    char *host, *port, buf[MAXLINE];
    host = argv[1];
    port = argv[2];
    clientfd= open_clientfd(host, port);
    while(fgets(buf, MAXLINE, stdin) != NULL) {
        write(clientfd, buf, strlen(buf));
        read(clientfd, buf, MAXLINE);
        fputs(buf, stdout);
    }
    close(clientfd);
    char inp[INP_SIZE];
    while (true) {
        printf("> ");   // get command
        fflush(stdout);

        // Parse argv from input
        fgets(inp, INP_SIZE, stdin); 
        inp[strlen(inp) - 1] = '\0';
        //printf("%s\n", inp);
        char *tmp = strtok(inp, " ");
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
        // Commands
        if (strcmp(args[0], "quit") == 0) {
            // printf("MAIN 4");
            break;
        }else{
            
        }
    }
}