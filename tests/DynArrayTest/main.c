#include <stdio.h>
#include <stdlib.h>
#include <DynArray.h>
#include <Utility.h>


static STI_BOOL IntComparator(const int* a, const int* b)
{
    return *a == *b;
}

static STI_BOOL IntPtrComparator(const int* a, const int** b)
{
    return *a == **b;
}

static void FreeDeleter(void** element)
{
    printf("Deleting '%zu'\n", (size_t)(*element));
    free(*element);
}

static void PtrSummer(int** element, int* out)
{
    int val = **element;
    *out += val;
}

static void ArrayPrinter(size_t* element, size_t* count)
{
    printf("Array[%zu] = '%zu'\n", (*count)++, *element);
}

static void IntArrayPrinter(int* element, size_t* count)
{
    printf("Array[%zu] = '%d'\n", (*count)++, *element);
}

int main(int argc, char** argv)
{
    DynArray array = DynArrayCreate(sizeof(int), 0, NULL);


    for(int i = 0; i < 10; ++i)
    {
        DynArrayPush(&array, &i);
    }

    printf("Array size is now '%zu' and capacity is '%zu'\n", array.size, array.capacity);


    for(int i = 0; i < 10; ++i)
    {
        printf("Array[%d] = %d\n", i, *(int*)DynArrayGet(&array, i));
    }

    int to_find = 5;
    STI_Finder finder = DynArrayFind(&array, &to_find, &IntComparator);

    if(finder.is_found)
    {
        printf("Found '%d' at Array['%zu']\n", to_find, finder.result.index);
    }

    DynArraySwap(&array, 0, 4, int);

    size_t count0 = 0;
    DynArrayForEach(&array, &IntArrayPrinter, &count0);



    DynArrayDestroy(&array);


    DynArray ptrArray = DynArrayCreate(sizeof(int*), 10, &FreeDeleter);
    
    for (int i = 0; i < 10; ++i)
    {
        int* ptr = malloc(sizeof(int));
        *ptr = i;
        DynArrayPush(&ptrArray, &ptr);
    }

    printf("Array size is now '%zu' and capacity is '%zu'\n", ptrArray.size, ptrArray.capacity);

    for (int i = 0; i < 10; ++i)
    {
        int** ptr = DynArrayGet(&ptrArray, i);


        printf("Array[%d] = %zu {%d}\n", i, (size_t)*ptr, **ptr);
    }

    int sum = 0;
    DynArrayForEach(&ptrArray, &PtrSummer, &sum);

    printf("The sum of all the values is '%d'\n", sum);


    size_t count = 0;
    DynArrayForEach(&ptrArray, &ArrayPrinter, &count);

    {
        int x = 3;
        DynArrayEraseIf(&ptrArray, &x, &IntPtrComparator);
    }    

    count = 0;
    DynArrayForEach(&ptrArray, &ArrayPrinter, &count);

    DynArrayDestroy(&ptrArray);


    return 0x0;
}