
#define _POSIX_C_SOURCE 201712L

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
    char *host, *port, buf[INP_SIZE];
    host = argv[1];
    port = argv[2];
    clientfd= open_clientfd(host, port);

    char inp[INP_SIZE];
    char output[INP_SIZE];
    while (true) {
        printf("> ");   // get command
        fflush(stdout);

        // Parse argv from input
        fgets(inp, INP_SIZE, stdin); 

        // Commands
        if (strstr(inp, "quit")) {
            // printf("MAIN 4");
            write(clientfd, inp, strlen(inp));
            close(clientfd);
            break;
        }else{
            write(clientfd, inp, strlen(inp));
            read(clientfd, output, INP_SIZE);

            fputs(output, stdout);
            printf("\n");
        }
    }
}