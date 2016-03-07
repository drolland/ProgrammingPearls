#include <sys/types.h>
#include <assert.h>

typedef struct s_file_chunk{
    int size;
    int32_t* array;
} FileChunk;


FileChunk* file_chunk_new(int size){
    FileChunk* new = malloc(sizeof(FileChunk));
    new->size = size;
    new->array = malloc(sizeof(int32_t)*size);
    return new;
}

void load_file_chunk(FileChunk* file_chunk, FILE* file){
    assert(file_chunk != NULL);
    
} 

void print_file_chunk

