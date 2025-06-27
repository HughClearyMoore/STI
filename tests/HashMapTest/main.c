#include <HashMap.h>
#include <stdio.h>

static IntMapPrinter(const char* key, int* i, void* ctx)
{
    printf("map['%s'] = {%d}\n", key, *i);
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

    return 0x0;
}