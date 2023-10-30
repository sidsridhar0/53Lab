#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INP_SIZE 100
struct Page{
    int addy[8];
    bool valid;     //if valid, in main memory(pp), else in dp
    bool dirty;     //written to
    int pp;         //physical page address
    int dp;         //disk page address
};

struct VirtualMemory{
    struct Page vm[16];    //16 vm
};

struct MainMemory{
    int mm[4][8];    //4 mm
};

struct DiskMemory{
    int dm[16][8];    //16 dm
};

void read_addy(int addy){

}

void write_addy(int addy, int num){

}

void showmain(int addy){

}

void showptable(){

}

int main(int argc, char* arg[]){
    //init algorithm
    bool algo_fifo = true;
    if(strcmp(arg[0], "LRU") == 0){
        algo_fifo = false;
    }
    struct VirtualMemory virtualMemory;
    struct DiskMemory diskMemory;
    struct MainMemory mainMemory;
    for (int i = 0; i < 16; i++) {
        virtualMemory.vm[i].valid = false;
        virtualMemory.vm[i].dirty = false;
        virtualMemory.vm[i].pp = -1;
        virtualMemory.vm[i].dp = -1;

        for (int j = 0; j < 8; j++) {
            virtualMemory.vm[i].addy[j] = -1;
            diskMemory.dm[i][j] = -1;
            if(i < 4){
                mainMemory.mm[i][j] = -1;
            }
        }
    }
    printf("INITIALIZED\n");

    char inp[INP_SIZE];
    while(1){
        //COPIED FROM LAB 2
        printf("> ");
        fflush(stdout);

        // parse argv from input
        fgets(inp, INP_SIZE, stdin);
        inp[strlen(inp)-1] = '\0';
        printf("%s", inp);
        char *tmp = strtok(inp, " ");
        if(tmp == NULL){
            continue;
        }
        char *argv[50];
        int num_args = 0;
        while (tmp != NULL) {
            argv[num_args] = strdup(tmp);
            num_args += 1;
            tmp = strtok(NULL, " ");
        }

        //commands
        if(strcmp(argv[0], "quit") == 0){
            break;
        }else if(strcmp(argv[0], "showptable") == 0){
            //argv[1] should be int
            showptable();
        }else{
            int num;
            num = atoi(argv[1]);
            if(strcmp(argv[0], "read") == 0){
                //argv[1] should be int
                read_addy(0);
            }else if(strcmp(argv[0], "write") == 0){
                //argv[1] and argv[2] should be ints
                write_addy(0, 0);
            }else if(strcmp(argv[0], "showmain") == 0){
                //argv[1] should be int
                showmain(0);
            }else{
                printf("INVALID COMMAND\n");
            }
        }
    }
    return 0;
}