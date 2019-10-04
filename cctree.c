#include "cctree.h"
#include "common.h"
#include<stdlib.h>

static int CreateNod(NOD **Nod)
{
    (*Nod) = malloc(sizeof(NOD));
    if (*Nod == NULL)
    {
        return -1;
    }
    (*Nod)->Lson = NULL;
    (*Nod)->Rson = NULL;
    (*Nod)->Dad = NULL;
    (*Nod)->Height = 0;
    return 0;
}

static int FindNod(CC_TREE *Tree, NOD **Nod, int *type, int Value)
{
    NOD *start = Tree->Root;
    while (start != NULL && start->Value != Value)
    {
        if (start->Value < Value)
        {
            start = start->Rson;
            *type = 1;
        }
        else
        {
            start = start->Lson;
            *type = 0;
        }
    }
    if (start == NULL)
    {
        return -1;
    }
    *Nod = start;
    return 0;
}


int TreeCreate(CC_TREE **Tree)
{
    if (Tree == NULL)
    {
        return -1;
    }
    (*Tree) = malloc(sizeof(CC_TREE));
    if ((*Tree) == NULL)
    {
        return -1;
    }
    (*Tree)->Root = NULL;
    (*Tree)->Size = 0;
    (*Tree)->Height = 0;
    return 0;
}

int TreeDestroy(CC_TREE **Tree)
{
    if (Tree == NULL || *Tree == NULL)
    {
        return -1;
    }
    TreeClear(*Tree);
    free(*Tree);
    *Tree = NULL;
    return 0;
}

int TreeInsert(CC_TREE *Tree, int Value)
{
    if (Tree == NULL)
    {
        return -1;
    }
    if (Tree->Root == NULL)
    {
        NOD *Nod;
        int h = CreateNod(&Nod);
        if (h == -1)
        {
            return -1;
        }
        Nod->Value = Value;
        Nod->Height = 0;
        Nod->Dad = NULL;
        Tree->Root = Nod;
        Tree->Size = 1;
        return 0;
    }
    NOD *start = NULL, *next = Tree->Root;
    while (next != NULL)
    {
        start = next;
        if (start->Value >= Value)
        {
            next = start->Lson;
        }
        else
        {
            next = start->Rson;
        }
    }
    NOD *Nod;
    int h = CreateNod(&Nod);
    if (h == -1)
    {
        return -1;
    }
    Nod->Value = Value;
    Nod->Dad = start;
    if (start->Value >= Value)
    {
        start->Lson = Nod;
        Nod->Height = -1;
    }
    else
    {
        start->Rson = Nod;
        Nod->Height = 1;
    }
    Tree->Size = Tree->Size + 1;
    return 0;
}

int TreeRemove(CC_TREE *Tree, int Value)
{
    if (Tree == NULL)
    {
        return -1;
    }
    NOD *start;
    int type = 0;
    int h = FindNod(Tree, &start, &type, Value);
    if (h == -1)
    {
        return -1;
    }
    NOD *aux;
    while (h == 0)
    {
        if (start->Rson == NULL && start->Lson == NULL)
        {
            if (Tree->Root == start)
            {
                Tree->Root = NULL;
            }
            else
            {
                if (type == 0)
                {
                    start->Dad->Lson = NULL;
                }
                else
                {
                    start->Dad->Rson = NULL;
                }
            }
            free(start);
        }
        else if (start->Rson == NULL)
        {
            if (Tree->Root == start)
            {
                Tree->Root = start->Lson;
                free(start);
                Tree->Root->Dad = NULL;
            }
            else
            {
                start->Lson->Dad = start->Dad;
                if (type == 0)
                {
                    start->Dad->Lson = start->Lson;
                }
                else
                {
                    start->Dad->Rson = start->Lson;
                }
                free(start);
            }
        }
        else
        {
            aux = start->Rson;
            int tson = 1;
            while (aux->Lson != NULL)
            {
                aux = aux->Lson;
                tson = 0;
            }
            if (aux->Rson != NULL)
            {
                aux->Rson->Dad = aux->Dad;
            }
            if (tson == 0)
            {
                aux->Dad->Lson = aux->Rson;
            }
            else
            {
                aux->Dad->Rson = aux->Rson;
            }
            start->Value = aux->Value;
            free(aux);
            aux = NULL;
        }
        h = FindNod(Tree, &start, &type, Value);
    }
    Tree->Size = Tree->Size - 1;
    return 0;
}

int TreeContains(CC_TREE *Tree, int Value)
{
    if (Tree == NULL)
    {
        return -1;
    }
    NOD *start = Tree->Root;
    while (start != NULL)
    {
        if (start->Value > Value)
        {
            start = start->Lson;
        }
        else if (start->Value < Value)
        {
            start = start->Rson;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

int TreeGetCount(CC_TREE *Tree)
{
    if (Tree == NULL)
    {
        return -1;
    }
    return Tree->Size;
}

int TreeGetHeight(CC_TREE *Tree)
{
    if (Tree == NULL)
    {
        return -1;
    }
    if (Tree->Root == NULL)
    {
        return 0;
    }
    return Tree->Height;
}

int TreeClear(CC_TREE *Tree)
{
    if (Tree == NULL)
    {
        return -1;
    }
    if (Tree->Size == 0)
    {
        Tree->Root = NULL;
        return 0;
    }
    NOD **st;
    st = malloc(sizeof(NOD)*Tree->Size);
    if (st == NULL)
    {
        return -1;
    }
    int vf = 0;
    st[0] = Tree->Root;
    for (int i = 0; i <= vf; i++)
    {
        if (st[i]->Lson != NULL)
        {
            vf = vf + 1;
            st[vf] = st[i]->Lson;
        }
        if (st[i]->Rson != NULL)
        {
            vf = vf + 1;
            st[vf] = st[i]->Rson;
        }
        free(st[i]);
        st[i] = NULL;
    }
    free(st);
    st = NULL;
    Tree->Root = NULL;
    return 0;
}



