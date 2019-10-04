#pragma once
#include "ccvector.h"

#define MODULO2 100000007
#define MODULO3 100000009

#define BASE1 31
#define BASE2 71
#define BASE3 101

typedef struct _NODK
{
    struct _NODK *Next;
    struct _NODK *Prev;
    char *s;
} NODK;
typedef struct _NODH
{
    struct _NODH *Next;
    struct _NODH *Prev;
    int Hash2;
    int Hash3;
    int Value;
    NODK *Pos;
} NODH;
typedef struct _CC_HASH_TABLE { 
    NODH **Table;
    NODK *kHead;
    NODK *currK;
    int countKeys;
    int Size;
} CC_HASH_TABLE;
  
int HtCreate(CC_HASH_TABLE** HashTable); 
int HtDestroy(CC_HASH_TABLE** HashTable); 
 
int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value); 
int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int *Value); 
int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key); 
int HtHasKey(CC_HASH_TABLE* HashTable, char* Key); 
int HtGetNthKey(CC_HASH_TABLE* HashTable, int Index, char** Key); 
int HtClear(CC_HASH_TABLE* HashTable); 
int HtGetKeyCount(CC_HASH_TABLE* HashTable);  
