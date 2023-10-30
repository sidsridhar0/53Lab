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
    struct Page mm[4];    //4 mm
};

struct DiskMemory{
    int dm[16][8];    //16 dm
};

int main(){
    printf("hello world");
}