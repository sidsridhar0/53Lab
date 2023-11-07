//Siddharthen Sridhar(95532627), Rohan Jayasekara(37328564)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INP_SIZE 100

struct Page{
    bool valid;     //if valid, in main memory(pp), else in dp
    bool dirty;     //written to
    int pp;         //physical page address
    // int dp;      //disk page address // not needed
};

struct Page virt_mem[16];   // virtual memory info
int main_mem[4][8];     // "physical main memory"
int disk_mem[16][8];    // "secondary disk storage"

int fifo_list[4][2];    // queue for fifo evicting
int lru_list[4][2];     // keeps track of update history for lru evicting
int lru_counter = 0;    // keeps track of time for lru updates
int open_spot = 0;      // index of open memory before it is full

bool algo_fifo;     // whether it uses FIFO or LRU

void showmain(int page_num) { // print a main memory page
    // printf("SHOWMAIN");
    for (int i = 0; i < 8; i++){
        printf("%d: %d\n", page_num * 8 + i, main_mem[page_num][i]);
    }
}

void showptable() { // print info about all virtual pages
    for (int i = 0; i < 16; i++){
        int pn = i;
        if(virt_mem[i].valid == true){
            pn = virt_mem[i].pp;
            // printf("SHOWPTABLE");
        }
        printf("%d:%d:%d:%d\n", i, virt_mem[i].valid, virt_mem[i].dirty, pn);
    }
}

void load_page(int disk_page, int main_page){ // moves page data from disk page to main memory page
    for(int i = 0; i < 8; i++){
        main_mem[main_page][i] = disk_mem[disk_page][i];
    }
    // printf("LOAD_PAGE");
}

void evict_fifo(int page){      // kick out main memory index in fifo_list[0][0]
    int victim = fifo_list[0][0];
    int virt_page = fifo_list[0][1];
    // printf("EVICTING %d\n", virt_page);
    for(int i = 0; i < 3; i++){     // shift fifo_list entries down
        fifo_list[i][0] = fifo_list[i+1][0];
        fifo_list[i][1] = fifo_list[i+1][1];
    }
    fifo_list[3][0] = victim;
    fifo_list[3][1] = page;
    virt_mem[virt_page].valid = false;  // reset dirty and valid for virtual page kicked out of main mem
    virt_mem[virt_page].dirty = false;
    load_page(page, victim);
    virt_mem[page].pp = victim;     // set the physical page address in the added virtual page
    // printf("EVICT_FIFO");
}

void evict_lru(int page){   // evict least recently used page
    int victim = 0;
    for(int i = 1; i < 4; i++){     // kick out virtual page at main memory index with the lowest update
        if(lru_list[victim][1] > lru_list[i][1]){
            victim = i;
            // printf("EVICT_LRU");
        }
    }
    int virt_page = lru_list[victim][0];
    // printf("EVICTING %d\n", virt_page);
    virt_mem[virt_page].valid = false;
    virt_mem[virt_page].dirty = false;
    load_page(page, victim);
    lru_list[victim][0] = page;
    // lru_list[victim][1] = lru_counter;
    virt_mem[page].pp = victim;
}

void evict_page(int page){      // use either evict_fifo or evict_lru based on whats chosen when script is ran
    if(algo_fifo == true){
        // printf("EVICT_PAGE FIFO");
        evict_fifo(page);
    }else{
        // printf("EVICT_PAGE LRU");
        evict_lru(page);
    }
}

void page_fault(int page){  // load_page in and remove specific page(based on LRU or FIFO)
    printf("A Page Fault Has Occurred\n");
    if(open_spot < 4){      // if there is open main memory just load the page into that space
        load_page(page, open_spot);
        virt_mem[page].pp = open_spot;
        fifo_list[open_spot][0] = open_spot;
        fifo_list[open_spot][1] = page;

        lru_list[open_spot][0] = page;
        lru_list[open_spot][1] = lru_counter;
        open_spot++;
        // printf("PAGE_FAULT 1");
    } else{     // if there is no open main memory, evict a page
        // printf("PAGE_FAULT 2");
        evict_page(page);
    }
    virt_mem[page].valid = true;
}

