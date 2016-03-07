#include "generate.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <glib.h>

int seed_has_been_initialized = 0;

int get_random_number(BitVector* bit_vector){
    
    if ( !seed_has_been_initialized){
        srand(clock());
        seed_has_been_initialized = 1;
    }
    
    int x = rand() * (float)( bit_vector->max_value ) / RAND_MAX;
    int inc = 0;
    
    if ( rand() < RAND_MAX / 2)
        inc = -1;
    else 
        inc = 1;
    
    while ( bit_vector_get(bit_vector,x) != 0){ 
        x += inc;
        if ( x < 0 )
            x = bit_vector->max_value;
        else if ( x > bit_vector->max_value )
            x = 0;
    }
    
    bit_vector_set(bit_vector,x);
    
    return x;    
    
}

#define MAX_NUMBER 9999999
#define COUNT 8000000

void generate_list(){
    
    int start = g_get_monotonic_time();
    
    BitVector* vector = bit_vector_new(MAX_NUMBER);

    FILE* f = fopen("list.txt","w");
   
    for ( int x = 0; x < COUNT; x++)
        fprintf(f,"%d\n",get_random_number(vector));

    
    fclose(f);
    
    int stop = g_get_monotonic_time();
    
    printf("%d numbers generated in %.0f ms\n",COUNT,(stop-start) / 1000.0f);
    
}