#include <HashMap.h>
#include <stdio.h>

#include <stdlib.h>

#include <math.h>

static void IntMapPrinter(const char* key, int* i, void* ctx)
{
    printf("map['%s'] = {%d}\n", key, *i);
}

static void IntPtrMapPrinter(const int* key, int** i, void* ctx)
{
    printf("map['%d'] = {%zu} = {%d}\n", *key, (size_t)*i, **i);
}

static void FreeDeleter(void** ptr)
{
    int** x = ptr;
    printf("Deleting '%zu' = {%d}\n", (size_t)*x, **x);

    free(*ptr);
}

int main(int argc, char** argv)
{
    HashMap map = HashMapCreate(HASHMAP_KEY_STRING,
        sizeof(int),
        64,
        &HashMapFNV1AHash,
        &HashMapStringCmp,
        NULL);

    int age = 10;

    HashMapInsert(&map, "John", &age);
    age = 50;
    HashMapInsert(&map, "Henry", &age);

    int* found_age = HashMapGet(&map, "John");
    if (found_age) printf("Map['%s'] = {%d}\n", "John", *found_age);

    HashMapForEach(&map, &IntMapPrinter, NULL);

    HashMapErase(&map, "John");

    HashMapForEach(&map, &IntMapPrinter, NULL);

    HashMapDestroy(&map);

    HashMap ptrMap = HashMapCreate(HASHMAP_KEY_INT,
        sizeof(int*),
        8,
        &HashMapIntHash,
        &HashMapIntCmp,
        &FreeDeleter);


    for (int i = 0; i < 10; ++i)
    {
        int* val = malloc(sizeof(int));
        *val = rand();
        HashMapInsert(&ptrMap, &i, &val);
    }

    HashMapForEach(&ptrMap, &IntPtrMapPrinter, NULL);

    size_t k = 9;
    HashMapErase(&ptrMap, &k);

    int* new_val = malloc(sizeof(int));
    k = 5;
    *new_val = 69;
    HashMapInsert(&ptrMap, &k, &new_val);

    HashMapForEach(&ptrMap, &IntPtrMapPrinter, NULL);

    HashMapDestroy(&ptrMap);

    return 0x0;
}