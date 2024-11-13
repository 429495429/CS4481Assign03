#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pnm_library_01_2022/libpnm.h"
#include "lz77_encoding_function.h"

int main(int argc, char *argv[]){
    printf("A ha! \n");
    if (argc != 3) {
        printf("Please input in format: %s <imagename> <filename>\n", argv[0]);
        return 1;
    }
    clock_t start, end;
    double cpu_time_used;

    char *filename = argv[1];
    unsigned int buffersize = atoi(argv[2]);
    float avg_offset, std_offset, avg_length, std_length;

    start = clock();  // time record start
    Encode_Using_LZ77(filename,buffersize,&avg_offset,&std_offset,&avg_length,&std_length);
    end = clock();// time record end
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;  
    printf("encoding time %f seconds\n", cpu_time_used);

    printf("offset avg %f std %f \n", avg_offset, std_offset);
    printf("length avg %f std %f \n", avg_length, std_length);

}