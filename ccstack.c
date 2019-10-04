#include "ccstack.h"
#include "common.h"
#include<stdlib.h>

#define STSIZE 4 

static int ReallocMore(CC_STACK *Stack)
{
    int *aux;
    aux = realloc(Stack->elems, sizeof(int)*Stack->memSize * 2);
    if (aux == NULL)
    {
        return -1;
    }
    Stack->elems = aux;
    Stack->memSize = Stack->memSize * 2;
    return 0;
}
static int ReallocLess(CC_STACK *Stack)
{
    int *aux;
    aux = realloc(Stack->elems, sizeof(int)*Stack->memSize / 2);
    if (aux == NULL)
    {
        return -1;
    }
    Stack->elems = aux;
    Stack->memSize = Stack->memSize / 2;
    return 0;
}

int StCreate(CC_STACK **Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    *Stack = malloc(sizeof(CC_STACK));
    if (*Stack == NULL)
    {
        return -1;
    }
    (*Stack)->elems = malloc(sizeof(int)*STSIZE);
    if ((*Stack)->elems == NULL)
    {
        StDestroy(Stack);
        return -1;
    }
    (*Stack)->actSize = 0;
    (*Stack)->memSize = STSIZE;
    return 0;
}

int StDestroy(CC_STACK **Stack)
{
    if (Stack == NULL || *Stack == NULL)
    {
        return -1;
    }
    if ((*Stack)->elems != NULL)
    {
        free((*Stack)->elems);
        (*Stack)->elems = NULL;
    }
    free(*Stack);
    *Stack = NULL;
    return 0;
}

int StPush(CC_STACK *Stack, int Value)
{
    if (Stack == NULL)
    {
        return 0;
    }
    if (Stack->actSize + 1 > Stack->memSize)
    {
        int h = ReallocMore(Stack);
        if (h == -1)
        {
            return -1;
        }
    }
    Stack->elems[Stack->actSize] = Value;
    Stack->actSize = Stack->actSize + 1;
    return 0;
}

int StPop(CC_STACK *Stack, int *Value)
{
    if (Stack == NULL)
    {
        return -1;
    }
    if (Value == NULL)
    {
        return -1;
    }
    if (Stack->actSize == 0)
    {
        return -1;
    }
    *Value = Stack->elems[Stack->actSize - 1];
    Stack->actSize = Stack->actSize - 1;
    if (Stack->actSize * 4 <= Stack->memSize)
    {
        int h = ReallocLess(Stack);
        if (h == -1)
        {
            //do nothing
        }
    }
    return 0;
}

int StPeek(CC_STACK *Stack, int *Value)
{
    if (Stack == NULL)
    {
        return -1;
    }
    if (Value == NULL)
    {
        return -1;
    }
    if (Stack->actSize == 0)
    {
        return -1;
    }
    *Value = Stack->elems[Stack->actSize-1];
    return 0;
}

int StIsEmpty(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    if (Stack->actSize == 0)
    {
        return 1;
    }
    return 0;
}

int StGetCount(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    return Stack->actSize;
}

int StClear(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    int *aux = realloc(Stack->elems, sizeof(int)*STSIZE);
    if (aux == NULL)
    {
        return -1;
    }
    Stack->elems = aux;
    Stack->actSize = 0;
    Stack->memSize = STSIZE;
    return 0;
}

int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush)
{
    if (Stack == NULL)
    {
        return -1;
    }
    if (StackToPush == NULL)
    {
        return -1;
    }
    int actSizeInit = Stack->actSize;
    for (int i = 0; i < StackToPush->actSize; i++)
    {
        int h = StPush(Stack, StackToPush->elems[i]);
        if (h == -1)
        {
            Stack->actSize = actSizeInit;
            if (Stack->actSize * 4 <= Stack->memSize)
            {
                h = ReallocLess(Stack);
                if (h == -1)
                {
                    //do nothing
                }
            }
            return -1;
        }
    }
    return 0;
}
