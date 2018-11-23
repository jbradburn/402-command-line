#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
// Directory libraries
#include <dirent.h>
#include <ftw.h>

#define DEFAULT_IDX_LENGTH  50      // Default limit for index length (max number of discrete airports)
#define DEFAULT_IDX_SIZE    1000    // Default limit for node structs in file
#define AIRPORT_NAME_LENGTH 3       // Char length of airport codes

const char *ROOT_DIR = "../../data";

int f_name(const char *, const struct stat *, int);
void index_dir();

typedef struct AirportNode {
    char *airport;
    struct idxitem *head;
} anode;
typedef struct IndexNode {
    char *name; // airline name
    int count;
    struct idxitem *next;
} idxitem;

idxitem** data_buf; 
idxitem* empty_item;
int index;

int main() {
    idxitem** data_buf = (idxitem**)malloc( DEFAULT_IDX_SIZE*sizeof(idxitem*) );
    index = 0;

    index_dir();
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
    
    data_buf[index] = (idxitem*)malloc( AIRPORT_NAME_LENGTH*sizeof(char) + sizeof(idxitem) );

    return 0;
}

// Function wrapper for ftw, don't really need this, can just call ftw in main instead.
void index_dir() {

    ftw(ROOT_DIR, (*f_name), 2);
}
