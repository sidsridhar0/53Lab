//Siddharthen Sridhar(95532627), Rohan Jayasekara(37328564)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INP_SIZE 100
#define HEAP_SIZE 127

void malloc_func(int num_bytes){
    return;
}

int realloc_func(int pointer, int num_bytes){
    return 0;
}

void free_func(int pointer){
    return;
}

void blocklist(){
    return;
}

void writemem(int pointer, char* vals){

}

void memcheck(int pointer){
    return;
}

void printmem(int pointer, int num_bytes){
    return;
}

int main(){
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
            continue;
        }

        //tokenize input
        char *argv[50];
        int num_args = 0;
        while (tmp != NULL) {
            argv[num_args] = strdup(tmp);
            num_args += 1;
            tmp = strtok(NULL, " ");
        }

        // Commands
        if(num_args == 1){
            if (strcmp(argv[0], "quit") == 0){           // 0 args 
                break;
            }else if(strcmp(argv[0], "blocklist") == 0){ // 0 args
                blocklist();
            }
        }else if(num_args == 2){
            if(strcmp(argv[0], "memcheck") == 0){        // 1 args
                memcheck(argv[1]);
            }else if(strcmp(argv[0], "free") == 0){      // 1 args
                free_func(argv[1]);
            }else if(strcmp(argv[0], "malloc") == 0){    // 1 args
                malloc_func(argv[1]);
            }
        }else if(num_args == 3){
        if(strcmp(argv[0], "printmem") == 0){           // 2 args
            printmem(argv[1], argv[2]);
        }else if(strcmp(argv[0], "writemem") == 0){     // 2 args
            writemem(argv[1], argv[2]);
        }else if(strcmp(argv[0], "realloc") == 0){      // 2 args
            realloc_func(argv[0], argv[1]);
        }
    }
}

