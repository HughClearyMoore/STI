#include "HashMap.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "Utility.h"

static const size_t HashMapGetBucketIndex(HashMap* map, const void* key)
{
    size_t hash = map->hash_function(key);
    return hash % map->bucket_count;
}

static const void* HashMapGetKeyRef(HashMapEntry* entry, HashMapKeyType type)
{
    switch(type)
    {
        case HASHMAP_KEY_INT:       return &entry->Key.i;
        case HASHMAP_KEY_STRING:    return entry->Key.s;
        default: assert(0 && "Invalid key type"); return NULL;
    }
}

static void HashMapSetKeyValue(HashMapEntry* entry, HashMapKeyType type, const void* key)
{
    switch (type)
    {
    case HASHMAP_KEY_INT:       entry->Key.i = *(int*)key; break;
    case HASHMAP_KEY_STRING:    entry->Key.s = strdup((const char*)key); break;
    default: assert(0 && "Invalid key type"); return NULL;
    }
}

HashMap HashMapCreate(const HashMapKeyType key_type,
    const size_t element_size,
    const size_t bucket_count,
    size_t(*hash_function)(const void*),
    STI_BOOL(*key_comparator)(const void*, const void*),
    void(*deleter)(void*))
{
    assert(hash_function);
    assert(key_comparator);

    HashMap map = {0};
    map.key_type = key_type;
    map.element_size = element_size;
    map.bucket_count = bucket_count;
    map.hash_function = hash_function;
    map.key_comparator = key_comparator;
    map.deleter = deleter;

    map.bucket = DynArrayCreate(sizeof(DynArray), bucket_count, NULL);

    for(size_t i = 0; i < bucket_count; ++i)
    {
        DynArray slot = DynArrayCreate(sizeof(HashMapEntry) + element_size, 0, NULL);
        DynArrayPush(&map.bucket, &slot);
    }

    return map;
}

void HashMapDestroy(HashMap* map)
{
    if (!map) return;

    void(*deleter)(void*) = map->deleter;
    size_t bucket_count = map->bucket_count;

    for (size_t i = 0; i < bucket_count; ++i)
    {
        DynArray* slot = DynArrayGet(&map->bucket, i);
       
        size_t size = DynArraySize(slot);
        for (size_t j = 0; j < size; ++j)
        {
            HashMapEntry* entry = DynArrayGet(slot, j);            

            if (deleter) deleter(entry->data);        
            if (map->key_type == HASHMAP_KEY_STRING) free(entry->Key.s);
        }

        DynArrayDestroy(slot);
    }

    DynArrayDestroy(&map->bucket);
    *map = (HashMap){ 0 };
}

void HashMapInsert(HashMap* map, const void* key, const void* data)
{
    assert(map);
    assert(key);
    assert(data);

    const size_t index = HashMapGetBucketIndex(map, key);

    DynArray* slot = DynArrayGet(&map->bucket, index);

    size_t size = DynArraySize(slot);
    STI_Finder finder;
    memset(&finder, 0, sizeof(finder));

    STI_BOOL(*comparator)(const void*, const void*) = map->key_comparator;

    for (size_t i = 0; i < size; ++i)
    {
        HashMapEntry* entry = DynArrayGet(slot, i);
        void* key_ref = HashMapGetKeyRef(entry, map->key_type);

        if (comparator(key, key_ref))
        {
            finder.is_found = STI_TRUE;
            finder.result.index = i;
            break;
        }
    }
   
    if (finder.is_found)
    {
        HashMapEntry* entry = DynArrayGet(slot, finder.result.index);
        if (map->deleter)
        {
            map->deleter(entry->data);
        }

        memcpy(entry->data, data, map->element_size);
    }
    else
    {
        HashMapEntry* new_entry = malloc(sizeof(HashMapEntry) + map->element_size);
        assert(new_entry && "lol buy more ram");
        memset(new_entry, 0, sizeof(HashMapEntry) + map->element_size);        
        memcpy(new_entry->data, data, map->element_size);
        HashMapSetKeyValue(new_entry, map->key_type, key);

        DynArrayPush(slot, new_entry);

        free(new_entry);
    }
}

void HashMapForEach(HashMap* map, void(*func)(const void*, void*, void*), void* ctx)
{
    const size_t bucket_count = map->bucket_count;

    for (size_t i = 0; i < bucket_count; ++i)
    {
        DynArray* slot = DynArrayGet(&map->bucket, i);

        const size_t slot_size = DynArraySize(slot);

        for (size_t j = 0; j < slot_size; ++j)
        {
            HashMapEntry* entry = DynArrayGet(slot, j);
            func(HashMapGetKeyRef(entry, map->key_type), entry->data, ctx);
        }
    }
}

void* HashMapGet(HashMap* map, const void* key)
{
    const size_t index = HashMapGetBucketIndex(map, key);
    DynArray* slot = DynArrayGet(&map->bucket, index);

    const size_t size = DynArraySize(slot);

    STI_BOOL(*comparator)(const void* a, const void* b) = map->key_comparator;

    for (size_t i = 0; i < size; ++i)
    {
        HashMapEntry* entry = DynArrayGet(slot, i);
        void* key_ref = HashMapGetKeyRef(entry, map->key_type);

        if (comparator(key_ref, key))
        {
            return entry->data;
        }
    }

    return NULL;
}

void HashMapErase(HashMap* map, const void* key)
{
    const size_t index = HashMapGetBucketIndex(map, key);
    DynArray* slot = DynArrayGet(&map->bucket, index);

    const size_t size = DynArraySize(slot);

    STI_BOOL(*comparator)(const void* a, const void* b) = map->key_comparator;

    for(size_t i = 0; i < size; ++i)
    {
        HashMapEntry* entry = DynArrayGet(slot, i);
        void* key_ref = HashMapGetKeyRef(entry, map->key_type);

        if(comparator(key_ref, key))
        {
            if(map->deleter) map->deleter(entry->data);
            if(map->key_type == HASHMAP_KEY_STRING) free(entry->Key.s);
            
            if(i != size - 1)
            {
                HashMapEntry* last = DynArrayBack(slot);
                memcpy(entry, last, slot->element_size);                
            }
            
            DynArrayPopBack(slot);
                        
            return;
        }
    }
}

const STI_BOOL HashMapContains(HashMap* map, const void* key)
{
    return HashMapGet(map, key);
}

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
size_t HashMapFNV1AHash(const char* key)
{
#if UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF
    size_t FNV_offset_bias = 14695981039346656037ULL;
    size_t FNV_prime = 1099511628211ULL;
#elif UINTPTR_MAX == 0xFFFFFFFF
    size_t FNV_offset_bias = 2166136261U;
    size_t FNV_prime = 16777619U;
#endif

    size_t hash = FNV_offset_bias;
    size_t length = strlen(key);
    for (size_t i = 0; i < length; ++i)
    {
        hash ^= key[i];
        hash *= FNV_prime;
    }

    return hash;
}

size_t HashMapIntHash(const size_t* key)
{
    return *key; // incredible stuff
}

STI_BOOL HashMapStringCmp(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

STI_BOOL HashMapIntCmp(const size_t* a, const size_t* b)
{
    return *a == *b;
}