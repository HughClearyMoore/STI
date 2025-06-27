#ifndef STI_HASH_MAP_27062025
#define STI_HASH_MAP_27062025

#include <stddef.h>

#include "DynArray.h"

typedef enum HashMapKeyType
{
    HASHMAP_KEY_INT,
    HASHMAP_KEY_STRING
} HashMapKeyType;

typedef struct HashMap
{
    HashMapKeyType key_type;
    size_t bucket_count;
    size_t(*hash_function)(const void*);
    size_t element_size;
    void(*deleter)(void*);
    STI_BOOL(*key_comparator)(const void*, const void*);
    DynArray bucket;
} HashMap;

typedef struct HashMapEntry
{
    union
    {
        size_t i;
        char* s;
    } Key;

    char data[];
} HashMapEntry;

HashMap HashMapCreate(const HashMapKeyType key_type, const size_t element_size, 
    const size_t bucket_count, 
    size_t(*hash_function)(const void*), 
    STI_BOOL(*key_comparator)(const void*, const void*),
    void(*deleter)(void*));
void HashMapDestroy(HashMap* map);
void HashMapInsert(HashMap* map, const void* key, const void* data);
void HashMapForEach(HashMap* map, void(*func)(const void*, void*, void*), void* ctx);
void* HashMapGet(HashMap* map, const void* key);
void HashMapErase(HashMap* map, const void* key);
const STI_BOOL HashMapContains(HashMap* map, const void* key);

size_t HashMapFNV1AHash(const char* key);
size_t HashMapIntHash(const size_t* key);
STI_BOOL HashMapStringCmp(const char* a, const char* b);
STI_BOOL HashMapIntCmp(const size_t* a, const size_t* b);

#endif