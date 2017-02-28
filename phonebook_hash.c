#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "phonebook_hash.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastName[], entry *pHead[])
{
    unsigned int hash = BKDRHash(lastName);
    entry *temp;
    if (pHead[hash]->pNext)
        temp = pHead[hash]->pNext;
    else
        return NULL;
    while (temp) {
        if (strcasecmp(lastName, temp->lastName) == 0)
            return temp;
        temp = temp->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e[])
{
    unsigned int hash = BKDRHash(lastName);

    /* allocate memory for the new entry and put lastName */
    e[hash]->pNext = (entry *) malloc(sizeof(entry));
    e[hash] = e[hash]->pNext;
    strcpy(e[hash]->lastName, lastName);
    e[hash]->pNext = NULL;
    return e[hash];
}

unsigned int BKDRHash(char lastName[])
{
    unsigned int hash = 0;
    int R=13;
    for (int i = 0; i < strlen(lastName); i++)
        hash = (R * hash + lastName[i])%SIZE;
    return hash;
}
unsigned int ELFhash(char lastName[])
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    int i = 0;
    while (i < strlen(lastName)) {
        hash = (hash << 4) + lastName[i];
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        }
        i++;
    }
    hash = (hash & 0x7FFFFFFF) % SIZE;

    return hash;
}

unsigned int PJWhash(char lastName[])
{
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters    = (unsigned int)((BitsInUnignedInt  * 3) / 4);
    unsigned int OneEighth        = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits         = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    unsigned int hash             = 0;
    unsigned int test             = 0;

    int i = 0;
    while (i < strlen(lastName)) {
        hash = (hash << OneEighth) + lastName[i];
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
        i++;
    }
    hash = (hash & 0x7FFFFFFF) % SIZE;

    return hash;
}

unsigned int APHash(char lastName[])
{
    unsigned int hash = 0;
    int i = 0;

    while (i < strlen(lastName)) {
        if ((i & 1) == 0) {
            hash ^= ((hash << 7) ^ lastName[i] ^ (hash >> 3));
        } else {
            hash ^= (~((hash << 11) ^ lastName[i] ^ (hash >> 5)));
        }
        i++;
    }
    hash = (hash & 0x7FFFFFFF) % SIZE;

    return hash;
}

unsigned int SDBMhash(char lastName[])
{
    unsigned int hash = 0;

    int i = 0;
    while (i < strlen(lastName)) {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = lastName[i] + (hash << 6) + (hash << 16) - hash;
        i++;
    }
    hash = (hash & 0x7FFFFFFF) % SIZE;

    return hash;
}

unsigned int RShash(char lastName[])
{
    unsigned int y = 375851;
    unsigned int x = 68639;
    unsigned int hash = 0;

    int i = 0;
    while (i < strlen(lastName)) {
        hash = hash * x + lastName[i];
        x *= y;
        i++;
    }
    hash = (hash & 0x7FFFFFFF) % SIZE;

    return hash;
}

unsigned int JShash(char lastName[])
{
    unsigned int hash = 1313245911;

    int i = 0;
    while (i < strlen(lastName)) {
        hash ^= ((hash << 5) + lastName[i] + (hash >> 2));
        i++;
    }
    hash = (hash & 0x7FFFFFFF) % SIZE;

    return hash;
}

void free_list(entry *e)
{
    entry *temp;
    while ((temp = e) != NULL) {
        e = e->pNext;
        free(temp);
    }
}
