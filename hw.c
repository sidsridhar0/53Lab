#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INP_SIZE 100

struct Page{
    bool valid;     //if valid, in main memory(pp), else in dp
    bool dirty;     //written to
    int pp;         //physical page address
    // int dp;         //disk page address
};

struct Page virt_mem[16];
int main_mem[4][8];
int disk_mem[16][8];

int fifo_list[4];
int lru_list[4];
int open_spot = 0;

bool algo_fifo;

void showmain(int page_num) {
    for (int i = 0; i < 8; i++){
        printf("%d: %d\n", i, main_mem[page_num][i]);
    }
}

void showptable() {
    for (int i = 0; i < 16; i++){
        printf("%d:%d:%d:%d\n", i, virt_mem[i].valid, virt_mem[i].dirty, i);
    }
}

void load_page(int disk_page, int main_page){
    for(int i = 0; i < 8; i++){
        main_mem[main_page][i] = disk_mem[disk_page][i];
    }
}

void evict_fifo(){
    for(int i = 0; i < 3; i++){
        fifo_list[i] = fifo_list[i+1];
    }
}

int evict_lru(){
    int evicted = lru_list[0];
}

int evict_page(){
    int page;
    return page;
    if(algo_fifo == true){
        evict_fifo();
        page = 3;
    }else{
        page = evict_lru();
    }
    return page;
}

void page_fault(int page){ // REVISIT
    printf("A Page Fault Has Occurred\n");
    if(open_spot < 4){
        load_page(page, open_spot);
        virt_mem[page].pp = open_spot;
        fifo_list[open_spot] = open_spot;
        open_spot++;
    } else{
        int victim = fifo_list[0];
        int evicted = evict_page();
        fifo_list[3] = victim;
        load_page(page, victim);
        virt_mem[page].pp = victim;
    }
    virt_mem[page].valid = 1;
}

void read_addy(int addy){
    int page = addy / 8;
    int local_addr = addy % 8;

    if(virt_mem[page].valid == false){
        page_fault(page);
    }
    int main_page = virt_mem[page].pp;
    printf("%d\n", main_mem[page][local_addr]);
}

void write_addy(int addy, int val){
    int page = addy / 8;
    int local_addr = addy % 8;
    disk_mem[page][local_addr] = val;
    if(virt_mem[page].valid == false){
        page_fault(page);
    }
    int main_page = virt_mem[page].pp;
    main_mem[main_page][local_addr] = val;
    printf("WRITTEN: %d", main_mem[main_page][local_addr]);
}

void init_memory(){
    for (int i = 0; i < 16; i++) {
        virt_mem[i].valid = 0;
        virt_mem[i].dirty = 0;
        virt_mem[i].pp = -1;

        for (int j = 0; j < 8; j++) {
            disk_mem[i][j] = -1;
            if(i < 4){
                main_mem[i][j] = -1;
            }
        }
    }
    printf("Initialized\n");
}


int main(int argc, char* arg[]){
    algo_fifo = true;
    if (argc > 1 && strcmp(arg[1], "LRU") == 0) {
        algo_fifo = false;
    }
    // Init algorithm
    init_memory();

    char inp[INP_SIZE];
    while (true) {
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