#include "DynArray.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

DynArray DynArrayCreate(const size_t element_size, const size_t initial_capacity, void(*deleter)(void*))
{
    DynArray arr = {0};
    arr.element_size = element_size;
    arr.deleter = deleter;
    DynArrayReserve(&arr, initial_capacity);
    
    return arr;
}

size_t DynArraySize(DynArray* array)
{
    assert(array && "how big is an idea?");
    return array->size;
}

void DynArrayReserve(DynArray* array, const size_t new_capacity)
{
    assert(array && "please sir, where may I put the memory?");
    if(new_capacity < array->capacity) return;
    
    array->data = realloc(array->data, new_capacity * array->element_size);
    assert(array->data && "Get more ram lol");
    array->capacity = new_capacity;
}

void DynArrayDestroy(DynArray* array)
{
    if(array == NULL) return;

    if(array->deleter)
    {
        size_t size = DynArraySize(array);
        void(*deleter)(void*) = array->deleter;

        for(size_t i = 0; i < size; ++i)
        {
            deleter(DynArrayGet(array, i));
        }
    }

    free(array->data);
    
    DynArray arr = {0};
    *array = arr;
}

void DynArrayPush(DynArray* array, void* element)
{
    assert(array && "no, I won't be pushing that");

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
    if(array->deleter) array->deleter(DynArrayBack(array));

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
    if(array->deleter) array->deleter(ptr_to_erase);
    
    memmove(ptr_to_erase, (unsigned char*)ptr_to_erase + array->element_size,
            (array->size - 1 - index) * array->element_size);
    
    --array->size;
}

STI_Finder DynArrayFind(DynArray* array, void* element, STI_BOOL(*cmp)(const void*, const void*))
{
    assert(array);
    assert(element && "can't see it mate");
    assert(cmp && "No comparison function provided");

    size_t size = DynArraySize(array);
    STI_Finder result;
    memset(&result, 0, sizeof(result));

    for(size_t i = 0; i < size; ++i)
    {
        if(cmp(element, DynArrayGet(array, i)))
        {
            result.is_found = STI_TRUE;
            result.result.index = i;
            break;
        }
    }

    return result;
}

void DynArrayEraseIf(DynArray* array, void* element, STI_BOOL(*cmp)(const void*, const void*))
{
    assert(array);
    assert(element && "Element to erase was NULL");
    assert(cmp && "No comparison function provided");

    for(size_t i = 0; i < DynArraySize(array);)
    {
        if(cmp(element, DynArrayGet(array, i)))
        {
            DynArrayErase(array, i);
        }
        else
        {
            ++i;
        }
    }
}

void DynArrayForEach(DynArray* array, void(*func)(void*, void*), void* ctx)
{
    size_t size = DynArraySize(array);

    for(size_t i = 0; i < size; ++i)
    {
        func(DynArrayGet(array, i), ctx);
    }
}

void DynArrayHeapSwap(DynArray* array, size_t index_a, size_t index_b)
{    
    if(index_a == index_b) return;

    void* a = DynArrayGet(array, index_a);
    void* b = DynArrayGet(array, index_b);

    void* tmp = malloc(array->element_size);
    assert(tmp && "Get more ram LOL");

    memcpy(tmp, a, array->element_size);
    memcpy(a, b, array->element_size);
    memcpy(b, tmp, array->element_size);

    free(tmp);
}

void DynArrayBatchPush(DynArray* array, void* data, const size_t data_element_count, const size_t offset)
{
    assert(offset + data_element_count <= array->capacity && "Batch Copy out of bounds");
    void* offset_ptr = DynArrayGet(array, offset);
    memcpy(offset_ptr, data, data_element_count * array->element_size);
    array->size = offset + data_element_count;
}