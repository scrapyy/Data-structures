#include "cchashtable.h"
#include "common.h"
#include<stdlib.h>
#define LIMIT 50
#define HSIZE 666013


static int GetNextPrime(int Value)
{
    int ok = -1;
    while (ok == -1)
    {
        ok = 0;
        Value = Value + 1;
        for (int i = 2; i*i <= Value; i++)
        {
            if (Value%i == 0)
            {
                ok = -1;
                break;
            }
        }
    }
    return Value;
}

static int HtReCreate(CC_HASH_TABLE** HashTable, int Size)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    *HashTable = malloc(sizeof(CC_HASH_TABLE));
    if (HashTable == NULL)
    {
        return -1;
    }
    (*HashTable)->Size = Size;
    (*HashTable)->Table = malloc(sizeof(NODH)*Size);
    if ((*HashTable)->Table == NULL)
    {
        HtDestroy(HashTable);
        return -1;
    }
    for (int i = 0; i < (*HashTable)->Size; i++)
    {
        (*HashTable)->Table[i] = NULL;
    }
    (*HashTable)->kHead = NULL;
    (*HashTable)->currK = NULL;
    (*HashTable)->countKeys = 0;
    return 0;
}

static int CopyHash(CC_HASH_TABLE *Source, CC_HASH_TABLE *Destination)
{
    for (int i = 0; i < Source->Size; i++)
    {
        NODH *start = Source->Table[i];
        while (start != NULL)
        {
            int h = HtSetKeyValue(Destination, start->Pos->s, start->Value);
            if (h == -1)
            {
                return -1;
            }
            start = start->Next;
        }
    }
    return 0;
}

static int Resize(CC_HASH_TABLE *HashTable)
{
    CC_HASH_TABLE *aux;
    int newsize = GetNextPrime(HashTable->Size * 2);
    int h = HtReCreate(&aux, newsize);
    if (h == -1)
    {
        return -1;
    }
    h = CopyHash(HashTable, aux);
    if (h == -1)
    {
        HtDestroy(&aux);
        return -1;
    }
    NODH **help = realloc(HashTable->Table, sizeof(NODH)*newsize);
    if (help == NULL)
    {
        HtDestroy(&aux);
        return -1;
    }
    HashTable->Table = help;
    for (int i = HashTable->Size; i < newsize; i++)
    {
        HashTable->Table[i] = NULL;
    }
    HashTable->Size = newsize;
    HtClear(HashTable);
    CopyHash(aux, HashTable);
    HtDestroy(&aux);
    return 0;
}

static int GetHash(char *s, int Base, int Modulo)
{
    long long hash = 0;
    for (int i = 0; s[i]; i++)
    {
        hash = hash * Base;
        hash = hash + s[i];
        hash = hash % Modulo;
    }
    return (int)hash;
}

static int CreateNodH(NODH **Nod)
{
    *Nod = malloc(sizeof(NODH));
    if (*Nod == NULL)
    {
        return -1;
    }
    (*Nod)->Next = NULL;
    (*Nod)->Prev = NULL;
    return 0;
}

static int CreateNodK(NODK **Nod)
{
    *Nod = malloc(sizeof(NODK));
    if (*Nod == NULL)
    {
        return -1;
    }
    (*Nod)->Next = NULL;
    (*Nod)->Prev = NULL;
    return 0;
}

static int ExistKey(CC_HASH_TABLE *HashTable, int h1, int h2, int h3,int *Nr)
{
    NODH *aux = HashTable->Table[h1];
    *Nr = 0;
    while (aux != NULL)
    {
        if (aux->Hash2 == h2 && aux->Hash3 == h3)
        {
            return -1;
        }
        aux = aux->Next;
        *Nr = *Nr + 1;
    }
    return 0;
}

int HtCreate(CC_HASH_TABLE** HashTable)
{
    int h = HtReCreate(HashTable, HSIZE);
    if (h == -1)
    {
        return -1;
    }
    return 0;
}

int HtDestroy(CC_HASH_TABLE** HashTable)
{
    if (HashTable == NULL || *HashTable == NULL)
    {
        return -1;
    }
    for (int i = 0; i < (*HashTable)->Size; i++)
    {
        NODH *start = (*HashTable)->Table[i], *aux;
        while (start != NULL)
        {
            aux = start->Next;
            free(start);
            start = NULL;
            start = aux;
        }
    }
    NODK *go = (*HashTable)->kHead, *auxgo;
    while (go != NULL)
    {
        auxgo = go->Next;
        free(go);
        go = NULL;
        go = auxgo;
    }
    free((*HashTable)->Table);
    (*HashTable)->Table = NULL;
    free(*HashTable);
    *HashTable = NULL;
    return 0;
}

