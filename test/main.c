#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* DEFAULT = "default";

const int FNAME_SIZE = 8;
const int FNAME_EXT_SIZE = 4;
const char* DEFAULT_FEXT = ".rec";
const char* DEFAULT_FILE_LOC = "/files/";


int createFile(char*);
int simpleWrite(char*, char*);
FILE *getFptr(char*);

int main() {

    char buff[FNAME_SIZE+1];
    char str[FNAME_SIZE+1];

    char* o1 = "Line1";
    //char* o2 = "Line2";
    //char* o3 = "Line3";

    printf("Enter a string: ");
    fgets(buff, FNAME_SIZE+1, stdin);
    snprintf(str, strlen(buff), "%.19s", buff);
    createFile(str);
    printf("\n%s\n\n", str);

    simpleWrite(str, "A line");
    writeLines(str);

    return 0;
}

// can probably create some safe null terminator wrapper fcts for string ops if it becomes easier
int createFile(char* cname) {
    
    FILE * fptr;
    char filename[FNAME_SIZE+FNAME_EXT_SIZE+1];

    // Construct file name (O(1))
    if (cname != NULL) {
        snprintf(filename, sizeof(filename), "%.20s%s", cname, DEFAULT_FEXT);
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

int writeLines(char* fname) 
{
    char line[4];
    FILE *fptr = getFptr(fname);
    fseek(fptr, 0, SEEK_END);
    for (int i = 0; i < 100; i++) {
        sprintf(line, sizeof(line), "a%03d", i);
        printf("\n%d\n", ptrWrite(fptr, line));
    }
    fclose(fptr);
    return 1;
}

int ptrWrite(FILE* ptr, char* str)
{
        if( ptr != NULL )
    {
        fprintf(ptr, str);
        return 1;
    }
    fclose(ptr);
    return -1;
}

int simpleWrite(char* fname, char* str) 
{
    FILE *fptr = getFptr(fname);
    if( fptr != NULL )
    {
        fprintf(fptr, str);
        fclose(fptr);
        return 1;
    }
    fclose(fptr);
    return -1;
}

FILE *getFptr(char* fname) 
{
    FILE *fptr;
    char filename[FNAME_SIZE+FNAME_EXT_SIZE+1];

    // Construct file name (O(1))
    if (fname != NULL) {
        snprintf(filename, sizeof(filename), "%.20s%s", fname, DEFAULT_FEXT);
        printf("\nfile: %s\n\n", filename);
        fptr = fopen(filename, "w");
        return fptr;
    }
    return NULL;
}
