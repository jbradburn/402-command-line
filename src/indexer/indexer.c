#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
// Directory libraries
#include <dirent.h>
#include <ftw.h>

#include "tokenizer.h"

#define DEFAULT_IDX_LENGTH  50      // Default limit for index length (max number of discrete airports)
#define DEFAULT_IDX_SIZE    1000    // Default limit for node structs in file
#define AIRPORT_NAME_LENGTH 3       // Char length of airport codes
#define AIRLINE_NAME_LENGTH 2

const char *ROOT_DIR = "../../data";

typedef struct AirportNode {
    char *airport;  // airport name
    struct inode *head;   // ptr to first airline for airport
    int children;
} anode;
typedef struct IndexNode {
    char *name; // airline name
    int count;  // number of flights on airline for head airport
    struct inode *next;
} inode;

// Struct print fcts
int printAnode(struct AirportNode*);
int printInode(struct IndexNode*);

// Function declarations
anode *addAirport(char*);
inode *addAirline(char*, char*);

anode* searchAirport(char*);
inode* searchAirline(char*,  char*);

int f_name(const char *, const struct stat *, int);
void index_dir();

char*** tokenize(char*);    //

anode** aport_array;    // Array to store complete index data before writing to file
inode* empty_item;      // Empty airline node in case of delete function
char** data_buf[3];     // Array to collect values of 

int main() {

    int index = 0;

    // DUMMY DATA - POPULATE LIST
    char *letters[5] = {"AAA", "BBB", "CCC", "DDD", "EEE"};
    char *lineletters[5] = {"FF", "GG", "HH", "II", "JJ"};

    // Initialize first airport in table
    aport_array = malloc( 10*sizeof(aport_array) );
    for (int i = 0; i < 5; i++ ) {
        addAirport(letters[i]);
        addAirline(letters[i], lineletters[i]);
        /*
        TEST ARGUMENTS
        addAirline(letters[i], "AA");
        addAirline(letters[i], "AA");
        addAirline(letters[i], "ZZ");
        addAirline(letters[i], "AA");
        addAirline(letters[i], lineletters[i]);
        */
        
    }

    /*
    // Temp code to add inode
    inode* temp = malloc( sizeof(inode) );
    temp->name = malloc( AIRLINE_NAME_LENGTH );
    temp->name = "AA";
    temp->count = 0;
    temp->next = NULL;
    inode* idx = aport_array[0]->head;
    idx->next = temp;
    aport_array[0]->children += 1;
     */
    for (int i = 0; i < 5; i++ ) {
        printAnode(aport_array[i]);
        //printInode(searchAirline(letters[i], "AA"));
        //printInode(searchAirline(letters[i], "FF"));
        listAirline(letters[i]);
    }




    //aport_array[0]->airport = "LAX";
    //aport_array[0]->head = temp;
    //aport_array[0]->children += 1;

    //printAnode(aport_array[0]);
    
    free(aport_array);
    return 0;
}

/*
    Search airports in O(n) 
 */
anode* searchAirport(char* airport_name) {
    
    if ( aport_array ) {
        size_t idx = 0;
        anode* temp = malloc( sizeof( anode* ) );
        temp = aport_array[idx];
        while ( temp ) {
            if ( temp->airport == airport_name )
                return temp;
            temp = *(aport_array + idx);
            ++idx;
        }
    }
    return NULL;
}

inode* searchAirline(char* airport_name,  char* airline_name) {
    anode* temp;
    if( temp = searchAirport(airport_name) ) {
        size_t idx = 0;
        inode* itemp = malloc( sizeof( inode* ) );
        itemp = temp->head;
        while( itemp ) {
            if ( itemp->name == airline_name ) 
                return itemp;
            itemp = itemp->next;
        }
    }
    return NULL;
}

/*
    Add an airport in O(n)
 */
anode *addAirport(char* airport_name) {
    int idx = length(aport_array);
    aport_array[ idx ] = malloc( sizeof( anode* ) );    // Allocate memory for next airport node

    aport_array[idx]->airport = malloc( AIRPORT_NAME_LENGTH*sizeof(char) );
    aport_array[idx]->head = malloc( sizeof( inode ) );

    aport_array[idx]->airport = airport_name;

    return aport_array[idx];
}

/*
    Add/incr airline in O(n)
 */
inode *addAirline(char* airport_name, char* airline_name) {
    // Allocate memory to local structs
    anode* airport;// = malloc( sizeof( anode* ) );
    inode* airline;// = malloc( sizeof( inode* ) );
    inode* idx = malloc( sizeof( inode* ) );

    if ( airport = searchAirport(airport_name) ) {
        assert(airport);
        if ( airline = searchAirline( airport_name, airline_name) ) {
            incAirline(airline);
            printf("\nDUPLICATE\n");
            return airline;
        }
        printf("Adding airline: %s\n", airline_name);

        
        idx = airport;
        airline = malloc( sizeof( inode* ));
        airline->name = (char*)malloc( AIRLINE_NAME_LENGTH );  
        airline->name = airline_name;
        airline->count = 1;
        airline->next = NULL;
        
        if ( airport->head != NULL ) {
            idx = airport->head;
            while ( idx->next != NULL ) {
                idx = idx->next;
            }
            idx->next = airline;
        }
        else {
            airport->head = airline;
        }

        //airline->next = malloc( sizeof(airline->next) );
        return airline;
    
    }
    return NULL;
}

/*
    Increment airport -> airline count
 */
int incAirline(inode* airline) {
    return airline->count += 1;
}

int listAirline(char* airport) {
    anode* temp = searchAirport(airport);
    inode* itemp = temp->head;
    while (itemp != NULL) {
        printInode(itemp);
        itemp = itemp->next;
    }
    return 1;
}


/*
    Get length of anode array;
 */
int length(anode** array) {
    int s = 0;
    while (array[s] != NULL) s++;
    return s;
}

int printAnode(anode* airport) {
    if (airport != NULL) {
        printf("\nAirport: %s\nChildren: %d\nHead: %p\n", airport->airport, airport->children, airport->head );
        return 1;
    }
    return 0;
}


int printInode(inode* airline) {
    if (airline != NULL) {
        printf("\nAirline: %s\nCount: %d\nThis: %p\nNext: %p\n", airline->name, airline->count, airline, airline->next );
        return 1;
    }
    return 0;
}

/*
 * Function to point for ftw
 */
int f_name(const char *f_name, const struct stat *status, int f_type) {
    if( f_type == FTW_NS )
        return 0;
    
    if( f_type == FTW_F )
        printf("%-30s\n", f_name);
    return 0;
}

int create_idx(const char *f_name, const struct stat *status, int f_type) {
    if( f_type == FTW_NS )
        return 0;
    
    if( f_type == FTW_F )
        printf("%-30s\n", f_name);

    return 0;
}

// Function wrapper for ftw, don't really need this, can just call ftw in main instead.
void index_dir() {

    ftw(ROOT_DIR, (*f_name), 2);
}

/*
 * Wrapper function to initialize and run tokenizer function from tokenizer.c
 */
char*** tokenize(char* fname) {
    // TOKENIZE

    char ***DATA;
	FILE* fp = fopen("../../data/AA.txt", "r");	// Open file as 'fp'
	assert(fp);	
	printf("\nFile open success.\n");
	DATA = (char***)malloc( MAX_SIZE*sizeof(char**) );
	DATA = tokenizeFile(fp);
    return DATA;
    // END TOKENIZE
}

void indexTokenData(char*** data) {
    return 0;
}
