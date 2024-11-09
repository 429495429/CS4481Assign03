#include <stdio.h>
#include <stdlib.h>
#include "pnm_library_01_2022/libpnm.h"
#include "lz77_encoding_function.h"

int main(int argc, char *argv[]){
    printf("A ha! \n");
    if (argc != 3) {
        printf("Please input in format: %s <imagename> <filename>\n", argv[0]);
        return 1;
    }
    char *filename = argv[1];
    unsigned int buffersize = atoi(argv[2]);
    float avg_offset, std_offset, avg_length, std_length;
    Encode_Using_LZ77(filename,buffersize,&avg_offset,&std_offset,&avg_length,&std_length);

    printf("offset avg %f std %f", avg_offset, std_offset);
    printf("length avg %f std %f", avg_length, std_length);

}