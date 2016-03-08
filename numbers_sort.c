#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <glib.h>

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

int load_chunk(Chunk* chunk, FILE* file) {
    assert(chunk != NULL);
    int i = 0;
    for (i = 0; i < chunk->size; i++) {
        int code = fscanf(file, "%d\n", &(chunk->array[i]));
        if (code != 1) {
            if (code != EOF)
                printf("An error occured parsing the file\n");
            return i;
        }
    }
    return i;
}

void chunk_print(Chunk* chunk) {
    for (int i = 0; i < chunk->size; i += 10) {
        for (int j = 0; j + i < i + 10 && j + i < chunk->size; j++) {
            printf("%d\t", chunk->array[i + j]);
        }
        printf("\n");
    }
}

int32_t chunk_pop(Chunk* ch) {
    if (ch->current - ch->array > (ch->size - 1) * sizeof (int32_t))
        return -1;
    int32_t val = *(ch->current);
    ch->current++;
    return val;
}

static int compare_int32(void const *a, void const *b) {
    const int32_t* pa = a;
    const int32_t* pb = b;

    return *pa - *pb;
}

void chunk_qsort(Chunk* ch) {
    qsort(ch->array, ch->size, sizeof (int32_t), compare_int32);
}

/* ------------- HEAP BINARY TREE ---------------
 */

typedef struct s_heap {
    Chunk* chunk;
    struct s_heap* left;
    struct s_heap* rigth;
} Heap;

/* TODO : Tail recursion */
static inline void heap_cascade(Heap* node){
    if ( node->left == NULL ){
        if ( node->rigth != NULL){
            node->left = node->rigth;
            node->rigth = NULL;
        }
        else
            return;
    }
    
    if ( node->rigth == NULL){
        if ( node->left->left != NULL){
            node->rigth = node->left->left;
            node->left->left = NULL;
            heap_cascade(node->left);
        }
    }
}

Heap* heap_insert(Heap* heap, Heap* to_insert) {
    if (heap == NULL)
        return heap;

    if (heap->left == NULL) {

        if (*(heap->chunk->current) <= *(to_insert->chunk->current)) {
            heap->left = to_insert;
            return heap;
        } else {
            to_insert->left = heap;
            return to_insert;
        }
    } else if (heap->rigth == NULL) {

        if (*(heap->chunk->current) <= *(to_insert->chunk->current)) {
            heap->rigth = to_insert;
            return heap;
        } else {
            to_insert->left = heap->left;
            heap->left = NULL;
            to_insert->rigth = heap;
            return to_insert;
        }
    } else {

        if (*(heap->chunk->current) > *(to_insert->chunk->current)) {
            to_insert->left = heap;
            to_insert->rigth = heap->left;
            heap->left = heap->rigth;
            heap->rigth = NULL;
            heap_cascade(heap);
            return to_insert;

        } else {
            if (*(heap->left->chunk->current) <= *(heap->rigth->chunk->current)) {
                if (*(to_insert->chunk->current) < *(heap->left->chunk->current)) {

                }

            } else {
                to_insert->left = heap->left;
                heap->left = NULL;
                to_insert->rigth = heap;
                return to_insert;
            }

        }


    }
}

void disk_merge_sort() {

    int start = g_get_monotonic_time();

    Chunk* ch = chunk_new(7000000);
    FILE* f = fopen("list.txt", "r");
    load_chunk(ch, f);
    chunk_qsort(ch);
    //print_chunk(ch);
    fclose(f);
    printf("%d\n", chunk_pop(ch));
    printf("%d\n", chunk_pop(ch));

    int stop = g_get_monotonic_time();

    printf("Numbers sorted in %.0f ms", (stop - start) / 1000.0f);
}