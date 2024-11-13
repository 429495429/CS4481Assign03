#include "lz77_encoding_function.h"

void LZ77_encode(unsigned char *pgm_image_data, long int *imageData_index, unsigned int max_buffer_size, long int imageData_size,
                 int *offset, int *offset_length, unsigned char *next_symbols, long int *encodeData_index){
    // initialize window of 0 length
    int offset_count = 0, length_count = 0;  
    unsigned char next_symbol = pgm_image_data[(*imageData_index)];

    
    // traverse elements in the buffer with max_buffer_size backwordly
    for (long int buffer_index = (*imageData_index) - 1; buffer_index >= 0 && buffer_index > (*imageData_index)-max_buffer_size ; buffer_index--) {
        int maxMatchLength = 0;
        int window_index=0;
        // printf("tracking 002 %ld \n ", buffer_index);
        //traverse image started at the index
        while ( pgm_image_data[buffer_index + (window_index % ((*imageData_index) - buffer_index))] == pgm_image_data[(*imageData_index) + maxMatchLength]
                // pgm_image_data[buffer_index+window_index] == pgm_image_data[(*imageData_index)+maxMatchLength] //if the window match, keep while
                && (*imageData_index)+maxMatchLength < imageData_size-1 //if reach end of image, end while
                ) {
            maxMatchLength++;
            window_index++;

            if(buffer_index+maxMatchLength == (*imageData_index)) //if the window reach the index of image, restart traverse in window
                break;
        }
        if(maxMatchLength>length_count){
            offset_count = (*imageData_index)-buffer_index;
            length_count = maxMatchLength;
            next_symbol = pgm_image_data[(*imageData_index)+maxMatchLength];

        }
        //if reach the end of image, end searching in buff
        if((*imageData_index)+length_count >= imageData_size-1)
            break;
    }
    offset[(*encodeData_index)] = offset_count;
    offset_length[(*encodeData_index)] = length_count;
    next_symbols[(*encodeData_index)] = next_symbol;
    (*imageData_index) += (length_count+1);


}

// LZ77 编码函数实现
void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size, float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr) {
    char outputFilename[256];
    snprintf(outputFilename, sizeof(outputFilename), "%s.%u.lz", in_PGM_filename_Ptr, searching_buffer_size);
    FILE *outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        perror("Error creating output file");
        return;
    }

    struct PGM_Image pic;
    load_PGM_Image(&pic,in_PGM_filename_Ptr);
    int width = pic.width;
    int height = pic.height;
    int maxGrayValue = pic.maxGrayValue;

    // head tags of the encode data
    fwrite(&height,sizeof(int),1,outputFile);//save the height
    printf("height: %d \n", height);
    fwrite(&width,sizeof(int),1,outputFile);//save the width
    printf("width: %d \n", width);
    fwrite(&maxGrayValue,sizeof(int),1,outputFile);//save the max gray value
    printf("mg: %d \n", maxGrayValue);

    //variables for encoding
    long int imageSize = width * height;
    unsigned char *imageData = (unsigned char*)malloc(imageSize*sizeof(unsigned char));
    long int imageIndex = 0;
    for(int row = 0; row < height; row++){
        for(int col = 0; col <width; col++){
            imageData[imageIndex]=pic.image[row][col];
            imageIndex++;
        }
    }
    imageIndex = 0;
    int *offsets = (int *)malloc(imageSize * sizeof(int));
    int *match_lengths = (int *)malloc(imageSize * sizeof(int));
    unsigned char *next_symbols = (unsigned char *)malloc(imageSize * sizeof(unsigned char));
    long int encodeIndex = 0;
    
    // encode
    while(imageIndex < imageSize){
        LZ77_encode(imageData,&imageIndex,searching_buffer_size,imageSize,offsets,match_lengths,next_symbols,&encodeIndex);
        encodeIndex++;
    }
    
    fwrite(&encodeIndex,sizeof(long int),1,outputFile);
    fwrite(offsets,sizeof(int),encodeIndex,outputFile);
    fwrite(match_lengths,sizeof(int),encodeIndex,outputFile);
    fwrite(next_symbols,sizeof(unsigned char),encodeIndex,outputFile);//save the encoded data
    printf("success save codes %ld \n", encodeIndex);
    fclose(outputFile);

    char offsetsCSVFilename[256];
    snprintf(offsetsCSVFilename, sizeof(offsetsCSVFilename), "%s.%u.offsets.csv", in_PGM_filename_Ptr, searching_buffer_size);
    FILE *offsetsCSVFile = fopen(offsetsCSVFilename, "wb");
    if (!offsetsCSVFile) {
        perror("Error creating output file");
        return;
    }
    long int *offsetsFrequency = (long int *)calloc(searching_buffer_size, sizeof(long int));
    for(long int i=0; i<encodeIndex; i++){
        offsetsFrequency[offsets[i]]++;
    }
    for(long int i=0; i<searching_buffer_size; i++){
        if (offsetsFrequency[i] != 0) { // 只写入非零值
            fprintf(offsetsCSVFile, "%ld, %ld\n", i, offsetsFrequency[i]);
        }
    }
    fclose(offsetsCSVFile);

    char lengthsCSVFilename[256];
    snprintf(lengthsCSVFilename, sizeof(lengthsCSVFilename), "%s.%u.lengths.csv", in_PGM_filename_Ptr, searching_buffer_size);
    FILE *lengthsCSVFile = fopen(lengthsCSVFilename, "wb");
    if (!lengthsCSVFile) {
        perror("Error creating output file");
        return;
    }
    long int *lengthsFrequency = (long int *)calloc(searching_buffer_size, sizeof(long int));
    for(long int i=0; i<encodeIndex; i++){
        lengthsFrequency[match_lengths[i]]++;
    }
    for(long int i=0; i<searching_buffer_size; i++){
        if (lengthsFrequency[i] != 0) { // 只写入非零值
            fprintf(lengthsCSVFile, "%ld, %ld\n", i, lengthsFrequency[i]);
        }
    }
    fclose(offsetsCSVFile);

    // statistics
    calculate_statistics(offsets, encodeIndex, avg_offset_Ptr, std_offset_Ptr);
    calculate_statistics(match_lengths, encodeIndex, avg_length_Ptr, std_length_Ptr);

    // free
    free(imageData);
    free(offsets);
    free(match_lengths);
    free(next_symbols);
}

// 统计数据计算函数实现
void calculate_statistics(int *data,long int count, float *avg, float *std) {
    if (count == 0) return;

    float sum = 0;
    for (long int i = 0; i < count; i++) {
        sum += data[i];
    }
    *avg = sum / count;

    float variance = 0;
    for (long int i = 0; i < count; i++) {
        variance += (data[i] - *avg) * (data[i] - *avg);
    }
    *std = sqrt(variance / count);
}