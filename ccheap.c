#include "ccheap.h"
#include "common.h"
#include<stdlib.h>


#define HPSIZE 4

static int CheckValue(int Value1, int Value2, int Type)
{
    if (Type == 0)
    {
        if (Value1 > Value2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (Value1 < Value2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

static void GetUpHeap(CC_HEAP *Heap, int pos)
{
    int dad = (pos - 1) / 2;
    while (pos > 0 && CheckValue(Heap->Vec->elems[dad], Heap->Vec->elems[pos], Heap->typeHeap))
    {
        int aux = Heap->Vec->elems[pos];
        Heap->Vec->elems[pos] = Heap->Vec->elems[dad];
        Heap->Vec->elems[dad] = aux;
        pos = dad;
        dad = (pos - 1) / 2;
    }
}

static void GetDownHeap(CC_HEAP *Heap, int pos)
{
    int son = 0;
    if (pos * 2 + 1 < Heap->Vec->actSize)
    {
        son = pos * 2 + 1;
    }
    if (pos * 2 + 2 < Heap->Vec->actSize && CheckValue(Heap->Vec->elems[pos * 2 + 1], Heap->Vec->elems[pos * 2 + 2],Heap->typeHeap))
    {
        son = pos * 2 + 2;
    }
    while (son != 0 && CheckValue(Heap->Vec->elems[pos], Heap->Vec->elems[son], Heap->typeHeap))
    {
        int aux = Heap->Vec->elems[son];
        Heap->Vec->elems[son] = Heap->Vec->elems[pos];
        Heap->Vec->elems[pos] = aux;
        pos = son;
        son = 0;
        if (pos * 2 + 1 < Heap->Vec->actSize)
        {
            son = pos * 2 + 1;
        }
        if (pos * 2 + 2 < Heap->Vec->actSize && CheckValue(Heap->Vec->elems[pos * 2 + 1], Heap->Vec->elems[pos * 2 + 2], Heap->typeHeap))
        {
            son = pos * 2 + 2;
        }
    }
}

int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
    if (MaxHeap == NULL)
    {
        return -1;
    }
    *MaxHeap = malloc(sizeof(CC_HEAP));
    if (*MaxHeap == NULL)
    {
        return -1;
    }
    int h = VecCreate(&(*MaxHeap)->Vec);
    if (h == -1)
    {
        HpDestroy(MaxHeap);
        return -1;
    }
    (*MaxHeap)->typeHeap = 1;
    if (InitialElements != NULL)
    {
        for (int i = 0; i < InitialElements->actSize; i++)
        {
            h = HpInsert(*MaxHeap, InitialElements->elems[i]);
            if (h == -1)
            {
                HpDestroy(MaxHeap);
                return -1;
            }
        }
    }
    return 0;
}

int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
    if (MinHeap == NULL)
    {
        return -1;
    }
    *MinHeap = malloc(sizeof(CC_HEAP));
    if (*MinHeap == NULL)
    {
        return -1;
    }
    int h = VecCreate(&(*MinHeap)->Vec);
    if (h == -1)
    {
        HpDestroy(MinHeap);
        return -1;
    }
    (*MinHeap)->typeHeap = 0;
    if (InitialElements != NULL)
    {
        for (int i = 0; i < InitialElements->actSize; i++)
        {
            h = HpInsert(*MinHeap, InitialElements->elems[i]);
            if (h == -1)
            {
                HpDestroy(MinHeap);
                return -1;
            }
        }
    }
    return 0;
}

int HpDestroy(CC_HEAP **Heap)
{
    if (Heap==NULL || *Heap==NULL)
    {
        return -1;
    }
    int h = VecDestroy(&(*Heap)->Vec);
    if (h == -1)
    {
        return -1;
    }
    free((*Heap));
    *Heap = NULL;
    return 0;
}



int HpInsert(CC_HEAP *Heap, int Value)
{
    if (Heap == NULL)
    {
        return -1;
    }
    int h = VecInsertTail(Heap->Vec, Value);
    if (h == -1)
    {
        return -1;
    }
    GetUpHeap(Heap, Heap->Vec->actSize - 1);
    return 0;
}

int HpRemove(CC_HEAP *Heap, int Value)
{
    if (Heap == NULL)
    {
        return -1;
    }
    for (int i = Heap->Vec->actSize - 1; i >= 0; i--)
    {
        if (Heap->Vec->elems[i] == Value)
        {
            Heap->Vec->elems[i] = Heap->Vec->elems[Heap->Vec->actSize - 1];
            VecRemoveByIndex(Heap->Vec, Heap->Vec->actSize - 1);
            GetDownHeap(Heap, i);
        }
    }
    return 0;
}

int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    if (Heap == NULL)
    {
        return -1;
    }
    if (ExtremeValue == NULL)
    {
        return -1;
    }
    if (Heap->Vec->actSize == 0)
    {
        return -1;
    }
    *ExtremeValue = Heap->Vec->elems[0];
    return 0;
}

int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    if (Heap == NULL)
    {
        return -1;
    }
    if (ExtremeValue == NULL)
    {
        return -1;
    }
    int aux;
    int h = HpGetExtreme(Heap, &aux);
    if (h == -1)
    {
        return -1;
    }
    h = HpRemove(Heap, aux);
    if (h == -1)
    {
        return -1;
    }
    *ExtremeValue = aux;
    return 0;
}

int HpGetElementCount(CC_HEAP *Heap)
{
    if (Heap == NULL)
    {
        return -1;
    }
    return VecGetCount(Heap->Vec);
}

static void CopyHeap(CC_HEAP *Source, CC_HEAP *Destination)
{
    for (int i = 0; i < Source->Vec->actSize; i++)
    {
        HpInsert(Destination, Source->Vec->elems[i]);
    }
}

static void RemoveOnlyRoot(CC_HEAP *Heap)
{
    Heap->Vec->elems[0] = Heap->Vec->elems[Heap->Vec->actSize - 1];
    VecRemoveByIndex(Heap->Vec, Heap->Vec->actSize - 1);
    GetDownHeap(Heap, 0);
}

int HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector)
{
    if (Heap == NULL)
    {
        return -1;
    }
    if (SortedVector == NULL)
    {
        return -1;
    }
    CC_HEAP *mask;
    int h;
    if (Heap->typeHeap == 0)
    {
        h = HpCreateMinHeap(&mask, NULL);
    }
    else
    {
        h = HpCreateMaxHeap(&mask, NULL);
    }
    if (h == -1)
    {
        return -1;
    }
    CopyHeap(Heap, mask);
    int lim = mask->Vec->actSize;
    for (int i = 0; i < lim; i++)
    {
        h = VecInsertTail(SortedVector, mask->Vec->elems[0]);
        if (h == -1)
        {
            HpDestroy(&mask);
            return -1;
        }
        RemoveOnlyRoot(mask);
    }
    HpDestroy(&mask);
    return 0;
}
