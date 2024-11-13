#include <stdio.h>
#include <stdlib.h>
#include "pnm_library_01_2022/libpnm.h"
#include "lz77_decoding_function.h"

int main(int argc, char *argv[]){
    printf("A ha! \n");
    if (argc != 2) {
        printf("Please input in format: %s <filename> \n", argv[0]);
        return 1;
    }
    char *filename = argv[1];
    Decode_Using_LZ77(filename);
}