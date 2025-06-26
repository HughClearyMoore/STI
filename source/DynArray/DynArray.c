#include "DynArray.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

DynArray DynArrayCreate(const size_t element_size)
{
    DynArray arr = {0};
    arr.element_size = element_size;
    
    return arr;
}

void DynArrayReserve(DynArray* array, const size_t new_capacity)
{
    if(new_capacity < array->capacity) return;
    
    array->data = realloc(array->data, new_capacity * array->element_size);
    assert(array->data && "Get more ram lol");
}

void DynArrayDestroy(DynArray* array)
{
    free(array->data);
    
    DynArray arr = {0};
    *array = arr;
}

void DynArrayPush(DynArray* array, void* element)
{
    if(array->size >= array->capacity)
    {
        DynArrayReserve(array, array->capacity ? array->capacity * 2 : 1);
    }

    memcpy((unsigned char*)array->data + array->size * array->element_size, element, array->element_size);
    ++array->size;
}

void* DynArrayGet(DynArray* array, const size_t index)
{
    assert(index < array->size && "that full on doesn't exist");
    return (unsigned char*)array->data + index * array->element_size;
}

void* DynArrayBack(DynArray* array)
{
    assert(array->size && "the back of what? your mum ain't that big");
    return DynArrayGet(array, array->size - 1);
}

void DynArrayPopBack(DynArray* array)
{
    assert(array->size && "can't pop a whole lot of nothing");
    --array->size;
}

void DynArrayErase(DynArray* array, const size_t index)
{
    assert(index < array->size && "erase deez nutz");
    if(index == array->size - 1)
    {
        DynArrayPopBack(array);
        return;
    }

    void* back = DynArrayBack(array);
    void* ptr_to_erase = DynArrayGet(array, index);
    
    memmove(ptr_to_erase, (unsigned char*)ptr_to_erase + array->element_size,
            (array->size - 1 - index) * array->element_size);
    
    --array->size;
}