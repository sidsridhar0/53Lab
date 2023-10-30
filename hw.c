#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INP_SIZE 100

struct Page {
    int vpn;        //main memory
    bool valid;     // if valid, in main memory (pp), else in dp
    bool dirty;     // written to
    int dp;         // disk page address
    
};

struct VirtualMemory {
    struct Page vm[16];    // 16 vm
};

struct MainMemory {
    int mm[4][8];    // 4x8 raw values
};

struct DiskMemory {
    int dm[16][8];    // 16x8 raw values
};

struct VirtualMemory virtualMemory;
struct DiskMemory diskMemory;
struct MainMemory mainMemory;

int open_memory(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            if(mainMemory.mm[i][j] == -1){
                int addy = i * 8 + j; 
                return addy;
            }
        }
    }
    return -1;
}

void load_data(int i, int j) {
    int open_addy = open_memory();
    if(open_addy == -1){
        //RUN FIFO OR LRU HERE TO CLEAR A PAGE OTHERWISE INFINITE LOOP
        printf("RUN FIFO/LRU");
    }else{
        //mod page info
        int data = virtualMemory.vm[i];
        
    }
}

void read_addy(int addy) {
    int i = addy / 8;
    int j = addy % 8;
    int data;
    if(virtualMemory.vm[i].valid){
        data = mainMemory.mm[virtualMemory.vm[i].vpn][j];
    }else{
        printf("A Page Fault Has Occurred");
        load_data(i, j);
        read_addy(addy);
    }
    
}

void write_addy(int addy, int num) {
    int i = addy / 8;
    int j = addy % 8;
}

void showmain(int page_num) {
    for (int i = 0; i < 8; i++){
        printf("%d: %d\n", i, mainMemory.mm[page_num][i]);
    }
}

void showptable() {
    for (int i = 0; i < 16; i++){
        printf("%d:%d:%d:%d\n", virtualMemory.vm[i].dp, virtualMemory.vm[i].valid, virtualMemory.vm[i].dirty, virtualMemory.vm[i].dp);
    }
}

void init_memory(){
    for (int i = 0; i < 16; i++) {
        virtualMemory.vm[i].valid = false;
        virtualMemory.vm[i].dirty = false;
        virtualMemory.vm[i].dp = -1;

        for (int j = 0; j < 8; j++){
            diskMemory.dm[i][j] = -1;
            if (i < 4) {
                mainMemory.mm[i][j] = -1;
            }
        }
    }
    printf("INITIALIZED\n");
}

int main(int argc, char* arg[]) {
    // Init algorithm
    bool algo_fifo = true;
    if (argc > 1 && strcmp(arg[1], "LRU") == 0) {
        algo_fifo = false;
    }

    init_memory();

    char inp[INP_SIZE];
    while (1) {
        printf("> ");
        fflush(stdout);

        // Parse argv from input
        fgets(inp, INP_SIZE, stdin);
        inp[strlen(inp) - 1] = '\0';
        //printf("%s\n", inp);
        char *tmp = strtok(inp, " ");
        if (tmp == NULL) {
            continue;
        }
        char *argv[50];
        int num_args = 0;
        while (tmp != NULL) {
            argv[num_args] = strdup(tmp);
            num_args += 1;
            tmp = strtok(NULL, " ");
        }

        // Commands
        if (strcmp(argv[0], "quit") == 0) {
            break;
        } else if (strcmp(argv[0], "showptable") == 0) {
            // argv[1] should be int
            showptable();
        } else {
            if (num_args < 2) {
                printf("Insufficient arguments.\n");
                continue;
            }
            int num = atoi(argv[1]);
            if (strcmp(argv[0], "read") == 0) {
                read_addy(num);
            } else if (strcmp(argv[0], "write") == 0) {
                if (num_args < 3) {
                    printf("Insufficient arguments for write command.\n");
                    continue;
                }
                int value = atoi(argv[2]);
                write_addy(num, value);
            } else if (strcmp(argv[0], "showmain") == 0) {
                showmain(num);
            } else {
                printf("INVALID COMMAND\n");
            }
        }
    }
    return 0;
}