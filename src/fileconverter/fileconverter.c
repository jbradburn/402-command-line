#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>
#include <ctype.h>
#include <errno.h>
#include <ctype.h>  // Using isdigit(char), isalpha(char)

// Constants for index and line size
static const int AIRLINE_INDEX_SIZE = 8;    // Initial index size
static const int MAX_LINE_SIZE = 50;        // Max characters per line, maybe global

// Function declarations
static int increase_airline_index_size();
static char* add_airline_to_index(char**, char**);

// Structures to facilitate indexing file names
char** airline_index;               // Index of airline codes
char** airline_index_path_names;    // Index of file paths for airline codes
int airline_index_count;            // Counter for index
int idxcap;                         // Current capacity of index


// Arrays to buffer input
static char* line_buffer;   // Buffer to hold an entire line of the file
static char inputBuffer[8]; // Character buffer

// Remove?
static int line_cursor = 0;
char default_path[2] = "./";

/*
    TEST CASE, LEAVE UNTIL SUBMISSION

    char* e1 = malloc(8);
    strcpy(e1, "example1");
    printf("%s\n", e1);
    char* d1 = malloc(8);
    strcpy(d1, "/dir/");
    printf("%s\n", d1);
    
    add_airline_to_index(&e1, &d1);
    add_airline_to_index(&e1, &d1);
    add_airline_to_index(&e1, &d1);
    add_airline_to_index(&e1, &d1);
    add_airline_to_index(&e1, &d1);
    add_airline_to_index(&e1, &d1);
    add_airline_to_index(&e1, &d1);
    add_airline_to_index(&e1, &d1);

    printf("%s\n", airline_index_path_names[0]);
    printf("%s\n", airline_index_path_names[1]);
    printf("%s\n", airline_index_path_names[2]);
    printf("%s\n", airline_index_path_names[3]);
*/

