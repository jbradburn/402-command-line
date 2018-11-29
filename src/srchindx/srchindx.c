#include <stdlib.h>
#include <stdio.h>

#include "srchindx.h"

// Function declarations
int hashFunction(char*,  int);

typedef struct HashItem {
   char airline[2];
   int count;
   int key;
   struct hnode *next;
} hnode;

size_t TABLE_SIZE;
hnode** hashTable;
hnode* delNode;
hnode* newNode;

int main(int argc, const char* argv[])
{   
    TABLE_SIZE = DEFAULT_TABLE_SIZE;
    hnode** hashTable = malloc( DEFAULT_TABLE_SIZE*sizeof(hnode*) );
    if ( argc < 2 || argc > 3 ) {
        printf("Invalid number of arguments: %d", argc);
        free(hashTable);
        return 0;
    }
    free(hashTable);
    return 0;
}

// Enforcing hash table with linear probing
hnode *search(char* key) {
    int hash = hashFunction(key, TABLE_SIZE);   

    while ( hashTable[hash] != NULL ) {
        if ( hashTable[hash]->key == key ) return hashTable[hash];  // If key matches, then return match
        hash += 1;          // Otherwise increment to next available space in table
        hash %= TABLE_SIZE; // or wrap around table
    }
    return NULL;
}

// Enforcing linear probing
void insert(char* key, hnode* data) {
    hnode *newNode = data;

    int hash = hashFunction(key, TABLE_SIZE);
    while ( hashTable[hash]->key != NULL && hashTable[hash]->key != -1 ) {
            hash += 1;
            hash %= TABLE_SIZE;
    }
    hashTable[hash] = data;
}

// Enforcing linear probing
hnode *delete(hnode *data) {
    int key = data->key;

    int hash = hashFunction(key, TABLE_SIZE);
    while ( hashTable[hash]->key != NULL ) {
        if ( hashTable[hash]->key == key ) {
            hnode *temp = hashTable[hash];
            hashTable[hash] = delNode;
            return temp;
        }
        hash += 1;
        hash %= TABLE_SIZE;
    }
    return NULL;
}




/*
typedef struct destport{
    char key[3];
    int val;
    dport *left;
    dport *right;
} dport;
 */
