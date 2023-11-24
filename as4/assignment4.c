//Siddharthen Sridhar(95532627), Rohan Jayasekara(37328564)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INP_SIZE 100
#define HEAP_SIZE 127

unsigned char memory[HEAP_SIZE];

void init_memory(){
    //init memory array to all 0s except for index 0 as header of free block 126
    unsigned char header_size = 127;
    header_size = header_size << 1;
    memory[0] = header_size;
    for(int i = 1; i < HEAP_SIZE; ++i){
        memory[i] = 0;
    }
}

int malloc_func(int num_bytes){
    /*
    This operation allows the user to allocate a block of memory from your heap. This
    operation should take one argument, the number of bytes which the user wants in the payload
    of the allocated block. This operation should print out a pointer which is the first address of the
    payload of the allocated block
    */
    int i = 0;
    int best_fit = -1;
    int best_size = -1;
    while(i < HEAP_SIZE){ // ! CHANGED TO BEST FIT NOT FIRST FIT
        if(!(memory[i] & 1) && ((memory[i] >> 1) > num_bytes)){
            if(best_fit == -1 || (memory[i] >> 1) < best_size){
                best_fit = i;
                best_size = (memory[i] >> 1);
            }
        }
        i = i + (memory[i] >> 1);   //move to next block
    }
    if(best_fit == -1){
        printf("Not enough space\n");
        return -1;
    } else{
        i = best_fit;
        int header_size = memory[i] >> 1;
        if(num_bytes < header_size - 1){
            int next_header = i + num_bytes + 1;
            if(next_header < HEAP_SIZE){
                memory[next_header] = header_size - (num_bytes + 1);
                memory[next_header] = memory[next_header] << 1;
            }
        }
        memory[i] = num_bytes + 1;
        memory[i] = memory[i] << 1;
        memory[i] += 1;
        printf("%d\n", i+1);
        return i + 1;
    }

    // while((i < HEAP_SIZE) && ((memory[i] & 1) || ((memory[i] >> 1) <= num_bytes))){ //i is inbounds AND (i is allocated OR not enough space in block)
    //     i = i + (memory[i] >> 1);   //move to next block
    // }
    // if(i >= HEAP_SIZE){
    //     printf("Not enough space\n");
    //     return -1;
    // }else{
    //     int header_size = memory[i] >> 1;
    //     if(num_bytes < header_size - 1){
    //         int next_header = i + num_bytes + 1;
    //         if(next_header < HEAP_SIZE){
    //             memory[next_header] = header_size - (num_bytes + 1);
    //             memory[next_header] = memory[next_header] << 1;
    //         }
    //     }
    //     memory[i] = num_bytes + 1;
    //     memory[i] = memory[i] << 1;
    //     memory[i] += 1;
    //     printf("%d\n", i+1);
    //     return i + 1;
    // }
}

void free_func(int pointer){
    /*
    This operation allows the user to resize a previously allocated block of memory. The
    operation takes two arguments. The first argument is a pointer to the payload of a previously
    allocated block of memory, and the second argument is the new size in bytes that the user
    wants for the payload of that block. If the new size is smaller than the current size of the block,
    your program should truncate the block and free any excess memory. If the new size is larger
    and there is sufficient adjacent free space in the heap, your program should expand the block
    into the adjacent free space. If there is not enough adjacent free space, your program should
    allocate a new block of the appropriate size, copy the contents of the old block to the new
    block, free the old block, and return a pointer to the payload of the new block. You can assume
    that the argument is a correct pointer to the payload of an allocated block.
    */
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
    /*
    This operation prints out information about all of the blocks in your heap. The
    information about blocks should be printed in the order that the blocks are contained in the
    heap. The following information should be printed about each block: pointer to the payload,
    block size, and the allocation status (allocated of free). All three items of information about a
    single block should be printed on a single line and should be separated by commas
    */
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
    if(count > HEAP_SIZE + 1){
        printf("ERROR: Wrote to header byte\n");
    }
}

