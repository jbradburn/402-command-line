#ifndef TOKENIZER_H
#define TOKENIZER_H

// Constants
const static char DELIM = ' ';			// String delimiter
const static char PRINT_TOK_SEP = ',';	// Separator for tokens in print func

const static size_t BUF_SIZE = 100;	// Line buffer size
const static size_t PARAM_NUM = 5;		// Number of parameters
const static size_t TOKEN_SIZE = 10;	// Bytes per parameter in line
const static size_t MAX_SIZE = 100;	// Max number of lines in file

// Function defs
char*** tokenizeFile(FILE* fp);
char** stringTokenize(char*, const char);
void printTriArray(char***, int);


#endif  /* TOKENIZER.H */