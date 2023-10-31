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

int fifo_list[4][2];
int lru_list[4][2];
int lru_counter = 0;
int open_spot = 0;

bool algo_fifo;

void showmain(int page_num) {
    printf("SHOWMAIN");
    for (int i = 0; i < 8; i++){
        printf("%d: %d\n", i, main_mem[page_num][i]);
    }
}

void showptable() {
    for (int i = 0; i < 16; i++){
        int pn = i;
        if(virt_mem[i].valid == true){
            pn = virt_mem[i].pp;
            printf("SHOWPTABLE");
        }
        printf("%d:%d:%d:%d\n", i, virt_mem[i].valid, virt_mem[i].dirty, pn);
    }
}

void load_page(int disk_page, int main_page){
    for(int i = 0; i < 8; i++){
        main_mem[main_page][i] = disk_mem[disk_page][i];
    }
    printf("LOAD_PAGE");
}

void evict_fifo(int page){
    int victim = fifo_list[0][0];
    int virt_page = fifo_list[0][1];
    // printf("EVICTING %d\n", virt_page);
    for(int i = 0; i < 3; i++){
        fifo_list[i][0] = fifo_list[i+1][0];
        fifo_list[i][1] = fifo_list[i+1][1];
    }
    fifo_list[3][0] = victim;
    fifo_list[3][1] = page;
    virt_mem[virt_page].valid = false;
    virt_mem[virt_page].dirty = false;
    load_page(page, victim);
    virt_mem[page].pp = victim;
    printf("EVICT_FIFO");
}

void evict_lru(int page){
    int victim = 0;
    for(int i = 1; i < 4; i++){
        if(lru_list[victim][1] > lru_list[i][1]){
            victim = i;
            printf("EVICT_LRU");
        }
    }
    int virt_page = lru_list[victim][0];
    printf("EVICTING %d\n", virt_page);
    virt_mem[virt_page].valid = false;
    virt_mem[virt_page].dirty = false;
    load_page(page, victim);
    lru_list[victim][0] = page;
    // lru_list[victim][1] = lru_counter;
    virt_mem[page].pp = victim;
}

void evict_page(int page){
    if(algo_fifo == true){
        printf("EVICT_PAGE FIFO");
        evict_fifo(page);
    }else{
        printf("EVICT_PAGE LRU");
        evict_lru(page);
    }
}

void page_fault(int page){ // REVISIT
    printf("A Page Fault Has Occurred\n");
    if(open_spot < 4){
        load_page(page, open_spot);
        virt_mem[page].pp = open_spot;
        fifo_list[open_spot][0] = open_spot;
        fifo_list[open_spot][1] = page;

        lru_list[open_spot][0] = page;
        lru_list[open_spot][1] = lru_counter;
        open_spot++;
        printf("PAGE_FAULT 1");
    } else{
        printf("PAGE_FAULT 2");
        evict_page(page);
    }
    virt_mem[page].valid = true;
}

void read_addy(int addy){
    int page = addy / 8;
    int local_addr = addy % 8;

    if(virt_mem[page].valid == false){
        page_fault(page);
        printf("READ_ADDY");
    }
    int main_page = virt_mem[page].pp;
    lru_counter++;
    lru_list[main_page][1] = lru_counter;
    printf("%d\n", main_mem[main_page][local_addr]);
}

void write_addy(int addy, int val){
    int page = addy / 8;
    int local_addr = addy % 8;
    disk_mem[page][local_addr] = val;
    if(virt_mem[page].valid == false){
        page_fault(page);
        printf("WRITE_ADDY");
    }
    int main_page = virt_mem[page].pp;
    virt_mem[page].dirty = true;
    main_mem[main_page][local_addr] = val;
    lru_counter ++;
    lru_list[main_page][1] = lru_counter;
    //printf("WRITTEN: %d\n", main_mem[main_page][local_addr]);
}

void init_memory(){
    printf("INIT_MEMORY");
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
    printf("ARG %d %s", argc, *arg);
    if (argc >= 1 && strcmp(arg[1], "LRU") == 0) {
        algo_fifo = false;
        printf("MAIN 1");
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
            printf("MAIN 2");
            continue;
        }
        char *argv[50];
        int num_args = 0;
        while (tmp != NULL) {
            printf("MAIN 3");
            argv[num_args] = strdup(tmp);
            num_args += 1;
            tmp = strtok(NULL, " ");
        }

        // Commands
        if (strcmp(argv[0], "quit") == 0) {
            printf("MAIN 4");
            break;
        } else if (strcmp(argv[0], "showptable") == 0) {
            // argv[1] should be int
            printf("MAIN 5");
            showptable();
        } else {
            if (num_args < 2) {
                printf("Insufficient arguments.\n");
                printf("MAIN 6");
                continue;
            }
            int num = atoi(argv[1]);
            if (strcmp(argv[0], "read") == 0) {
                printf("MAIN 7");
                read_addy(num);
            } else if (strcmp(argv[0], "write") == 0) {
                if (num_args < 3) {
                    printf("MAIN 8");
                    printf("Insufficient arguments for write command.\n");
                    continue;
                }
                int value = atoi(argv[2]);
                write_addy(num, value);
            } else if (strcmp(argv[0], "showmain") == 0) {
                if(num < 4){
                    printf("MAIN 9");
                    showmain(num);
                } else{
                    printf("Invalid page number.\n");
                    printf("MAIN 10");
                }
            } else {
                printf("MAIN 11");
                printf("INVALID COMMAND\n");
            }
        }
    }
    return 0;
}