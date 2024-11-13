#include <stdio.h>
#include <stdlib.h>

#include "pnm_library_01_2022/libpnm.c"
#include "./mean_absolute_error.h"

int main(int argc, char *argv[]){
    printf("A ha! \n");
    if (argc != 3) {
        printf("Please input in format: %s <filename> <imagename> \n", argv[0]);
        return 1;
    }
    char *pc1name = argv[1];
    char *pc2name = argv[2];

    printf("picture1 name %s \n",pc1name);
    printf("picture2 name %s \n",pc2name);

    float mae = mean_absolute_error(pc1name,pc2name);

    printf("The mae between these pics is %f \n", mae);
    return 0;
}