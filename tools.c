#include <stdlib.h>
#include <malloc.h>
#include <glib.h>

#include "tools.h"

#define WARMUP_SIZE 25000000

void tools_warmup(){
    float *a,*b,*c;
    a = malloc(WARMUP_SIZE * sizeof(float));
    b = malloc(WARMUP_SIZE * sizeof(float));
    c = malloc(WARMUP_SIZE * sizeof(float));
    
    for(int i = 0; i < WARMUP_SIZE;i++){
        a[i] = rand() * 1000.0f / RAND_MAX;
        b[i] = rand() * 1000.0f / RAND_MAX;
    }
    
    for(int i = 0; i < WARMUP_SIZE;i++)
        c[i] = a[i] * b[i];
    
             
    free(a);
    free(b);
    free(c);
    
    
}

void tools_print_memusage(){
    struct mallinfo info =  mallinfo();
    printf("user memory %.2f Mo, %.2f Mo with mmap\n",info.arena / 1000000.0f,info.hblkhd / 1000000.0f);
}
