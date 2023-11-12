//Siddharthen Sridhar(95532627), Rohan Jayasekara(37328564)
/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INP_SIZE 100
#define HEAP_SIZE 127

unsigned char memory[HEAP_SIZE];

void init_memory(){
    unsigned char header_size = 127;
    header_size = header_size << 1;
    memory[0] = header_size;
    for(int i = 1; i < HEAP_SIZE; ++i){
        memory[i] = 0;
    }
}

void malloc_func(int num_bytes){
    int i = 0;
    while((i < HEAP_SIZE) && ((memory[i] & 1) || ((memory[i] >> 1) <= num_bytes))){ //i is inbounds AND (i is allocated OR not enough space in block)
        i = i + (memory[i] >> 1);   //move to next block
    }
    if(i >= HEAP_SIZE){
        printf("Not enough space\n");
    }else{
        int header_size = memory[i] >> 1;
        if(num_bytes < header_size - 1){
            int next_header = i + num_bytes + 1;
            memory[next_header] = header_size - (num_bytes + 1);
            memory[next_header] = memory[next_header] << 1;
        }
        memory[i] = num_bytes + 1;
        memory[i] = memory[i] << 1;
        memory[i] += 1;
        printf("%d\n", i+1);
    }
}

void free_func(int pointer){
    int header = pointer - 1;
    int header_size = (memory[header] >> 1);
    int next_header = header + header_size;

    while(next_header < HEAP_SIZE && !(memory[next_header] & 1)){
        memory[header] = header_size + (memory[next_header] >> 1);
        memory[header] = memory[header] << 1;
        header_size = (memory[header] >> 1);
        next_header = header + header_size;
    }
    if(memory[header] & 1){
        memory[header] -= 1;
    }
}

void blocklist(){
    int i = 0;
    int count = 0;
    while(i < HEAP_SIZE && count <= HEAP_SIZE + 1){
        if((memory[i] & 1) == 1){
            printf("%d, %d, allocated\n", i+1, (memory[i] >> 1) - 1);
        } else{
            printf("%d, %d, free\n", i+1, (memory[i] >> 1) - 1);
        }
        i += memory[i] >> 1; 
        count++;
    }
}

void writemem(int pointer, char* vals){
    int len_vals = 0;
    while(vals[len_vals] != '\0'){
        memory[pointer + len_vals] = vals[len_vals];
        len_vals++;
    }
}

void printmem(int pointer, int num_bytes){
    for(int i = 0; i < num_bytes; ++i){
        //convert to hexadecimal from ASCII
        int tens = memory[pointer + i] / 16;
        int ones = memory[pointer + i] % 16; //! Account for A-F for 10-15
        if(ones < 10){
            printf("%d%d ", tens, ones);
        }else{
            char ones_char = 65 + (ones - 10);
            printf("%d%c ", tens, ones_char);
        }
    }
    printf("\n");
}

int realloc_func(int pointer, int num_bytes){
    //check if smaller
    int header = pointer - 1;
    int header_size = memory[header] >> 1;
    if(header_size - 1 == num_bytes){
        return header + 1;
    }
    else if(header_size - 1 > num_bytes){
        //if smaller remove excess
        //for new smaller block
        memory[header] = num_bytes + 1;
        memory[header] = memory[header] << 1;
        memory[header] += 1;
        
        // freed portion
        int next_header = header + num_bytes + 1;
        memory[next_header] = header_size - (num_bytes + 1);
        memory[next_header] = memory[next_header] << 1;
    return header + 1;
    //}else{
        // int next_header = header + header_size;
        // if(!(memory[next_header] & 1) && (header_size + (memory[next_header]>>1))){
        //     memory[header] = num_bytes + 1;
        //     memory[header] = memory[header] << 1;
        //     memory[header] += 1;
            
        //     // freed portion
        //     int next_header = header + num_bytes + 1;
        //     memory[next_header] = header_size - (num_bytes + 1);
        //     memory[next_header] = memory[next_header] << 1;
        //     //change this header
        //     //add new free block header(make sure doesnt overwrite)
        // }
    }
    //check if smaller
        //if smaller remove excess
    //else
        //see if can add more space after
            //add excess
        //else see if enough space for new block
            //malloc block, free curr block
        //else coalesce
            //see if can relocate block
                //malloc block, free curr block
            //else return
    return 0;
}

int main(){
    char inp[INP_SIZE];
    init_memory();
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
            //set argv[2] to int
            int arg1 = atoi(argv[1]);

            if(strcmp(argv[0], "free") == 0){            // 1 args
                free_func(arg1);
            }else if(strcmp(argv[0], "malloc") == 0){    // 1 args
                malloc_func(arg1);
            }
        }else if(num_args == 3){
            //set argv[2] and argv[3] to ints
            int arg1 = atoi(argv[1]);

            if(strcmp(argv[0], "printmem") == 0){           // 2 args
                int arg2 = atoi(argv[2]);
                printmem(arg1, arg2);
            }else if(strcmp(argv[0], "writemem") == 0){     // 2 args
                writemem(arg1, argv[2]);
            }else if(strcmp(argv[0], "realloc") == 0){      // 2 args
                int arg2 = atoi(argv[2]);
                realloc_func(arg1, arg2);
            }
        }
    }
}

