#pragma once

#include <sys/types.h>

typedef struct s_bit_vector {
    int32_t max_value;
    int32_t* vector;
} BitVector;

BitVector* bit_vector_new(int max_value);

void bit_vector_set(BitVector* bit_vector,int value);

int bit_vector_get(BitVector* bit_vector,int value);