void read_addy(int addy){   // print value at int address
    int page = addy / 8;    // virtual page to be read
    int local_addr = addy % 8;

    if(virt_mem[page].valid == false){  // page fault if valid bit is 0
        page_fault(page);
        // printf("READ_ADDY");
    }

    int main_page = virt_mem[page].pp;    // main_memory index of virtual page
    lru_counter++;  // update lru counter
    lru_list[main_page][1] = lru_counter;
    printf("%d\n", main_mem[main_page][local_addr]);
}

void write_addy(int addy, int val){ //  writes value into address
    int page = addy / 8;
    int local_addr = addy % 8;
    disk_mem[page][local_addr] = val;   // update the disk memory first
    if(virt_mem[page].valid == false){
        page_fault(page);
        // printf("WRITE_ADDY");
    }

    int main_page = virt_mem[page].pp;
    virt_mem[page].dirty = true;
    main_mem[main_page][local_addr] = val;
    lru_counter ++;
    lru_list[main_page][1] = lru_counter;
    //printf("WRITTEN: %d\n", main_mem[main_page][local_addr]);
}

void init_memory(){     // initialize all memory to -1 and valid/dirty bits to 0
    // printf("INIT_MEMORY");
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
        if(i < 4){
            fifo_list[i][0] = -1;
            fifo_list[i][1] = -1;

            lru_list[i][0] = -1;
            lru_list[i][1] = -1;
        }
    }
    printf("Initialized\n");
}


int main(int argc, char* arg[]){
    algo_fifo = true;
    // printf("ARG %d %s", argc, *arg);
    if (argc > 1 && strcmp(arg[1], "LRU") == 0) {  // Whether it uses LRU or FIFO
        algo_fifo = false;
        // printf("MAIN 1");
    }
    // Init algorithm
    init_memory();

    char inp[INP_SIZE];
    while (true) {
        printf("> ");   // get command
        fflush(stdout);

        // Parse argv from input
        fgets(inp, INP_SIZE, stdin); 
        inp[strlen(inp) - 1] = '\0';
        //printf("%s\n", inp);
        char *tmp = strtok(inp, " ");
        if (tmp == NULL) {
            // printf("MAIN 2");
            continue;
        }

        //tokenize input
        char *argv[50];
        int num_args = 0;
        while (tmp != NULL) {
            // printf("MAIN 3");
            argv[num_args] = strdup(tmp);
            num_args += 1;
            tmp = strtok(NULL, " ");
        }

        // Commands
        if (strcmp(argv[0], "quit") == 0) {
            // printf("MAIN 4");
            break;
        } else if (strcmp(argv[0], "showptable") == 0) {
            // argv[1] should be int
            // printf("MAIN 5");
            showptable();
        } else {
            if (num_args < 2) {
                printf("Insufficient arguments.\n");
                // printf("MAIN 6");
                continue;
            }
            int num = atoi(argv[1]);
            if (strcmp(argv[0], "read") == 0) {
                // printf("MAIN 7");
                read_addy(num);
            } else if (strcmp(argv[0], "write") == 0) {
                if (num_args < 3) {
                    // printf("MAIN 8");
                    printf("Insufficient arguments for write command.\n");
                    continue;
                }
                int value = atoi(argv[2]);
                write_addy(num, value);
            } else if (strcmp(argv[0], "showmain") == 0) {
                if(num < 4){
                    // printf("MAIN 9");
                    showmain(num);
                } else{
                    printf("Invalid page number.\n");
                    // printf("MAIN 10");
                }
            } else {
                // printf("MAIN 11");
                printf("INVALID COMMAND\n");
            }
        }
    }
    return 0;
}