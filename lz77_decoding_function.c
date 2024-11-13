#include "./lz77_decoding_function.h"

void convert_token_to_image(unsigned char *encode_image_data, long int *encode_image_index, int *offsets, int *length, unsigned char *next_symbols, long int token_index){
    if(offsets[token_index]==0){
        encode_image_data[(*encode_image_index)] = next_symbols[token_index];
        (*encode_image_index)++;
        // printf("token %ld image %d %ld \n", token_index, next_symbols[token_index], (*encode_image_index));
        return;
    }

    int match_length = length[token_index];
    long int input_image_index = (*encode_image_index);
    long int window_index = input_image_index-offsets[token_index];
    for(int i=0; i<match_length;i++){
        encode_image_data[(*encode_image_index)] = encode_image_data[window_index+i%offsets[token_index]];
        (*encode_image_index)++;
        // printf("token %ld image %d %ld \n", token_index, encode_image_data[(*encode_image_index)-1], (*encode_image_index));
        
    }
    encode_image_data[(*encode_image_index)] = next_symbols[token_index];
    (*encode_image_index)++;
    // printf("token %ld image %d %ld \n", token_index, encode_image_data[(*encode_image_index)-1], (*encode_image_index));
    
    return;
}

void Decode_Using_LZ77(char *in_compressed_filename_Ptr){
    FILE *compressed_photo = fopen(in_compressed_filename_Ptr,"r+");
    if (compressed_photo == NULL) {
        printf("failed to open the file \n");   
        exit(1);
    }
    int image_height, image_width, max_gray_value, imageSize;
    long int number_of_tokens;
    printf(" %s \n",in_compressed_filename_Ptr);
    fread(&image_height,sizeof(int),1,compressed_photo);//save the height
    printf("height: %d \n", image_height);
    fread(&image_width,sizeof(int),1,compressed_photo);//save the width
    printf("width: %d \n", image_width);
    imageSize = image_height*image_width;
    fread(&max_gray_value,sizeof(int),1,compressed_photo);//save the max gray value
    printf("mg: %d \n", max_gray_value);
    fread(&number_of_tokens,sizeof(long int),1,compressed_photo);
    printf("lenghth of token: %ld \n", number_of_tokens);

    int *offsets = (int *)malloc(number_of_tokens * sizeof(int));
    int *match_lengths = (int *)malloc(number_of_tokens * sizeof(int));
    unsigned char *next_symbols = (unsigned char *)malloc(number_of_tokens * sizeof(unsigned char));

    int temp = 0;
    unsigned char c;
    for(long int i=0; i<number_of_tokens; i++){
        fread(&temp,sizeof(int),1,compressed_photo);
        offsets[i] = temp;
    }

    for(long int i=0; i<number_of_tokens; i++){
        fread(&temp,sizeof(int),1,compressed_photo);
        match_lengths[i]=temp;
    }

    for(long int i=0; i<number_of_tokens; i++){
        fread(&c,sizeof(unsigned char),1,compressed_photo);
        next_symbols[i]=c;
    }

    char outputFilename[256];
    snprintf(outputFilename, sizeof(outputFilename), "%s.pgm", in_compressed_filename_Ptr);
    printf("generate decoded image on %s\n", outputFilename);
    struct PGM_Image PGMpc;
    create_PGM_Image(&PGMpc, image_width, image_height, max_gray_value);
    PGMpc.height = image_height;
    PGMpc.width = image_width;
    PGMpc.maxGrayValue = max_gray_value;
    unsigned char *encode_image_data = (unsigned char *)malloc(imageSize * sizeof(unsigned char));
    long int image_index = 0;
    for(int i=0; i<number_of_tokens; i++){
        convert_token_to_image(encode_image_data,&image_index,offsets,match_lengths,next_symbols,i);
    }
    image_index = 0;
    for(int i=0; i<PGMpc.height;i++){
        for(int j=0; j<PGMpc.width;j++){
            PGMpc.image[i][j] = encode_image_data[image_index];
            // printf("%d ",PGMpc.image[i][j]);
            image_index++;
        }
        // printf("\n");
    }
    save_PGM_Image(&PGMpc, outputFilename, false);
    free_PGM_Image(&PGMpc);
    free(offsets);
    free(match_lengths);
    free(next_symbols);
    free(encode_image_data);

}