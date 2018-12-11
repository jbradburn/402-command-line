#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tokenizer.h"

/*
int main() {
	char ***DATA;
	FILE* fp = fopen("../../data/AA.txt", "r");	// Open file as 'fp'

	assert(fp);	
	printf("\nFile open success.\n");
	DATA = (char***)malloc( MAX_SIZE*sizeof(char**) );
	DATA = tokenizeFile(fp);
	free(DATA);
}
*/

extern token_file_wrapper* tokenizeFile(FILE* fp) {
	char LINE_BUF[BUF_SIZE];	// Line buffer
	char **FILE_LINES_BUF;		// Buffer to collect all lines in a file
	char ***DATA;				// Array for sets of 'tokens' (parameters)

	// Memory Allocation
	FILE_LINES_BUF = (char**) malloc( MAX_SIZE*sizeof( char* ) );	// Allocate MAX_LINES char ptrs to line array
	DATA = (char***) malloc( MAX_SIZE*sizeof( char** ) );			// Allocate MAX_LINES double char pointers to DATA arrayS
	int i = 0;														// (i)ndex for reading file
	while ( fgets(LINE_BUF, BUF_SIZE, fp ) != NULL ) {					// Get one line from file pointer
		FILE_LINES_BUF[i] = (char*)malloc( strlen(LINE_BUF) + 1 );		// Allocate strlen(LINE_BUF) + 1 bytes to FILE_LINES_BUF[i]
		//DATA[i] = (char**)malloc( TOKEN_SIZE*sizeof(char*) );			// Allocate 5 char pointers to DATA[i]
		FILE_LINES_BUF[i] = strdup( LINE_BUF );							// Copy first line from LINE_BUF to FILE_LINES_BUF[i]
		i = i + 1;														// Increment i
	}
	//free(LINE_BUF);
	int NUM_LINES =	i; 											// i == number of lines, i => NUM_LINES
	for (int j = 0; j < NUM_LINES; j++) {						// For each line in LineS Buffer
		DATA[j] = stringTokenize(FILE_LINES_BUF[j]);		// Tokenize each string in FILE_LINES_BUF[] and place in DATA[]
	}
	assert( DATA );
	printTriArray(DATA, NUM_LINES);
	token_file_wrapper* OUTPUT = malloc( sizeof(token_file_wrapper) );
	OUTPUT->DATA = DATA;
	OUTPUT->SIZE = NUM_LINES;
	return OUTPUT;
}

/*
 * Tokenize input string into char** by separator s_delim
 */
char** stringTokenize(char* str) {
    const char s[2] = " ";  // Set delim
    char *token;            // Char* to hold tokens

    token = strtok(str, s); // Init first token into string
    
    char** result = malloc( sizeof(char*) );    // Allocate n_param strings to result
    int idx = 0;                                        // Init index to 0

    while( token != NULL ) {            // While more tokens are in source string
        printf( " %s\n", token );       // Print token
        result[idx] = malloc(sizeof(token));
        strcpy(result[idx++], token);
        token = strtok(NULL, s);        // Get next token
    
        result = realloc(result, sizeof(char*) * (idx+1) );
    }

    free(token);    // Free token
    return result;  // Return result
}

/*
 *	Print tokenized data from array
 */
void printTriArray(char ***arr, int nlines) {
	for ( int x = 0; x < nlines; x++ ) {		// For each char** subarray in input arr
		for ( int y = 0; y < PARAM_NUM; y++ ) {	// For each char* element in subarray
			printf( "[%s]%s", arr[x][y], ( y == PARAM_NUM - 1 ) ? "\n":"," );	// Print [element] with comma if not last element
		}	
	}
}