void writemem(int pointer, char* vals){
    /*
    This operation writes alpha-numeric characters into memory. The operation takes
    two arguments and there should be no empty spaces or null character at the end of the input.
    The first argument is a pointer to the location in memory and the second argument is a
    sequence of alpha-numeric characters which will be written into memory, starting at the
    address indicated by the pointer. The first character will be written into the address indicated by
    the pointer, and each character thereafter will be written into the neighboring addresses
    sequentially. For example, the operation “writemem 3 abc” will write an ‘a’ into address 3,
    a ‘b’ into address ‘4’, and a ‘c’ into address 5.
    You can assume that the pointer argument will always be an address in the heap, assume that
    all of the characters will be written into addresses in the heap
    */
    int len_vals = 0;
    while(vals[len_vals] != '\0'){
        memory[pointer + len_vals] = vals[len_vals];
        len_vals++;
    }
}

void printmem(int pointer, int num_bytes){
    /*
    This operation prints out a segment of memory in hexadecimal. The operation
    takes two arguments. The first argument is a pointer to the first location in memory to print,
    and the second argument is an integer indicating how many addresses to print. The contents of
    all addresses will be printed on a single line and separated by a single space.
    */
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
    /*
    This operation allows the user to resize a previously allocated block of memory. The
    operation takes two arguments. The first argument is a pointer to the payload of a previously
    allocated block of memory, and the second argument is the new size in bytes that the user
    wants for the payload of that block. If the new size is smaller than the current size of the block,
    your program should truncate the block and free any excess memory. If the new size is larger
    and there is sufficient adjacent free space in the heap, your program should expand the block
    into the adjacent free space. If there is not enough adjacent free space, your program should
    allocate a new block of the appropriate size, copy the contents of the old block to the new
    block, free the old block, and return a pointer to the payload of the new block. You can assume
    that the argument is a correct pointer to the payload of an allocated block.
    */
    int header = pointer - 1;
    int header_size = memory[header] >> 1;
    int tmp = header_size;
    if(header_size - 1 == num_bytes){       //check if same size
        printf("%d\n", pointer);
        return pointer;
    }
    else if(header_size - 1 > num_bytes){   //check if smaller
        //if smaller remove excess
        //for new smaller block
        memory[header] = num_bytes + 1;
        memory[header] = memory[header] << 1;
        memory[header] += 1;
        
        // freed portion
        int next_header = header + num_bytes + 1;
        memory[next_header] = header_size - (num_bytes + 1);
        memory[next_header] = memory[next_header] << 1;
        printf("%d \n", pointer);
        return pointer;
    }else{                                  //check if larger
        //!COALESCE
        int next_header = header + header_size;
        while(next_header < HEAP_SIZE && !(memory[next_header] & 1) && (header_size - 1 < num_bytes)){  //next header not oob, is free and smaller than num_bytes
            memory[header] = header_size + (memory[next_header] >> 1);
            memory[header] = memory[header] << 1;
            memory[header] += 1;
            header_size = (memory[header] >> 1);
            next_header = header + header_size;
        }
        if(header_size - 1 < num_bytes){
            int new_spot = malloc_func(num_bytes);
            if(new_spot != -1){
                for(int i = 0; i < tmp; i++){
                    memory[new_spot + i] = memory[pointer + i];
                }
            }
            free_func(pointer);
            return pointer;
        }else{
            if(header_size - 1 > num_bytes){
                memory[header] = num_bytes + 1;
                memory[header] = memory[header] << 1;
                memory[header] += 1;
                
                // freed portion
                int next_header = header + num_bytes + 1;
                memory[next_header] = header_size - (num_bytes + 1);
                memory[next_header] = memory[next_header] << 1;
            }
            printf("%d \n", pointer);
            return pointer;
        }
    }
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
                int temp = malloc_func(arg1);
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
