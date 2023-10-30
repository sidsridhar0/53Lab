#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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

    return 0;
}