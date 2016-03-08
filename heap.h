#pragma once

#define HEAP_PARENT(i) (i-1)/2
#define HEAP_LEFT(i) (i+1) << 1 - 1
#define HEAP_RIGHT(i) (i+1) << 1 


void test_heap();
