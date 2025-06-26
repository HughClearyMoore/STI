#ifndef STI_DYN_ARRAY_26062025
#define STI_DYN_ARRAY_26062025

#include "stddef.h"

typedef struct DynArray
{
    void* data;
    size_t size;
    size_t capacity;
    size_t element_size;
} DynArray;


DynArray DynArrayCreate(const size_t element_size);
void DynArrayReserve(DynArray* array, const size_t new_capacity);
void DynArrayDestroy(DynArray* array);
void DynArrayPush(DynArray* array, void* element);
void* DynArrayGet(DynArray* array, const size_t index);
void* DynArrayBack(DynArray* array);
void DynArrayPopBack(DynArray* array);
void DynArrayErase(DynArray* array, const size_t index);


#endif