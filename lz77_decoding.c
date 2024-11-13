#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pnm_library_01_2022/libpnm.h"
#include "lz77_decoding_function.h"

int main(int argc, char *argv[]){
    printf("A ha! \n");
    if (argc != 2) {
        printf("Please input in format: %s <filename> \n", argv[0]);
        return 1;
    }
    clock_t start, end;
    double cpu_time_used;

    char *filename = argv[1];
    start = clock();  // time record start
    Decode_Using_LZ77(filename);
    end = clock();// time record end
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;  
    printf("decoding time %f seconds\n", cpu_time_used);
}