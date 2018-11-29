#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
// Directory libraries
#include <dirent.h>

#include "tokenizer.h"
#include "../srchindx/srchindx.h"

#define AIRPORT_NAME_SIZE 4
#define HASH_TABLE_BASE_SIZE 100
#define DEFAULT_AIRPORT_CAPACITY 128

const char *ROOT_DIR = "../../data";

typedef struct inode {
    char *name; // airline name
    int count;  // number of flights on airline for head airport
    struct inode* next;
} inode;

typedef struct anode {
    char *airport;          // airport name
    struct inode* data;    // airlines for airport
    size_t capacity;        // max number of elements
    size_t size;            // num elements
    int children;           // deprecated
} anode;

// Airport lists
int airport_init(anode*, char*, size_t);
int airline_init(inode*, char*);
int airport_resize(anode*, size_t);
int airline_insert(anode*, inode*);
inode* airline_search(anode*, char*);
void prettyPrintAirport(anode*);
void prettyPrintAirline(inode*);

// Hash Table
int hashFunction(char*, int);
int initHashTable();
anode* search(char*);
int insert(anode*);
anode* delete(anode*);
//int prettyPrintHashTable();

// Indexer
int f_name(const char*, const struct stat*, int, struct FTW*);
void index_dir();
token_file_wrapper* tokenize(char*);
int indexTokenData(token_file_wrapper*, const char*);

// HASH TABLE VARS/DEFS
size_t TABLE_SIZE;
anode** hashTable;
anode* delNode;
anode* newNode;

char** key_list;
int key_idx;

int main() {
    key_list = malloc( sizeof(char*)*HASH_TABLE_BASE_SIZE );
    key_idx = 0;
    TABLE_SIZE = HASH_TABLE_BASE_SIZE;
    initHashTable();

    index_dir();

    anode* printy = malloc(sizeof(anode));
    for (int x = 0; x < key_idx; x++) {
        printy = search(key_list[x]);
        prettyPrintAirport( printy );
        print_airlines( printy );
    }

    free(hashTable);
    return 0;
}



int airport_init(anode* airport, char* name, size_t init_capacity) {
    //anode* airport = *input;
    airport->data = (inode*)malloc( sizeof(inode) ); // Allocate mem = sizeof inode * init capacity
    if( airport->data == NULL) {
        return -1;  // Failure
    }
    airport->airport = malloc( AIRPORT_NAME_SIZE );
    strcpy(airport->airport, name);
    strcat(airport->airport, "\0");
    //airport->airport = name;
    airport->capacity = init_capacity;
    airport->size = 0;
    return 0;       // Success
}
int airport_resize(anode* old_memory, size_t new_size) {
    anode* new_memory = realloc(old_memory, new_size);
    if(!new_memory) {
        return -1;
    }
    old_memory = new_memory;
    return 0;
}
int airline_init(inode* airline, char* name) {
    if(!airline) return -1;
    airline->name = name;
    airline->count = 0;
    airline-> next = NULL;
    return 0;
}
int airline_insert(anode* airport, inode* input) {
    inode* head = airport->data;
    while(head->next != NULL) {
        if( strcmp(head->next->name, input->name) == 0 ) {
            head->next->count += 1;
            return 0;
        }
        head = head->next;
    }
    head->next = malloc(sizeof(inode));
    head->next = (inode*)input;
    head->next->count = 1;
    airport->size += 1;
    return 0;
}
void print_airlines(anode* airport) {
    inode* head = airport->data;
    if(head->next) head = head->next;
    printf("========\nFlights in %s\n========\n", airport->airport);
    while(head != NULL) {
        
        prettyPrintAirline(head);
        head = head->next;
    }
    printf("========\nEnd\n========\n");
}

inode* airline_search(anode* airport, char* airline) {
    if(!airport || !airline){
        return -1;
    }
    inode* head = airport->data;
    while( head->next != NULL ) {
        if( strcmp(head->next->name, airline) == 0 ) {
            return head->next;
        }
        head = head->next;
    }
    return NULL;
}
void prettyPrintAirport(anode* airport) {
    if(airport) {
        printf("\nAirport: %s\nCapacity: %ld\nSize: %ld\nHead: %p\n Self: %p\n\n", airport->airport, airport->capacity, airport->size, airport->data, airport);
    }
}
void prettyPrintAirline(inode* airline) {
    if(airline) {
        printf("\nAirline: %s\nFlights: %d\nHead: %p\n\n", airline->name, airline->count, airline);
    }
}
/*
inode* airline_deep_copy(inode* airline) {
    if(!airline) return NULL;
    inode* copy_airline = malloc( sizeof(inode) );
    strcpy(copy_airline->name, airline->name)
    copy_airline->count = 0;
}
*/

/*
    HASH TABLE
 ============================================
 */
/*
int hashFunction(char*, int);
int initHashTable();
anode* search(char*);
void insert(char*, anode*);
anode* delete(anode*);
 */

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

int initHashTable() {
    hashTable = malloc( TABLE_SIZE*sizeof(anode*) );
    if(!hashTable) 
        return -1;
    return 0;
}

