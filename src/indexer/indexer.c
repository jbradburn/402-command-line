#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
// Directory libraries
#include <dirent.h>

#include "tokenizer.h"
#include "../create/create.h"

// Number of tokens per line
#define NUM_LINE_TOKENS 5

typedef struct anode {
    char *name;          // airport name
    
    struct anode* next;
    
    struct inode* data;    // airlines for airport
    
    int size;            // num elements
} anode;

typedef struct inode {
    char *name; // airline name

    int count;  // number of flights on airline for head airport
    
    struct inode* next;
} inode;

// Structures for holding data
anode* airport_list;

// Function Declarations
int f_name(const char*, const struct stat*, int, struct FTW*);
void index_dir();
char** stringTokenize(char*);

anode* find(char*); 
void insertFirst(char*);

inode* findAirline(anode*, char*);
void insertFirstAirline(anode*, char*);


char* getLetters(char**);

// Variables
char* src_dir;
char* dest_file_path;
FILE* dest_fptr;
int main(int argc, char* argv[]) {

    // Init airport_list
    airport_list = NULL;
    src_dir = malloc(sizeof(argv[2]));
    // Determine source directory and destination file
    if (argc == 2) {
        printf("argv[1]: %s\n", argv[1]);
        src_dir = malloc(sizeof(argv[1]));
        dest_file_path = malloc(11);
        strcpy(src_dir, argv[1]);
        dest_file_path = "invdex.txt";
    }
    else if (argc == 3) {
        printf("argv[1]: %s argv[2]: %s\n", argv[1], argv[2]);
        dest_file_path = malloc(sizeof(argv[1]));
        src_dir = malloc(sizeof(argv[2]));
        strcpy(dest_file_path, argv[1]);
        strcpy(src_dir, argv[2]);
    }
    dest_fptr = fopen(dest_file_path, "w");
    // Open destination file pointer
    if ( dest_fptr == NULL ) {
        printf("Destination file could not be opened.\n");
        return -1;
    }

    index_dir();

    anode* top = malloc(sizeof(anode));
    top = airport_list;

    while(top != NULL) {
        printf("%s\n", top->name);
        // Array to hold whole line before printing
        char* line = malloc(100);
        strcpy(line, top->name);    // Put airport in line
        //fwrite(top->name, sizeof(top->name), sizeof(char), dest_fptr);  // Write Airport to newline
        strcat(line, " ");          // put Space in line
        //fwrite(" ", 1, sizeof(char), dest_fptr);
        inode* head = NULL;
        head = top->data;
        
        // Null terminate, find end of line, write to file
        char nil = 0;
        strcat(line, &nil);         // Null terminate line
        char lcount = 0;
        while (line[lcount] != 0) lcount++;
        fwrite(line, 1, lcount, dest_fptr);
        while( head != NULL ) {
            line = NULL;
            line = malloc(100);
            char tmp[4];
            strcpy(line, head->name);   // Put airline in line
            strcat(line, " ");          // Put space in line
            sprintf(tmp, "%d", head->count);
            strcat(line, tmp);          // Put count in line
            strcat(line, " ");          // Put space in line
            printf("    %s, %d\n", head->name, head->count);
            
            char nil = 0;
            strcat(line, &nil);         // Null terminate line
            char lcount = 0;
            while (line[lcount] != 0) lcount++;

            fwrite(line, 1, lcount, dest_fptr); // Write whole line to file
            head = head->next;
        }
        fwrite("\n", 1, sizeof(char), dest_fptr);
        top = top->next;
    }

    fclose(dest_fptr);
    return 0;
}

// Function wrapper for ftw, don't really need this, can just call ftw in main instead.
void index_dir() {
    int flags = 0;
    flags |= FTW_PHYS;
    nftw(src_dir, f_name, 20, flags);
    //prettyPrintIndexData();
    return;

}

