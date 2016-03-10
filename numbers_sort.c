#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <glib.h>
#include <malloc.h>
#include <cairo.h>
#include <math.h>

#include "tools.h"

/* ------------- CHUNK OF DATA ---------------
 */

typedef struct s_file_chunk {
    int size;
    int32_t* array;
    int32_t* current;
} Chunk;

Chunk* chunk_new(int size) {
    assert(size > 0);
    Chunk* new = malloc(sizeof (Chunk));
    new->size = size;
    new->array = malloc(sizeof (int32_t) * size);
    new->current = new->array;
    return new;
}

void chunk_free(Chunk* ch) {
    free(ch->array);
    free(ch);
}

int chunk_load(Chunk* chunk, FILE* file) {
    assert(chunk != NULL);
    int i = 0;
    for (i = 0; i < chunk->size; i++) {
        int code = fscanf(file, "%d\n", &(chunk->array[i]));
        if (code != 1) {
            if (code != EOF)
                printf("An error occured parsing the file\n");
            chunk->size = i;
            break;
        }

    }
    chunk->current = chunk->array;
    return chunk->size;
}

void chunk_print(Chunk* chunk) {
    for (int i = 0; i < chunk->size; i += 10) {
        for (int j = 0; j + i < i + 10 && j + i < chunk->size; j++) {
            printf("%d\t", chunk->array[i + j]);
        }
        printf("\n");
    }
}

static inline int32_t chunk_value(Chunk* ch){
    return *(ch->current);
}

static inline int32_t chunk_inc(Chunk* ch) {
    if (ch->current - ch->array >= ( ch->size -1 ) )
        return -1;
    ch->current++;
    return 1;
}

static int compare_int32(void const *a, void const *b) {
    const int32_t* pa = a;
    const int32_t* pb = b;

    return *pa - *pb;
}

void chunk_qsort(Chunk* ch) {
    qsort(ch->array, ch->size, sizeof (int32_t), compare_int32);
}

#define CHUNK_SIZE 25000

void chunk_test(){
    
    FILE* f = fopen("list.txt", "r");
    
    Chunk* chunk = chunk_new(CHUNK_SIZE);
    
    chunk_load(chunk,f);
    
    for(int i = 0; i < CHUNK_SIZE + 50; i++){
        printf("%d\n",chunk_value(chunk));
        if ( chunk_inc(chunk) < 0 ) break;
    }
    
    fclose(f);
}


#define NB_CHUNK 9
#define CHUNK_SIZE 25000

void disk_merge_sort() {

    int start = g_get_monotonic_time();

    Chunk * ch[NB_CHUNK];

    FILE* f = fopen("list.txt", "r");
    FILE* f_out = fopen("out_list.txt", "w");

    int data_remaining = 1;

    for (int i = 0; i < NB_CHUNK; i++)
        ch[i] = NULL;

    int valid_chunks = 0;

    for (int i = 0; i < NB_CHUNK && data_remaining == 1; i++) {
        ch[i] = chunk_new(CHUNK_SIZE);
        if (chunk_load(ch[i], f) != CHUNK_SIZE)
            data_remaining = 0;
        chunk_qsort(ch[i]);
        valid_chunks++;
    }


    int count = 0;
    
    while (valid_chunks > 0) {
        
        for (int i = 0; i < NB_CHUNK; i++) {
            
            
            
            int min = INT32_MAX;
            int index = -1;
            
            for (int j = 0; j < NB_CHUNK; j++)
                if (ch[j] != NULL) {
                    if (chunk_value(ch[j]) < min) {
                        min = chunk_value(ch[j]);
                        index = j;
                    }
                }
            
            fprintf(f_out, "%d\n", chunk_value(ch[index]));
            
            if (chunk_inc(ch[index]) < 0) {
                if (data_remaining) {
                    if (chunk_load(ch[index], f) != CHUNK_SIZE)
                        data_remaining = 0;
                    chunk_qsort(ch[index]);
                } else {
                    chunk_free(ch[index]);
                    ch[index] = NULL;
                    valid_chunks--;
                }
            }
        }
        
        
    }


    fclose(f);
    fclose(f_out);


    int stop = g_get_monotonic_time();

    printf("Numbers sorted in %.0f ms", (stop - start) / 1000.0f);
}

