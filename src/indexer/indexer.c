#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
// Directory libraries
#include <dirent.h>
#include <ftw.h>

const char *ROOT_DIR = "../../data";

int f_name(const char *, const struct stat *, int);
void index_dir();

int main() {
    index_dir();
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

// Function wrapper for ftw, don't really need this, can just call ftw in main instead.
void index_dir() {

    ftw(ROOT_DIR, (*f_name), 2);
}
