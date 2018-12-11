#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

char *getLine(FILE *);

char *getLine(FILE *fptr)
{
    if (fptr != NULL)
    {
        int i = 0;
        char cur;
        char* buf = malloc(sizeof(char));               // Alloc char to buf
        while ( (cur = fgetc(fptr) ) != 0 ) {           // While fgetc not NULL
            if ( cur != 10 ) {                          // If char != line feed
                buf[i++] = cur;                         // Add char to buf
                buf = realloc(buf, sizeof(buf) + sizeof(char)); // Allocate +1 char to buf
            }
            else if ( cur == 10 || cur == EOF){  // If char == line feed or char == EOF
                buf[i] = 0;         // Null terminate buf
                return buf;         // Return buf (Line)
            }
        }
    }
    return NULL;        // If line doesn
}