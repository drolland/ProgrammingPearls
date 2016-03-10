#include "heap.h"

#define PARENT(i)   i/2
#define LEFT(i)     i<<1
#define RIGTH(i)    (i<<1)+1

typedef void* Heap;

#define VALUE(i) (int)(Heap[i])
#define ASSIGN(heap,i,value) heap[i] = (void*)(value)



