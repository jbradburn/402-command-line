#include <stdio.h>

#include "filelib.h"
#include "globals.h"


int createFile(char* cname) {
    
    FILE * fptr;
    char filename[FNAME_SIZE+FNAME_EXT_SIZE+1];

    // Construct file name (O(1))
    if (cname != NULL) {
        printf("\n%s\n", DEFAULT_FILE_LOC);
        snprintf(filename, sizeof(filename), "%s%.20s%s", DEFAULT_FILE_LOC, cname, DEFAULT_FEXT);
    }
    else return -1;

    fptr = fopen(filename, "w");
    
    if (fptr == NULL)
    {
        printf("Error or file does not exist\n");
        return -1;
    }
    fclose(fptr);
    return 1;
}