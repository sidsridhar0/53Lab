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

int main(){
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
    printf("Initialized\n");

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
        if(strcmp(argv[0], "quit") == 0){
            break;
        }
    }

    return 0;
}