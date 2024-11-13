#include "./mean_absolute_error.h"

float mean_absolute_error(char *file_name_1_ptr, char *file_name_2_ptr){
    struct PGM_Image pc1, pc2;
    load_PGM_Image(&pc1, file_name_1_ptr);
    load_PGM_Image(&pc2, file_name_2_ptr);

    if(pc1.height != pc2.height || pc1.width != pc2.width ){
        free_PGM_Image(&pc1);
        free_PGM_Image(&pc2);
        printf("image with different height \n");
        return -1;
    }

    int mg1 = pc1.maxGrayValue;
    int mg2 = pc2.maxGrayValue;
    float mg_scale = 1.0;
    if(pc1.maxGrayValue != pc2.maxGrayValue)
        mg_scale = mg1 / mg2;
    printf("mg scale %f \n", mg_scale);

    float total_error = 0.0;
    for (int i=0; i<pc1.height; i++){
        for (int j=0; j<pc1.width; j++){
            int pix1 = pc1.image[i][j];
            int pix2 = pc2.image[i][j];
            float pix_error = fmax((float)pix1,(float)pix2*mg_scale)-fmin((float)pix1,(float)pix2*mg_scale);
            total_error += pix_error;
        }
    }

    printf("total error %f \n", total_error);

    total_error = total_error/(pc1.height*pc1.width);
    return total_error;
}