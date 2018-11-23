/**
 * C program to count occurrences of a word in file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//Function to count file occurrence
int countOccurrences(FILE *fptr, const char *airportWord);


int main()
{
    FILE *fptr;
    char airportWord[50];
    char airportTextFile[5];
    int  airportWordCount;
    char c[1000];
   
    /* Input word to provide text file to search through */
    printf("Enter the airport text file you want to search through: ");
    scanf("%s", airportTextFile);
  

    /* Input word to search in file */
    printf("Enter the airport you're searching for: ");
    scanf("%s", airportWord);

    //Open file
    fptr = fopen(airportTextFile, "r");
     

    // Exit if file not opened  
    if (fptr == NULL)
    {
        printf("Unable to open file.\n");
        exit(EXIT_FAILURE);
    }

    // Call function to count all occurrence of word
    airportWordCount = countOccurrences(fptr, airportWord);

    printf("'%s' is found %d times in file.", airportWord, airportWordCount);


    // Close file
    fclose(fptr);

    return 0;
}


/**
 * Returns total occurrences of a word in given file.
 */
int countOccurrences(FILE *fptr, const char *airportWord)
{
    char str[1000];
    char *pos;

    int index, count;
    
    count = 0;

    // Read line from file till end of file.
    printf("Data from the file: \n"); 
    while ((fgets(str, sizeof str, fptr)) != NULL)
    {
    	
    	 
        printf("%s", str);
  
        index = 0;

        // Find next occurrence of word in str
        while ((pos = strstr(str + index, airportWord)) != NULL)
        {
            // Index of word in str is
            // Memory address of pos - memory
            // address of str.
            index = (pos - str) + 1;

            count++;
        }
    }

    return count;
}
