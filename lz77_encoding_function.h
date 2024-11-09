#ifndef LZ77_ENCODING_FUNCTION_H
#define LZ77_ENCODING_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./pnm_library_01_2022/libpnm.h"

// 定义匹配的最大长度
#define MAX_MATCH_LENGTH 255

// 函数声明
void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size, float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr);

// 计算平均值和标准差的辅助函数
void calculate_statistics(int *data, long int count, float *avg, float *std);

#endif