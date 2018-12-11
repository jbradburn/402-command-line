#include <stdlib.h>
#include <stdio.h>

#include "srchindx.h"

// Function declarations
int hashFunction(char *, int);
llnode *search(char *);
void insert(char *, char *, int);
typedef struct LinkedListNode
{
    char *key;
    char *airline;
    int count;
    struct LinkedListNode *next;
} llnode;

size_t TABLE_SIZE;
llnode **hashTable;
llnode *delNode;
llnode *newNode;

int main(int argc, const char *argv[])
{
    TABLE_SIZE = DEFAULT_TABLE_SIZE;
    llnode **hashTable = malloc(DEFAULT_TABLE_SIZE * sizeof(llnode *));
    if (argc < 2 || argc > 3)
    {
        printf("Invalid number of arguments: %d", argc);
        free(hashTable);
        return 0;
    }

    free(hashTable);
    return 0;
}

// Enforcing hash table with linear probing
llnode *search(char *key)
{
    int hash = hashFunction(key, TABLE_SIZE);

    while (hashTable[hash] != NULL)
    {
        if (strcmp(hashTable[hash]->key, key) == 0)
            return hashTable[hash]; // If key matches, then return match
        hash += 1;                  // Otherwise increment to next available space in table
        hash %= TABLE_SIZE;         // or wrap around table
    }
    return NULL;
}

// Enforcing linear probing
void insert(char *key, char *airline, int count)
{
    llnode *newNode = malloc(sizeof(llnode));
    newNode->key = key;
    newNode->airline = airline;
    newNode->count = 0;
    newNode->next = NULL;

    int hash = hashFunction(key, TABLE_SIZE);

    // While hash has collision, -> linear probe
    while (hashTable[hash]->key != NULL && hashTable[hash]->key != -1)
    {
        // If new node key = key at hash location
        if (strcmp(hashTable[hash]->key, key) == 0)
        {
            llnode *temp = hashTable[hash];
            // Insert new node at end of bucket
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newNode;
            return;
        }
        hash += 1;
        hash %= TABLE_SIZE;
    }
    hashTable[hash] = malloc(sizeof(llnode));
    hashTable[hash] = newNode;
}

/*
    HASH TABLE
 ============================================
 */

/*Hash Function*/
int hashFunction(char *s, int T)
{

    /* The parameter s represents the symbol to be hashed and  */
    /* the parameter T represents the size of the hash table.  */
    /* The function returns the hash value for the symbol s.   */

    /* String s is assumed to be terminated with '\0'.         */
    /* It is also assumed that T is at least 2. The returned   */
    /* hash value is an integer in the range 0 to T-1.         */

    /* The function computes the hash value using arithmetic   */
    /* based on powers of the BASE value defined below.        */

#define BASE 127

    int h = 0; /* Will hold the hash value at the end. */
    int temp;  /* Temporary.                           */

    /* The hash value is computed in the for loop below. */
    for (; *s != 0; s++)
    {
        temp = (BASE * h + *s);
        if (temp < 0)
        {
            temp = -temp;
        }
        h = temp % T;
    }

    /* The hash value computation is complete. So, */
    return h;
} /* End of hash function */
