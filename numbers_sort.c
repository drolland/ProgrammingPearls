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

int32_t chunk_value(Chunk* ch) {
    return *(ch->current);
}

int32_t chunk_inc(Chunk* ch) {
    if (ch->current - ch->array >= (ch->size - 1))
        return -1;
    ch->current++;
    return 1;
}

int compare_int32(void const *a, void const *b) {
    const int32_t* pa = a;
    const int32_t* pb = b;

    return *pa - *pb;
}

void chunk_qsort(Chunk* ch) {
    qsort(ch->array, ch->size, sizeof (int32_t), compare_int32);
}

#define CHUNK_SIZE 30

void chunk_test() {

    FILE* f = fopen("heap_list.txt", "r");

    Chunk* chunk = chunk_new(CHUNK_SIZE);

    chunk_load(chunk, f);

    for (int i = 0; i < CHUNK_SIZE + 50; i++) {
        printf("%d\n", chunk_value(chunk));
        if (chunk_inc(chunk) < 0) break;
    }

    int code = chunk_load(chunk, f);
    printf("After file termniated, chunk load = %d\n", code);

    fclose(f);
}


#define NB_CHUNK 9
#define MEMORY 1000000
#undef CHUNK_SIZE
#define CHUNK_SIZE 2500000 /* TODO, test segfault for big numbers */

void disk_merge_sort() {

    int start = g_get_monotonic_time();


    Chunk * chunk;
    FILE* f = fopen("list.txt", "r");
    char file_name[256];
    int data_remaining = 1;
    int k = 0;
    
    chunk = chunk_new(MEMORY);
    while (data_remaining) {

        int code = chunk_load(chunk, f);
        if (code == 0) {
            data_remaining = 0;
            break;
        } else if (code != MEMORY)
            data_remaining = 0;

        chunk_qsort(chunk);

        k++;
        sprintf(file_name, "int_%d.txt", k);
        FILE* f_int = fopen(file_name, "w");
        do {
            fprintf(f_int, "%d\n", chunk_value(chunk));
        } while (chunk_inc(chunk) > 0);
        fclose(f_int);
    }

    fclose(f);
    chunk_free(chunk);

    int nb_chunk = k;

    Chunk * ch[nb_chunk];
    FILE * f_int[nb_chunk];
    int file_has_data[nb_chunk];
    FILE* f_out = fopen("out_list.txt", "w");

    for (int i = 0; i < nb_chunk; i++) {
        ch[i] = NULL;
        sprintf(file_name, "int_%d.txt", i+1);
        f_int[i] = fopen(file_name, "r");
        file_has_data[i] = 1;
    }


    int valid_chunks = 0;

    for (int i = 0; i < nb_chunk; i++) {
        ch[i] = chunk_new(MEMORY / nb_chunk);
        if (chunk_load(ch[i], f_int[i]) != MEMORY / nb_chunk)
            file_has_data[i] = 0;
        chunk_qsort(ch[i]);
        valid_chunks++;
    }


    while (valid_chunks > 0) {

        for (int i = 0; i < nb_chunk && valid_chunks > 0; i++) {

            int min = INT32_MAX;

            int index = -1;

            for (int j = 0; j < nb_chunk; j++)
                if (ch[j] != NULL) {
                    if (chunk_value(ch[j]) < min) {
                        min = chunk_value(ch[j]);
                        index = j;
                    }

                }

            fprintf(f_out, "%d\n", chunk_value(ch[index]));

            if (chunk_inc(ch[index]) < 0) {
                if (file_has_data[index]) {
                    int nb_loaded = chunk_load(ch[index], f_int[index]);
                    if (nb_loaded != MEMORY / nb_chunk) {
                        file_has_data[index] = 0;
                    }
                    if (nb_loaded == 0) {
                        chunk_free(ch[index]);
                        ch[index] = NULL;
                        valid_chunks--;
                    } else
                        chunk_qsort(ch[index]);
                } else {
                    chunk_free(ch[index]);
                    ch[index] = NULL;
                    valid_chunks--;
                }
            }
        }


    }


    for (int i = 0; i < nb_chunk; i++) {
        fclose(f_int[i]);
        sprintf(file_name, "int_%d.txt", i+1);
        remove(file_name);
    }
    
   
    fclose(f_out);


    int stop = g_get_monotonic_time();

    printf("Numbers sorted in %.0f ms", (stop - start) / 1000.0f);
}

