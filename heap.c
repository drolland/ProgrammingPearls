#include <strings.h>

#include "heap.h"
#include <stdio.h>

static inline void heapify_min_int(int* heap, int i) {
    if (heap[HEAP_LEFT(i)] <= heap[HEAP_RIGHT(i)]) {
        if (heap[i] >= heap[HEAP_LEFT(i)]) {
            int temp = heap[i];
            heap[i] = heap[HEAP_LEFT(i)];
            heap[HEAP_LEFT(i)] = temp;
        } 
    }
    else {
        if (heap[i] >= heap[HEAP_RIGHT(i)]) {
            int temp = heap[i];
            heap[i] = heap[HEAP_RIGHT(i)];
            heap[HEAP_RIGHT(i)] = temp;
        } 
    }
}


void heap_build_min_int(int* heap, int size) {
    for(int i = (size - 2) / 2; i >= 0; i--){
        heapify_min_int(heap,i);
    }
}


void heap_print_int(int* heap,int size){
    for(int i = 0; i < size;i++)
        printf("%d\t",heap[i]);
    printf("\n");
}


void test_heap(){
    int heap[7] = {5 , 3 , 2 , 4 , 1 , 4 , 6};
    heap_print_int(heap,7);
    heapify_min_int(heap,0);
    heap_print_int(heap,7);
    heapify_min_int(heap,1);
    heap_print_int(heap,7);
    heapify_min_int(heap,2);
    heap_print_int(heap,7);
}