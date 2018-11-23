#include <stdlib.h>
#include <stdio.h>

const static size_t DEFAULT_BUCKET_SIZE = 7;
const static size_t DEFAULT_TABLE_SIZE = 127;
size_t TABLE_SIZE;

// Function declarations
int hashFunction(char *,  int);

typedef struct HashItem {
   char airline[2];
   int count;
   int key;
   hnode *next;
} hnode;

hnode* hashTable[DEFAULT_TABLE_SIZE];
hnode* delNode;
hnode* newNode;

int main(int argc, const char* argv[])
{
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
    while ( hashTable[hash]->key 1= NULL && hashTable[hash]->key != -1 ) {
            hash += 1;
            hash %= TABLE_SIZE;
    }
    hashTable[hash] = data;
}

// Enforcing linear probing
hnode *delete(hnode *data) {
    int key = data->key;

    int hash = hashFunction(key);
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

/*Hash Function*/
int hashFunction(char *s,  int T) {

   /* The parameter s represents the symbol to be hashed and  */
   /* the parameter T represents the size of the hash table.  */
   /* The function returns the hash value for the symbol s.   */

   /* String s is assumed to be terminated with '\0'.         */
   /* It is also assumed that T is at least 2. The returned   */
   /* hash value is an integer in the range 0 to T-1.         */

   /* The function computes the hash value using arithmetic   */
   /* based on powers of the BASE value defined below.        */

   #define  BASE   127

   int h = 0;     /* Will hold the hash value at the end. */
   int temp;      /* Temporary.                           */

   /* The hash value is computed in the for loop below. */
   for (;  *s != 0;  s++) {
       temp = (BASE * h + *s);
       if (temp < 0) temp = -temp;
       h = temp % T;
   }

   /* The hash value computation is complete. So, */
   return h;

} /* End of hash function */


/*
typedef struct destport{
    char key[3];
    int val;
    dport *left;
    dport *right;
} dport;
 */
