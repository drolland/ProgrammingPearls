#include "bit_vector.h"
#include <stdlib.h>


BitVector* bit_vector_new(int max_value){
    BitVector* bit_vector;
    bit_vector = malloc(sizeof(BitVector));
    bit_vector->max_value = max_value;
    bit_vector->vector = calloc((max_value / 32 + 1),sizeof(int32_t));
    return bit_vector;
}

void bit_vector_set(BitVector* bit_vector,int value){
    
    int div = value / 32;
    int remainder = value % 32; 
    
    int* p = &(bit_vector->vector[div]);
    *p = *p | ( 1 << (remainder)); 
}

int bit_vector_get(BitVector* bit_vector,int value){
        
    return bit_vector->vector[value / 32] & ( 1 << (value % 32));
}