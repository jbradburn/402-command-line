#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

const int command_line_input = 100;

char **tokenizer(char *);

int main(int argc, char *argv[])
{
    char *naiveinterfaceInput = malloc(100); //Array To Hold Input Of The Command Script That The User Is Prompted To Enter By Naiveinterface
    char *buf = malloc(command_line_input);
    char **result; // Array for result
    int pid, status;
    size_t max_size = 50;

    if (argc == 1)
    {

        printf("Please Enter Your Command Input \n");
        scanf("%[^\n]s", buf);
        strcpy(naiveinterfaceInput, buf);

        result = tokenizer(naiveinterfaceInput); // Tokenize str into result

        pid = fork();
        printf("PID: %d\n", pid);
        if (pid == 0)
            execvp(result[0], result);
        if (pid > 0)
            pid = wait(&status);
    }
    else if (argc == 2)
    {
        FILE *script = fopen(argv[1], "r");
        char* linebuf = malloc(sizeof(char));
        while ((getline(&naiveinterfaceInput, &max_size, script)) != -1)
        {
            result = tokenizer(naiveinterfaceInput);

            pid = fork();
            printf("PID: %d\n", pid);
            if (pid == 0)
                execvp(result[0], result);
            if (pid > 0)
                pid = wait(&status);
        }
    }
}

char **tokenizer(char *str)
{
    const char s[2] = " "; // Set delim
    char *token;           // Char* to hold tokens

    token = strtok(str, s); // Init first token into string

    char **result = malloc(sizeof(char *)); // Allocate n_param strings to result
    int idx = 0;                            // Init index to 0

    while (token != NULL)
    {                           // While more tokens are in source string
        printf(" %s\n", token); // Print token
        result[idx] = malloc(sizeof(token));
        strcpy(result[idx++], token);
        token = strtok(NULL, s); // Get next token

        result = realloc(result, sizeof(char *) * (idx + 1));
    }

    free(token);   // Free token
    return result; // Return result
}