// Enforcing hash table with linear probing
anode *search(char* key) {
    int hash = hashFunction(key, TABLE_SIZE);   
    //printf("Hash Table Search Check 1: entered function\n");
    while ( hashTable[hash] != NULL ) {
        //printf("Hash Table Search Check 2: value at hash not NULL\n");
        if ( strcmp(hashTable[hash]->airport, key) != -1 ) {
            //printf("Hash Table Search Check 3: match found\n");
            return hashTable[hash];  // If key matches, then return match
        }
        hash += 1;          // Otherwise increment to next available space in table
        hash %= TABLE_SIZE; // or wrap around table
    }
    return NULL;
}

// Enforcing linear probing
int insert(anode* data) {
    if (!data) return -1;

    // Hash key from input airport
    char* key = data->airport;
    int hash = hashFunction(key, TABLE_SIZE);
    //printf("\n%d\n", hash);
    if (hashTable[hash] != NULL) {
        //printf("COLLISION\n");
        while (hashTable[hash] != NULL) {
            //printf("%s, %s\n", hashTable[hash]->airport, key);
            if ( strcmp(hashTable[hash]->airport, key) == 0 ) {
                //printf(" DUP\n");
                return 0;
            }
            else {
                hash += 1;
                hash %= TABLE_SIZE;
            }
        }
    }

    hashTable[hash] = malloc( sizeof(anode) );
    hashTable[hash] = data;
    key_list[key_idx] = malloc(sizeof(char)*3);
    key_list[key_idx] = key;
    key_idx++;
    //printf(" ADDED\n");
    return 0;
}

// Enforcing linear probing
anode *delete(anode *data) {
    int key = data->airport;

    int hash = hashFunction(key, TABLE_SIZE);
    while ( hashTable[hash]->airport != NULL ) {
        if ( hashTable[hash]->airport == key ) {
            anode *temp = hashTable[hash];
            hashTable[hash] = delNode;
            return temp;
        }
        hash += 1;
        hash %= TABLE_SIZE;
    }
    return NULL;
}

/*
    TOKENIZER
    =======================
 */

/*
 * Function to point for ftw
 */

int f_name(const char *f_name, const struct stat *sb, int f_type, struct FTW *ftw_buf) {
    if( f_type == FTW_NS )
        return 0;
    
    if( f_type == FTW_F ) {
        char *base_name = malloc( 7 );
        char *dupname = malloc( 7 ); 
        (base_name = strrchr(f_name, '/')) ? ++base_name : (base_name = f_name); // Extract base filename from path
        dupname = strdup(base_name);
        char *air_name = malloc ( 3*sizeof(char) );
        air_name = strtok( dupname, ".");

        char*** token_data = (char***)malloc( MAX_SIZE*sizeof(char**) );
        printf("%s\n", base_name);
        token_data = tokenize(f_name);
        indexTokenData(token_data, air_name);
    }
    return 0;
}

// Function wrapper for ftw, don't really need this, can just call ftw in main instead.
void index_dir() {
    int flags = 0;
    flags |= FTW_PHYS;
    nftw(ROOT_DIR, f_name, 20, flags);
    //prettyPrintIndexData();
}

/*
 * Wrapper function to initialize and run tokenizer function from tokenizer.c
 */
token_file_wrapper* tokenize(char* fname) {
    // TOKENIZE

    token_file_wrapper* DATA; 
	FILE* fp = fopen(fname, "r");	// Open file as 'fp'
	assert(fp);	
	printf("\nFile open success.\n");

	DATA = malloc( MAX_SIZE*sizeof(token_file_wrapper) );

    
	DATA = tokenizeFile(fp);
    return DATA;
    // END TOKENIZE
}

int indexTokenData(token_file_wrapper* data_wrapper, const char* line_name) {
    if( !data_wrapper ) return -1;
    int size = data_wrapper->SIZE;
    
    //printf("\nSize: %d\n", size);
    
    char*** data = malloc( MAX_SIZE*sizeof(char**) );
    data = data_wrapper->DATA;

    inode* newAirline;
    anode* newAirport1;
    anode* newAirport2;
    for ( int i = 0; i < size; i++ ) {
        char* airport1 = malloc( 3 );
        airport1 = data[i][1];
        char* airport2 = malloc( 3 );
        airport2 = data[i][2];
        newAirport1 = malloc( sizeof(anode) );
        newAirport2 = malloc( sizeof(anode) );

        // Initialize new airports
        airport_init(newAirport1, airport1, DEFAULT_AIRPORT_CAPACITY);
        //printf("Insert: %s", airport1);
        insert(newAirport1);     // Add airport to hash table
        anode* portsearch1 = malloc(sizeof(anode));
        portsearch1 = search(airport1);

        airport_init(newAirport2, airport2, DEFAULT_AIRPORT_CAPACITY);
        //printf("Insert: %s", airport2);
        insert(newAirport2);     // Add airport to hash table
        anode* portsearch2 = malloc(sizeof(anode));
        portsearch2 = search(airport2);

        //printf("\n\n");
        // Initialize new airline
        newAirline = (inode*)malloc( sizeof(inode) );
        airline_init(newAirline, line_name);  
        airline_insert( portsearch1, newAirline );    // Add airline flight to New Airport Vector 1

        newAirline = (inode*)malloc( sizeof(inode) );
        airline_init(newAirline, line_name);  
        airline_insert( portsearch2, newAirline );    // Add airline flight to New Airport Vector 2
    }
    printf("File index success\n");

    return 0;
}