int f_name(const char *f_name, const struct stat *sb, int f_type, struct FTW *ftw_buf) {
    printf("\n%s\n", f_name);
    if( f_type == FTW_NS )
        return 0;
    
    if( f_type == FTW_F ) {
        printf("Is file\n");

        // File pointer for current file in directory traversal
        FILE * cur_file;
        // Array to hold string tokens
        char** cur_tokens = malloc( sizeof(char*) * NUM_LINE_TOKENS 
        );
        if ( (cur_file = fopen(f_name, "rb")) == NULL ) {
            printf("Error: A file in the source directory could not be accessed.\n");
            return -1;
        }
        char** file_data = malloc( sizeof(char*) );
        char* line_buffer = malloc(50*sizeof(char));
        size_t max_line_size = 50;
        
        // Get each line, tokenize, add airport instance, add airline to airport instance 
        while( getline(&line_buffer, &max_line_size, cur_file) != -1 ) {
            printf("%s", line_buffer);  // Print current line
            cur_tokens = stringTokenize(line_buffer);   // Tokenize current line

            // Cursor anode
            anode* temp;
            inode* itemp;
            temp = airport_list;
            // Search airport list

            char* line_code = malloc( 50 );
            strcpy(line_code, f_name);
            /*    
                int cur = 0;
                while (f_name[cur] != 46 && cur > 2) {
                    line_code[cur] = f_name[cur];
                    line_code = realloc(line_code, sizeof(line_code) + sizeof(char) ); 
                    cur++;
                }
                line_code = realloc(line_code, sizeof(line_code) + sizeof(char)*4 );
                strcat(line_code, ".txt");
                cur = cur + 4;
                line_code[cur] = 0;
            */
            if ( (temp = find(cur_tokens[1]) ) == NULL) {
                printf("%s\n", cur_tokens[1]);
                insertFirst(cur_tokens[1]);
                printf("%s\n", airport_list->name);
                //char* line_code = getLetters(&cur_tokens[0]);
                insertFirstAirline(airport_list, line_code);
            }
            if ( (temp = find(cur_tokens[2]) ) == NULL) {
                printf("%s\n", cur_tokens[2]);
                insertFirst(cur_tokens[2]);
                printf("%s\n", airport_list->name);
                //char* line_code = getLetters(&cur_tokens[0]);
                insertFirstAirline(airport_list, line_code);
            }
            else {
                if ( (itemp = findAirline(find(cur_tokens[1]), line_code) ) == NULL ) {
                    insertFirstAirline(find(cur_tokens[1]), line_code);
                }
                else {
                    itemp->count += 1;
                }
                if ( (itemp = findAirline(find(cur_tokens[2]), line_code) ) == NULL ) {
                    insertFirstAirline(find(cur_tokens[2]), line_code);
                }
                else {
                    itemp->count += 1;
                }
            }

        } 
    }
    return 0;
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

// Airport functions
//find a link with given key
anode* find(char* key) {

   //start from the first link
   struct anode* current = airport_list;

   //if list is empty
   
   if(airport_list == NULL) {
      return NULL;
   }

   //navigate through list
   while( strcmp(current->name, key) != 0) {
       printf("Current->name: %s, key: %s\n", current->name, key);
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//insert link at the first location
void insertFirst(char* key) {
   //create a link
   anode *link = malloc(sizeof(struct anode));
	
   inode* new_data = NULL; 

    link->name = malloc(sizeof(key));
    link->name = key;
   link->data = new_data;
	
   //point it to old first node
   link->next = airport_list;
	
   //point first to new first node
   airport_list = link;
   printf("Node added:\nname: %s\n", link->name);
}

// Airline functions
//find a link with given key
inode* findAirline(anode* airport, char* key) {

   //start from the first link
   struct inode* current = airport->data;

   //if list is empty
   
   if(airport->data == NULL) {
      return NULL;
   }

   //navigate through list
   while( strcmp(current->name, key) != 0) {
       printf("Airline Current: %s, key: %s\n", current->name, key);
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//insert link at the first location
void insertFirstAirline(anode* airport, char* key) {
   //create a link
   inode *link = malloc(sizeof(struct anode));

    link->name = malloc(sizeof(key));
    link->name = key;
    link->count = 1;
	
   //point it to old first node
   link->next = airport->data;
	
   //point first to new first node
   airport->data = link;
   printf("Node added:\nname: %s\n", link->name);
}

char* getLetters(char** str) {
    if (*str != NULL) {
        char* result = malloc(sizeof(char));
        char* data = *str;
        int cur = 0;
        while ( isalpha(data[cur]) ) {
            result[cur] = data[cur];
            result = realloc(result, sizeof(result) + sizeof(char));
            cur++;
         } 
         // Null terminate airline code string
         result[cur] = 0;
         return result;
    }
    return NULL;
}