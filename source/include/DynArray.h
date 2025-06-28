#ifndef STI_DYN_ARRAY_26062025
#define STI_DYN_ARRAY_26062025

#include <stddef.h>

#include "Utility.h"

typedef struct DynArray
{
    void* data;
    size_t size;
    size_t capacity;
    size_t element_size;
    void(*deleter)(void*);
} DynArray;


DynArray DynArrayCreate(const size_t element_size, const size_t initial_capacity, void(*deleter)(void*));
void DynArrayReserve(DynArray* array, const size_t new_capacity);
void DynArrayDestroy(DynArray* array);
void DynArrayPush(DynArray* array, void* element);
void* DynArrayGet(DynArray* array, const size_t index);
void* DynArrayBack(DynArray* array);
void DynArrayPopBack(DynArray* array);
void DynArrayErase(DynArray* array, const size_t index);
size_t DynArraySize(DynArray* array);
STI_Finder DynArrayFind(DynArray* array, void* element, STI_BOOL(*cmp)(const void*, const void*));
void DynArrayEraseIf(DynArray* array, void* element, STI_BOOL(*cmp)(const void*, const void*));
void DynArrayForEach(DynArray* array, void(*func)(void*, void*), void* ctx);
void DynArrayHeapSwap(DynArray* array, size_t index_a, size_t index_b);
void DynArrayBatchPush(DynArray* array, void* data, const size_t data_element_count, const size_t offset);

#define DynArraySwap(array, index_a, index_b, type) \
    do \
    { \
        type* _a27062025 = DynArrayGet(array, index_a); \
        type* _b27062025 = DynArrayGet(array, index_b); \
        STI_SWAP(*_a27062025, *_b27062025, type); \
    } while(0)
    
#endif