int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    if (Key == NULL)
    {
        return -1;
    }
    int h1 = GetHash(Key, BASE1, HashTable->Size);
    int h2 = GetHash(Key, BASE2, MODULO2);
    int h3 = GetHash(Key, BASE3, MODULO3);
    int Nr;
    if (ExistKey(HashTable, h1, h2, h3, &Nr) == -1)
    {
        return -1;
    }
    NODK *nodk;
    int h = CreateNodK(&nodk);
    if (h == -1)
    {
        return -1;
    }
    NODH *nodh;
    h = CreateNodH(&nodh);
    if (h == -1)
    {
        free(nodk);
        nodk = NULL;
        return -1;
    }
    int l = 0;
    for (int i = 0; Key[i] != 0; i++)
    {
        l = l + 1;
    }
    nodk->s = malloc(sizeof(char)*l);
    if (nodk->s == NULL)
    {
        free(nodk);
        free(nodh);
        nodk = NULL;
        nodh = NULL;
        return -1;
    }
    nodk->s[l] = 0;
    for (int i = 0; Key[i] != 0; i++)
    {
        nodk->s[i] = Key[i];
    }
    if (HashTable->kHead == NULL)
    {
        HashTable->kHead = nodk;
    }
    else
    {
        HashTable->kHead->Prev = nodk;
        nodk->Next = HashTable->kHead;
        HashTable->kHead = nodk;
    }
    nodh->Hash2 = h2;
    nodh->Hash3 = h3;
    nodh->Pos = nodk;
    nodh->Value = Value;
    if (HashTable->Table[h1] == NULL)
    {
        HashTable->Table[h1] = nodh;
    }
    else
    {
        HashTable->Table[h1]->Prev = nodh;
        nodh->Next = HashTable->Table[h1];
        HashTable->Table[h1] = nodh;
    }
    HashTable->countKeys = HashTable->countKeys + 1;
    if (Nr == LIMIT)
    {
        Resize(HashTable);
        
    }
    return 0;
}

int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int *Value)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    if (Key == NULL)
    {
        return -1;
    }
    if (Value == NULL)
    {
        return -1;
    }
    int h1 = GetHash(Key, BASE1, HashTable->Size);
    int h2 = GetHash(Key, BASE2, MODULO2);
    int h3 = GetHash(Key, BASE3, MODULO3);
    NODH *start = HashTable->Table[h1];
    while (start != NULL)
    {
        if (start->Hash2 == h2 && start->Hash3 == h3)
        {
            *Value = start->Value;
            return 0;
        }
        start = start->Next;
    }
    return -1;
}

int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    if (Key == NULL)
    {
        return -1;
    }
    int h1 = GetHash(Key, BASE1, HashTable->Size);
    int h2 = GetHash(Key, BASE2, MODULO2);
    int h3 = GetHash(Key, BASE3, MODULO3);
    NODH *start = HashTable->Table[h1];
    while (start != NULL)
    {
        if (start->Hash2 == h2 && start->Hash3 == h3)
        {
            if (start->Pos->Prev != NULL && start->Pos->Next != NULL)
            {
                start->Pos->Prev->Next = start->Pos->Next;
                start->Pos->Next->Prev = start->Pos->Prev;
                free(start->Pos);
                start->Pos = NULL;
            }
            else if (start->Pos->Prev != NULL)
            {
                start->Pos->Prev->Next = NULL;
                free(start->Pos);
                start->Pos = NULL;
            }
            else if (start->Pos->Next != NULL)
            {
                start->Pos->Next->Prev = NULL;
                HashTable->kHead = start->Pos->Next;
                free(start->Pos);
                start->Pos = NULL;
            }
            if (start->Pos == HashTable->kHead)
            {
                HashTable->kHead = start->Pos->Next;
                free(start->Pos);
                start->Pos = NULL;
            }
            if (start->Prev != NULL && start->Next!=NULL)
            {
                start->Prev->Next = start->Next;
                start->Next->Prev = start->Prev;
                free(start);
                start = NULL;
            }
            else if (start->Prev != NULL)
            {
                start->Prev->Next = NULL;
                free(start);
                start = NULL;
            }
            else if (start->Next != NULL)
            {
                start->Next->Prev = NULL;
                HashTable->Table[h1] = start->Next;
                free(start);
                start = NULL;
            }
            if (start == HashTable->Table[h1])
            {
                HashTable->Table[h1] = start->Next;
                free(start);
                start = NULL;
            }
            HashTable->countKeys = HashTable->countKeys - 1;
            return 0;
        }
        start = start->Next;
    }
    return -1;
}

int HtHasKey(CC_HASH_TABLE* HashTable, char* Key)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    if (Key == NULL)
    {
        return -1;
    }
    int h1 = GetHash(Key, BASE1, HashTable->Size);
    int h2 = GetHash(Key, BASE2, MODULO2);
    int h3 = GetHash(Key, BASE3, MODULO3);
    int nr;
    if (ExistKey(HashTable, h1, h2, h3, &nr) == -1)
    {
        return 1;
    }
    return 0;
}

int HtGetNthKey(CC_HASH_TABLE* HashTable, int Index, char** Key)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    if (Key == NULL)
    {
        return -1;
    }
    if (HashTable->currK == NULL)
    {
        HashTable->currK = HashTable->kHead;
    }
    int l = 0;
    for (int i = 0; HashTable->currK->s[i] != 0; i++)
    {
        l = l + 1;
    }
    (*Key) = malloc(sizeof(char)*l);
    if (*Key == NULL)
    {
        return -1;
    }
    for (int i = 0; HashTable->currK->s[i] != 0; i++)
    {
        (*Key)[i] = HashTable->currK->s[i];
    } 
    (*Key)[l] = 0;
    HashTable->currK = HashTable->currK->Next;
    CC_UNREFERENCED_PARAMETER(Index);
    return 0;
}

int HtClear(CC_HASH_TABLE* HashTable)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    for (int i = 0; i < HashTable->Size; i++)
    {
        NODH *start = HashTable->Table[i], *aux;
        HashTable->Table[i] = NULL;
        while (start != NULL)
        {
            aux = start->Next;
            free(start);
            start = NULL;
            start = aux;
        }
    }
    NODK *go = HashTable->kHead, *auxgo;
    HashTable->kHead = NULL;
    while (go != NULL)
    {
        auxgo = go->Next;
        free(go);
        go = NULL;
        go = auxgo;
    }
    HashTable->countKeys = 0;
    HashTable->Size = HashTable->Size;
    return 0;
}

int HtGetKeyCount(CC_HASH_TABLE* HashTable)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    return HashTable->countKeys;
}
