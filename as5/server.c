#define _POSIX_C_SOURCE 201712L
#define MAXLINE 256
typedef struct sockaddr SA;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

char *stock_list[50];

//struct Stockdata stockdata_list[10][350];

// typedef struct {
//     char date[11];
//     double close;
// } StockDay;

// typedef struct {
//     char* stock_name;
//     struct StockDay stock_days[300];
// } Stock;

// struct Stock master_stocks[50];

// void parse_csv(const char *file_path){
//     FILE *file = fopen(file_path, "r");
//     if (!file) {
//         perror("Error opening file");
//         exit(1);
//     }else{
        
//     }
// }

char *make_list() {
    char *print_list = (char *)malloc(MAXLINE);
    print_list[0] = '\0';

    for (int i = 0; i < 50 && stock_list[i] != NULL; i++) {
        strcat(print_list, stock_list[i]);
        strcat(print_list, " | ");
    }
    print_list[strlen(print_list) - 2] = '\0';
    return print_list;
}

char* get_price(char **argv) {
    return NULL;
    char *print_statement;
    return print_statement;
}

char* max_profit(char **argv) {
    return NULL;
    char* max_prof1 = "test";
    return max_prof1;
}

void load_data(int argc, char **argv) {
    for (int i = 1; i < argc - 1; i++) {
        argv[i][strlen(argv[i]) - 4] = '\0';
        stock_list[i - 1] = argv[i];
        //stockdata_list[i - 1] = 
    }
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

void handle_commands(int connfd) {
    size_t n;
    char buf[MAXLINE] = "";
    while((n = read(connfd, buf, MAXLINE)) != 0) {
        printf("%s", buf);

        char *tmp = strtok(buf, " ");
        if (tmp == NULL) {
            continue;
        }

        //tokenize input
        char *args[50];
        int num_args = 0;
        while (tmp != NULL) {
            args[num_args] = strdup(tmp);
            num_args += 1;
            tmp = strtok(NULL, " ");
        }
        memset(buf, 0, sizeof(buf));

        if(strstr(args[0], "quit")){
            exit(0);
        }else{
            //commands
            if(strstr(args[0], "List")){
                char *print_list = make_list();
                write(connfd, print_list, strlen(print_list));
                free(print_list);
            } else if (strstr(args[0], "Prices")) {
                printf("Flag 1");
                char *price = get_price(args);
                printf("Flag 2");
                write(connfd, price, strlen(price));
                printf("Flag 3");
                free(price);
            } else if (strstr(args[0], "MaxProfit")) {
                char *max_prof = max_profit(args);
                write(connfd, max_prof, strlen(max_prof));
            } else {
                char *inv_syn = "Invalid syntax";
                write(connfd, inv_syn, strlen(inv_syn));
            }
        }
        fflush(stdout);
    }
}

int main(int argc, char **argv) {
    //! PORT IS THE LAST ARGUMENT IN ARGV, ALL CSV FILES ARE BEFORE

    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; /* Enough room for any addr */
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    load_data(argc, argv);

    printf("server started\n");
    listenfd = open_listenfd(argv[argc - 1]);
    if (listenfd < 0) {
        fprintf(stderr, "Error opening listening socket\n");
        exit(1);
    }

    clientlen = sizeof(struct sockaddr_storage);
    connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
    if (connfd < 0) {
        fprintf(stderr, "Error accepting connection\n");
        close(connfd);
        exit(0);
    }

    getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);

    handle_commands(connfd);

    close(connfd);

    exit(0);
}