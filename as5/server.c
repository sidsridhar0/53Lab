//Siddharthen Sridhar 95532627
//Rohan Joseph Jayasekara 37328564

#define _POSIX_C_SOURCE 201712L
#define MAXLINE 256 // Size of the message must not 256 bytes
typedef struct sockaddr SA;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


struct StockDay { // Line from csv file
    char date[11];
    double close;
};

struct Stock { // Stock name + csv info
    char name[30];
    struct StockDay stock_days[300];
};

struct Stock data[10]; // List of Stocks [TSLA, MSFT]
int num_data = 0;

void parse_csv(char *file_path, struct StockDay stock_days[300]){ 
    // Populates stock_days list with data from csv file

    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char line[1024];
    fgets(line, sizeof(line), file);
    for(int i = 0; i<300; i++){
        if (fgets(line, sizeof(line), file)) {

            //! WORKS
            sscanf(line, "%10[^,],%*lf,%*lf,%*lf,%lf,%*lf,%*lld", stock_days[i].date, &stock_days[i].close);
            
            //! NO WARNINGS
            //sscanf(line, "%10[^,],%*f,%*f,%*f,%*f,%*f,%llf", stock_days[i].date, &stock_days[i].close);

        }
    }
    fclose(file);
}

void load_data(int argc, char **argv) {
    // Fills data with info from csv files
    for (int i = 1; i < argc - 1; i++) {
        num_data++;
        size_t length = strlen(argv[i]) + 1;
        char *stock_name = (char *)malloc(length);
        strcpy(stock_name, argv[i]);
        stock_name[length-5] = '\0';

        strcpy(data[i-1].name, stock_name);
        parse_csv(argv[i], data[i-1].stock_days);
    }
}

char *make_list() {
    // Creates string with all stock names in data
    char *print_list = (char *)malloc(MAXLINE);
    print_list[0] = '\0';

    for (int i = 0; i < num_data; i++) {
        strcat(print_list, data[i].name);
        strcat(print_list, " | ");
    }
    print_list[strlen(print_list) - 2] = '\0';
    return print_list;
}

char* get_price(char **argv) {
    // Gives the price for a specific day for a specific stock
    argv[2][strlen(argv[2])-1] = '\0';
    char* print_statement = (char *)malloc(MAXLINE);
    strcpy(print_statement, "Unknown");
    for(int i = 0; i < num_data;i++){
        if(strcmp(data[i].name,argv[1]) == 0){ // Finds matching stock name
            for(int j = 0; j<300; j++){
                // printf("%s -- %s\n", data[i].stock_days[j].date, argv[2]);
                if(strcmp(data[i].stock_days[j].date, argv[2]) == 0){ // Finds matching date
                    sprintf(print_statement, "%.2f", data[i].stock_days[j].close);
                    break;
                }
            }
            break;
        }
    }
    return print_statement;
}

char* max_profit(char **argv) {
    // Calculates max profits between two dates
    argv[3][strlen(argv[3])-1] = '\0';
    char* max_prof = (char *)malloc(MAXLINE);
    strcpy(max_prof, "Unkown");
    double max_val = 0;
    int start = -1;
    int end = -1;
    for(int i = 0; i < num_data;i++){
        if(strcmp(data[i].name,argv[1]) == 0){ // Finds desired stock
            for(int j = 0; j<300; j++){
                if(strcmp(data[i].stock_days[j].date, argv[2]) == 0){ // Finds start date pointer
                    start = j;
                }
                if(strcmp(data[i].stock_days[j].date, argv[3]) == 0){ // Finds end date pointer
                    end = j;
                    break;
                }
            }
            if(start == -1 || end == -1){ // If start or end date not found return "Invalid dates"
                return max_prof;
            }
            for(int j = start; j < end+1; j++){ // Look at every buy/sell date pair within range to find max_al
                for(int k = j; k < end+1; k++){
                    if(data[i].stock_days[k].close - data[i].stock_days[j].close > max_val){
                        max_val = data[i].stock_days[k].close - data[i].stock_days[j].close;
                    }
                }
            }
            sprintf(max_prof, "%.2f", max_val);
            break;
        }
    }
    return max_prof;
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
                char *price = get_price(args);
                write(connfd, price, strlen(price));
                free(price);
            } else if (strstr(args[0], "MaxProfit")) {
                char *max_prof = max_profit(args);
                write(connfd, max_prof, strlen(max_prof));
                free(max_prof);
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
    struct sockaddr_storage clientaddr;
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
