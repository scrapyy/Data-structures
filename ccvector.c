#include "ccvector.h"
#include "common.h"
#include<stdlib.h>
#include "ccheap.h"

#define VECSIZE 1

static int ReallocMore(CC_VECTOR *Vector)
{
    int *aux = realloc(Vector->elems, sizeof(int)*Vector->memSize * 2);
    if (aux == NULL)
    {
        return -1;
    }
    Vector->elems = aux;
    Vector->memSize = Vector->memSize * 2;
    return 0;
}

static int ReallocLess(CC_VECTOR *Vector)
{
    int *aux = realloc(Vector->elems, sizeof(int)*Vector->memSize / 2);
    if (aux == NULL)
    {
        return -1;
    }
    Vector->elems = aux;
    Vector->memSize = Vector->memSize / 2;
    return 0;
}

int VecCreate(CC_VECTOR **Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    (*Vector) = malloc(sizeof(CC_VECTOR));
    if ((*Vector) == NULL)
    {
        return -1;
    }
    (*Vector)->elems = malloc(sizeof(int)*VECSIZE);
    if ((*Vector)->elems == NULL)
    {
        VecDestroy(Vector);
        return -1;
    }
    (*Vector)->actSize = 0;
    (*Vector)->memSize = VECSIZE;
    return 0;
}

int VecDestroy(CC_VECTOR **Vector)
{
    if (Vector == NULL || *Vector == NULL)
    {
        return -1;
    }
    if ((*Vector)->elems != NULL)
    {
        free((*Vector)->elems);
        (*Vector)->elems = NULL;
    }
    free(*Vector);
    *Vector = NULL;
    return 0;
}

int VecInsertTail(CC_VECTOR *Vector, int Value)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Vector->memSize < Vector->actSize + 1)
    {
        int h = ReallocMore(Vector);
        if (h != 0)
        {
            return -1;
        }
    }
    Vector->elems[Vector->actSize] = Value;
    Vector->actSize = Vector->actSize + 1;
    return 0;
}

int VecInsertHead(CC_VECTOR *Vector, int Value)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Vector->memSize < Vector->actSize + 1)
    {
        int h = ReallocMore(Vector);
        if (h != 0)
        {
            return -1;
        }
    }
    for (int i = Vector->actSize; i > 0; i--)
    {
        Vector->elems[i] = Vector->elems[i - 1];
    }
    Vector->elems[0] = Value;
    Vector->actSize = Vector->actSize + 1;
    return 0;
}

int VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Index >= Vector->actSize)
    {
        return -1;
    }
    if (Index < 0)
    {
        return -1;
    }
    if (Vector->memSize < Vector->actSize + 1)
    {
        int h = ReallocMore(Vector);
        if (h != 0)
        {
            return -1;
        }
    }
    for (int i = Vector->actSize; i > Index + 1; i--)
    {
        Vector->elems[i] = Vector->elems[i - 1];
    }
    Vector->elems[Index + 1] = Value;
    Vector->actSize = Vector->actSize + 1;
    return 0;
}

int VecRemoveByIndex(CC_VECTOR *Vector, int Index)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Index >= Vector->actSize)
    {
        return -1;
    }
    if (Index < 0)
    {
        return -1;
    }
    for (int i = Index + 1; i < Vector->actSize; i++)
    {
        Vector->elems[i - 1] = Vector->elems[i];
    }
    Vector->actSize = Vector->actSize - 1;
    if (Vector->actSize * 4 <= Vector->memSize)
    {
        int h = ReallocLess(Vector);
        if (h != 0)
        {
            // do nothing
        }
    }
    return 0;
}

int VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Value == NULL)
    {
        return -1;
    }
    if (Index >= Vector->actSize)
    {
        return -1;
    }
    if (Index < 0)
    {
        return -1;
    }
    *Value = Vector->elems[Index];
    return 0;
}

int VecGetCount(CC_VECTOR *Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    return Vector->actSize;
}

int VecClear(CC_VECTOR *Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    int *aux = realloc(Vector->elems, sizeof(int)*VECSIZE);
    if (aux == NULL)
    {
        return -1;
    }
    Vector->elems = aux;
    Vector->actSize = 0;
    Vector->memSize = VECSIZE;
    return 0;
}

int VecSort(CC_VECTOR *Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    CC_HEAP *Heap;
    int h = HpCreateMinHeap(&Heap, Vector);
    if (h == -1)
    {
        return -1;
    }
    h = HpSortToVector(Heap, Vector);
    if (h == -1)
    {
        return -1;
    }
    HpDestroy(&Heap);
    return 0;
}
