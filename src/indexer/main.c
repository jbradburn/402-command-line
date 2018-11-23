#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Constants
const char DELIM = ' ';			// String delimiter
const char PRINT_TOK_SEP = ',';	// Separator for tokens in print func

const size_t BUF_SIZE = 100;	// Line buffer size
const size_t PARAM_NUM = 5;		// Number of parameters
const size_t TOKEN_SIZE = 10;	// Bytes per parameter in line
const size_t MAX_SIZE = 100;	// Max number of lines in file

// Function defs
char** stringTokenize(char*, const char);
void printTriArray(char***, int);

int main() {
	char LINE_BUF[BUF_SIZE];	// Line buffer
	char **FILE_LINES_BUF;		// Buffer to collect all lines in a file
	char ***DATA;				// Array for sets of 'tokens' (parameters)

	FILE* fp = fopen("./AA.txt", "r");	// Open file as 'fp'
	assert(fp);	
	printf("\nFile open success.\n");

	// Memory Allocation
	FILE_LINES_BUF = (char**) malloc( MAX_SIZE*sizeof( char* ) );	// Allocate MAX_LINES char ptrs to line array
	DATA = (char***) malloc( MAX_SIZE*sizeof( char** ) );			// Allocate MAX_LINES double char pointers to DATA arrayS
	int i = 0;														// (i)ndex for reading file
	while ( fgets(LINE_BUF, BUF_SIZE, fp ) != NULL ) {					// Get one line from file pointer
		FILE_LINES_BUF[i] = (char*)malloc( strlen(LINE_BUF) + 1 );		// Allocate strlen(LINE_BUF) + 1 bytes to FILE_LINES_BUF[i]
		DATA[i] = (char**)malloc( TOKEN_SIZE*sizeof(char*) );			// Allocate 5 char pointers to DATA[i]
		strcpy( FILE_LINES_BUF[i], LINE_BUF );							// Copy first line from LINE_BUF to FILE_LINES_BUF[i]
		i = i + 1;														// Increment i
	}
	int NUM_LINES =	i; 											// i == number of lines, i => NUM_LINES
	for (int j = 0; j < NUM_LINES; j++) {						// For each line in LineS Buffer
		DATA[j] = stringTokenize(FILE_LINES_BUF[j], DELIM);		// Tokenize each string in FILE_LINES_BUF[] and place in DATA[]
	}
	assert( DATA );
	printTriArray(DATA, NUM_LINES);
}

/*
 * Tokenize input string into char** by separator s_delim
 */
char** stringTokenize(char* str, const char s_delim) {
	//int count = PARAM_NUM;
	char s_str[2];	// char* for delimiter
	s_str[0] = ' ';	// delimiter character
	s_str[1] = 0;	// null terminator

	// Tokenize strings
    char **result = malloc( PARAM_NUM * sizeof( char* ) );	// Allocate char* to array, N == number of elements in line
    if (result) {
    	size_t idx = 0;						// Index to track ptr offset 
    	char *token = strtok(str, s_str);	// Store first token 

    	while (token) {							// While token (from strtok) still has data (hasn't reached end of line)
    		assert( idx < PARAM_NUM );			// Check Nth token isn't greater than (N)umber of elements we want to read
    		*(result + idx++) = strdup(token);	// Increment char* ptr and duplicate last token into char** result at offset idx (idx = number of char*)
    		token = strtok(0, s_str);			// Store next token
    	}
    	assert( idx == PARAM_NUM );					// Check index is exactly the number of parameters we expect
    	*( result + idx ) = 0;						// Set null terminator at end of char**
    	result[4][strlen( (char*) result )-1] = 0;	// Set last line break to '\0'
    }
    return result;
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