int main(int argc, char *argv[]) {

    char* src_fname;    
    char* src_dirname;

    // File and directory pointers for reading source to destination files
    FILE* src_fptr;     // Source file
    FILE* dest_fptr;    // Current destination file
    DIR* dest_dptr;     // Destination directory
    
    // Validate input
    // Check for number of arguments, remove if handled by interface
    if ( argc != 3 ) {
        fprintf(stderr, "Please enter two arguments.\n");
        return EXIT_FAILURE;
    }
    // Check if file can be opened
    src_fptr = fopen(argv[1], "rb");
    if ( !src_fptr ) {
        fprintf(stderr, "Couldn't open infile: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    src_dirname = malloc(sizeof argv[2]);
    strcpy(src_dirname, argv[2]);
    src_dirname = argv[2];
    // TODO: Probably don't need to open directory directly
    // Check if directory can be opened
    if( ( dest_dptr = opendir(argv[2]) ) == NULL){
        fprintf(stderr, "Couldn't open directory: %s\n", argv[2]);
        return EXIT_FAILURE;
    }
    printf("Valid arguments.\n");
    // Init airline_index, path_names, counter
    airline_index_count = 0;
    airline_index = malloc( AIRLINE_INDEX_SIZE*sizeof(char*) );     
    airline_index_path_names = malloc( AIRLINE_INDEX_SIZE*sizeof(char*) );
    idxcap = AIRLINE_INDEX_SIZE;

    // Get length of source file in chars (bytes)
    size_t src_file_size;               // Int for size of file
    fseek(src_fptr, 0, SEEK_END);       // Cur to end of file
    src_file_size = ftell(src_fptr);    // Get position of cur
    fseek(src_fptr, 0, SEEK_SET);       // Cur to beg of file

    // 'Flags'
    int FLAG_INCOMPLETE_BYTE = 0;   // Flag for reading less than 8 "bits" (chars as bits)
    int FLAG_END_OF_AIRLINE_CODE = 0;   // Flag to indicate buffer currently holds only airline code

    // Buffers for reading chars from file to lines
    char* line_buffer; // Allocate memory to line buffer
    int line_buffer_cursor = 0;     
    char bin_char_buffer[8];    // Buffer to read in 8 chars to convert to ascii
    int bin_chars_read; 
    char temp;                  // Char to hold result from 8 "bits"

    char* file_dest_name;

    // Allocate memory to line buffer
    line_buffer = malloc( MAX_LINE_SIZE * sizeof(char) );
    
    FILE* test = fopen("./output/newfile.txt", "w");
    fclose(test);
    printf("Init 1 Passed\n");
    for ( int i = 0; i < src_file_size; i+=8 ) {
        temp = 0;
        // Attempt to read 8 chars (expected (0|1){8}) and check if less than 8 bits are read
        bin_chars_read = fread(bin_char_buffer, sizeof(char), sizeof bin_char_buffer, src_fptr);
        if ( bin_chars_read < sizeof(bin_char_buffer) ) {
            FLAG_INCOMPLETE_BYTE = 1;
            break;
        }
        //printf("Read 8 bin chars, %s\n", bin_char_buffer);
        // Iterate through 'bits' in char buffer, 'convert' to char and store in temp, write to line buffer
        for ( int bit_cursor = 0; bit_cursor < 8; ++bit_cursor) {
            temp |= (bin_char_buffer[bit_cursor] & 1) << (7 - bit_cursor);
        }
        //line_buffer[line_buffer_cursor++] = temp; // Write char to line buffer, incr line buffer
        printf("Converted to char, %c\n", temp);
        // Check flag to separate airline code and index
        printf("Line: %s\n", line_buffer);
        if ( isdigit(temp) && FLAG_END_OF_AIRLINE_CODE == 0 ) {
            file_dest_name = malloc( (sizeof(line_buffer)+4)*sizeof(char) );
            file_dest_name = add_airline_to_index(&line_buffer, &src_dirname);  // TODO: src_dirname must be created
            printf("Airline: %s Directory: %s\n", line_buffer, src_dirname);
            if ( dest_fptr = fopen(file_dest_name, "a") ) {
                //fseek(dest_fptr, 0, SEEK_END);
                // TODO: Handle filename not opening
                // Implement Create here? Maybe not necessary?
            }
            FLAG_END_OF_AIRLINE_CODE = 1;
        }
        // If current char is "\r" == carriage return, then we are at end of line
        else if( temp == 13 ) {
            //fgetc(src_fptr);            // Move cursor past next char (expected '\r' carriage return), 
            printf("Is r\n");
            fseek(src_fptr, 8, SEEK_CUR);
            
            printf("Line: %s\n", line_buffer);
            char lf = 13;       // Add carriage return (13 == \r)
            fwrite(line_buffer, line_cursor, sizeof(char), dest_fptr);  // Write line buffer to destination file
            fwrite(&lf, 1, sizeof(char), dest_fptr);
            fclose(dest_fptr);                                                  // Close destination file
            
            // free line buffer and allocate new memory, prevent mem leak
            free(line_buffer);                                      // Free line_buffer
            free(file_dest_name);
            line_buffer = malloc( MAX_LINE_SIZE * sizeof(char) );   // Allocate new line_buffer
            line_cursor = 0;                                        // Reset line cursor
            FLAG_END_OF_AIRLINE_CODE = 0;                           // Reset airline code flag for next line
        }
        if (temp != 13) {
            line_buffer[line_cursor] = temp;
            line_cursor++;
        }
        // Free pointers to rellocate at beginning of loop
        //printf("Checked index and loaded filename: %s\n", file_dest_name);
    }

    // Handle incomplete byte flag
    if ( FLAG_INCOMPLETE_BYTE != 0 ) {
        printf("Error: incomplete byte, result = %s\n", line_buffer); // Print incomplete line buffer
        if ( line_cursor > 26 ) {
            char lf = 13;       // Add carriage return (13 == \r)
            fwrite(line_buffer, line_cursor, sizeof(char), dest_fptr);  // Write line buffer to destination file
            fwrite(&lf, 1, sizeof(char), dest_fptr);
            fclose(dest_fptr);                                                  // Close destination file
            
            // free line buffer and allocate new memory, prevent mem leak
            free(line_buffer);                                      // Free line_buffer
            free(file_dest_name);
            printf("Writing final line...\n");
            return 0;
        }
    }
}

/*  increase_fb_size
 *  Increase file buffer size by 2.0x, LF = 0.5
 * 
 *  return 0: Successful    -1: file_buffer doesn't exist
 * 
 * NOT USED
 */
static int increase_fb_size() {
    if( !line_buffer ) return -1;

    line_buffer = realloc( line_buffer, 2 * sizeof(line_buffer) );  
    return 0;
}

/*  increase_airline_index_size
 *  Increase airline index size by 2.0x, LF = 0.5
 * 
 *  return 0: Successful    -1: Index or index path names don't exist
 */
static int increase_airline_index_size() {
    //if( !airline_index || !airline_index_path_names) return -1;

    printf("realloc...\n");
    airline_index = realloc( airline_index, 2 * idxcap * sizeof(char*) );
    airline_index_path_names = realloc( airline_index_path_names, 2 * idxcap * sizeof(char*) );
    idxcap *= 2;
    return 0;
}

/*  add_airline_to_index
 *  Checks index and if airline is present, adds new airline and path to file
 * 
 *  @param new_airline  Airline to add to index
 * 
 */
static char* add_airline_to_index(char** new_airline, char** dest_directory) {

    if ( !(*new_airline) ) return 0;
    printf("pass %s\n", *new_airline);
    char* test = malloc( sizeof(*new_airline) +1);
    strcpy(test, *new_airline);
    // Create path from airline name
    char* path_name = malloc( sizeof *new_airline + sizeof *dest_directory + 8);
    //printf("%s\n", *dest_directory);
    strcpy(path_name, *dest_directory);
    strcat(path_name, "/");
    strcat(path_name, test);
    strcat(path_name, ".txt");
    printf("Path: %s\n", path_name);

    airline_index[airline_index_count] = malloc( sizeof *new_airline );
    strcpy( airline_index[airline_index_count], *new_airline );
    airline_index_path_names[airline_index_count] = malloc( sizeof(*dest_directory)+sizeof(*new_airline)+4 );    
    strcpy( airline_index_path_names[airline_index_count], path_name);

    airline_index_count += 1;
    // Increase size of airline index if necessary
    if ( airline_index_count >= idxcap/2 ) {
        increase_airline_index_size();
    }
    
    return airline_index_path_names[airline_index_count-1];
}
