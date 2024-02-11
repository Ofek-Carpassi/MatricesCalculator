/* A calculator for matrices. */
/* The main function, all the intercation with the user and the command analysis. */
/* Input: <command> <arg1>, <arg2>, <arg3> (all the arguments are separated by commas and spaces) */

/*
A blueprint for the program:
main:
    - call getInput. DONE
    - call parseCommand. DONE
    - call executeCommand.
getInput:
    - get the input from the user. DONE
    - clean the input from spaces and tabs. DONE
    - check for errors in the input. DONE
getLine:
    - get the input from the user. DONE
parseCommand:
    - gets the command from the input. DONE
    - check if the command is valid (is in the list of commands). DONE
    - remove the command from the input. DONE
    - return a number that represents the command. DONE
getArguements:
    - break the input into the command and the arguements. Finish by 11/3/2024
    - check if the command is valid (is in the list of commands). Finish by 11/3/2024
executeCommand:
    - call the corresponding function for the command. Finish by 11/3/2024
six command execution functions:
    - check if the arguments are valid for the command. Finish by 11/3/2024
    - each function will call the corresponding function from mymat.h. Finish by 11/3/2024
*/

#include <stdio.h> /* fpr input and output */
#include <string.h> /* for string manipulation */
#include <ctype.h> /* for character manipulation */
#include <stdlib.h> /* for atof */
#include "mymat.h" /* for the matrix functions (mymat.h is a header file that I created) */

/* A struct that maps between a matrix name and a pointer to the matrix */
typedef struct {
    char* matrixName;
    mat* matrix;
} MatrixMap;

/* Declarations of the functions */
void getInput(char* input, char* clearInput);
void getLine(char* input);
int parseCommand(char* input, char* command, char* commands[]);
void getArguements(char* input, char* command, char* args[]);
void executeCommand(char* command, char* args[], MatrixMap* matrixMap);
void executeReadMat(char* args[], MatrixMap* matrixMap);
void executePrintMat(char* args[], MatrixMap* matrixMap);
void executeAddMat(char* args[], MatrixMap* matrixMap);
void executeSubMat(char* args[], MatrixMap* matrixMap);
void executeMulMat(char* args[], MatrixMap* matrixMap);
void executeMulScalar(char* args[], MatrixMap* matrixMap);
void executeTransMat(char* args[], MatrixMap* matrixMap);

/* Defining all needed constants */
#define TRUE 1
#define FALSE 0
#define AMOUNT_OF_NUMBERS 16
#define AMOUNT_OF_MATRICES 6
#define MAX_LENGTH_OF_CLEAN_INPUT 648
#define MAX_LENGTH_OF_COMMAND 9

/* The main function */
int main()
{
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F; /* Defining all the matrices */
    char input[] = "", clearInput[MAX_LENGTH_OF_CLEAN_INPUT] = "", command[MAX_LENGTH_OF_COMMAND] = ""; /* The input from the user and the command */
    char* commands[] = {"read_mat", "print_mat", "add_mat", "sub_mat", "mul_mat", "mul_scalar", "trans_mat", "stop"};
    float numList[AMOUNT_OF_NUMBERS] = {0}; /* A number list of 0's for initializing the matrices */
    MatrixMap matrixMap[AMOUNT_OF_MATRICES]; /* A lookup table for the matrices */
    int commandIndex = -1; /* The index of the command in the commands array */

    /* Initialize the lookup table */
    matrixMap[0].matrixName = "MAT_A"; /* the name of the matrix */
    matrixMap[0].matrix = &MAT_A; /* the pointer to the matrix */
    matrixMap[1].matrixName = "MAT_B"; /* the name of the matrix */
    matrixMap[1].matrix = &MAT_B; /* the pointer to the matrix */
    matrixMap[2].matrixName = "MAT_C"; /* the name of the matrix */
    matrixMap[2].matrix = &MAT_C; /* the pointer to the matrix */
    matrixMap[3].matrixName = "MAT_D"; /* the name of the matrix */
    matrixMap[3].matrix = &MAT_D; /* the pointer to the matrix */
    matrixMap[4].matrixName = "MAT_E"; /* the name of the matrix */
    matrixMap[4].matrix = &MAT_E; /* the pointer to the matrix */
    matrixMap[5].matrixName = "MAT_F"; /* the name of the matrix */
    matrixMap[5].matrix = &MAT_F; /* the pointer to the matrix */

    /* Intialize all matrices to zero using an array of 16 zeros and the read_mat function */
    read_mat(&MAT_A, numList);
    read_mat(&MAT_B, numList);
    read_mat(&MAT_C, numList);
    read_mat(&MAT_D, numList);
    read_mat(&MAT_E, numList);
    read_mat(&MAT_F, numList);

    /* The main loop */
    while (TRUE)
    {
        getInput(input, clearInput); /* Get the input from the user */
        if(clearInput[0] == '\0') /* If the input is empty */
        {
            continue;
        }
        printf("The input is: %s\n");
        commandIndex = parseCommand(clearInput, command, commands); /* Parse the command from the input */
        if(command[0] == '\0' || commandIndex == -1)
        {
            continue;
        }
        printf("The command is: %s\n", command);
        printf("The input is: %s\n", clearInput);
        break;
    }
}

/* The function that gets the input from the user */
void getInput(char* input, char* clearInput)
{
    int i = 0, startingIndex = 0, j = 0, lastCommaIndex = 0, isWaitingForComma = FALSE, isInCommand = TRUE, isInEndOfCommand = FALSE, endOfCommandIndex = 0, endOfInputIndex = 0;
    
    getLine(input); /* Get the input from the user */
    
    /* Create a string the length of the input that will be cleared from spaces and tabs */
    /* The maximal input is a read mat with the 16 longest float number possible (read_mat,MAT_A,-3.40282e+38,-3.402828e+38,...,-3.40282e+38) - the length of the input will be 24+16*39 = 648 */
    /* The length of -3.40282e+38 is 39 in decimal and the amount of commas is 18, the amount of numbers is 16, the length of the command name and the matrix name is 24 */
    /* We will just put a \0 at the end of the string and the rest will be filled with spaces */

    for(i = strlen(input) - 1; i > 0; i--)
    {
        if(input[i] == ' ' || input[i] == '\t')
        {
            continue;
        }
        else
        {
            if(input[i-1] == ' ' || input[i-1] == '\t')
            {
                endOfCommandIndex = i;
                break;
            }
        }
    }
    printf("The end of the command is: %d\n", endOfCommandIndex);

    /* Clean the input from spaces and tabs - each time we get to a space we will call a function that checks if the current argument is valid */
    for(i = 0; input[i] != '\0'; i++)
    { 
        if(i == endOfCommandIndex)
        {
            startingIndex = i;
            for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ' ' && input[j+startingIndex] != '\t'; j++)
            {
                if(input[j+startingIndex] == ',' && input[j+startingIndex+1] != ' ' && input[j+startingIndex+1] != '\t' && input[j+startingIndex+1] != '\0' && input[j+startingIndex+1] != '\n')
                {
                    printf("Illegal comma\n");
                    clearInput[0] = '\0';
                    return;
                }
                clearInput[j+lastCommaIndex] = input[j+startingIndex];
            }
            endOfInputIndex = j+startingIndex;
            printf("The end of the input is: %d\n", endOfInputIndex);

            j--;
            while(input[j+startingIndex] == ' ' || input[j+startingIndex] == '\t')
            {
                j--;
            }
            /* Put a comma at the end for our getArguements function */
            clearInput[j+lastCommaIndex+1] = ',';

            /* Check for extreneous text after the command */
            for(j = endOfInputIndex; input[j] != '\0'; j++)
            {
                if(input[j] != ' ' && input[j] != '\t' && input[j] != '\n' && j != startingIndex)
                {
                    printf("Extraneous text after the command\n");
                    clearInput[0] = '\0';
                    return;
                }
            }
            break;
        }
        else if(input[i] == ' ' || input[i] == '\t')
        {
            continue;
        }
        else if(input[i] == ',')
        {
            if(isWaitingForComma == FALSE)
            {
                printf("Illegal comma\n");
                clearInput[0] = '\0';
                return;
            }
            isWaitingForComma = FALSE;
        }
        else if(input[i] != ' ' && input[i] != '\t' && (input[i-1] == ' ' || input[i-1] == '\t' || i-1 < 0) && isWaitingForComma == FALSE && input[i] != ',')
        {
            if(isInCommand == TRUE)
            {
                isInCommand = FALSE;
                startingIndex = i;
                for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ' '; j++)
                {
                    if(input[j+startingIndex] == ',')
                    {
                        printf("Illegal comma\n");
                        clearInput[0] = '\0';
                        //clearInput = "";
                        return;
                    }
                    clearInput[j] = input[j+startingIndex];
                }
                j--;
                while(input[j+startingIndex] == ' ' || input[j+startingIndex] == '\t')
                {
                    j--;
                }
                isWaitingForComma = FALSE;
            }
            else
            {
                startingIndex = i;
                for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ','; j++)
                {
                    clearInput[j+lastCommaIndex] = input[j+startingIndex];
                }
                j--;
                while(input[j+startingIndex] == ' ' || input[j+startingIndex] == '\t')
                {
                    j--;
                }
                isWaitingForComma = TRUE;
            }
            /* Put a comma at the end for our getArguements function */
            clearInput[j+lastCommaIndex+1] = ',';
            lastCommaIndex += j+2;
            /* Move the index to the end of the current argument */
            i = j + startingIndex;
        }
        else if(input[i] != ' ' && input[i] != '\t' && (input[i-1] == ',' || input[i-1] == ' ' || input[i-1] == '\t') && isWaitingForComma == TRUE)
        {
            printf("Missing comma\n");
            clearInput[0] = '\0';
            return;
        }
    }
}

/* A function that gets a line from the input */
void getLine(char *str)
{
    while((*str = getchar()) != EOF && *str != '\n')
    {
        str++;
    }
    *str = '\0';
}

int parseCommand(char* input, char* command, char* commands[])
{
    int i = 0, commandEndIndex = 0, commandIndex = -1;

    for(i = 0; i < strlen(input); i++)
    {
        if(input[i] == ',')
        {
            break;
        }
        command[i] = input[i];
    }    
    command[i] = '\0'; /* Add a null terminator to the command string */
    commandEndIndex = i;

    for(i = 0; i < 8; i++)
    {
        if(strcmp(command, commands[i]) == 0)
        {
            commandIndex = i;
            break;
        }
    }

    if(commandIndex == -1)
    {
        printf("Undefined command name\n");
        command[0] = '\0';
        return -1;
    }

    if(commandIndex == 7)
    {
        for(i = commandEndIndex; i < strlen(input); i++)
        {
            if(input[i] != ' ' && input[i] != '\t')
            {
                printf("Extraneous text after end of command\n");
                command[0] = '\0';
                return -1;
            }
        }
    }

    /* remove the command from the input */
    for(i = commandEndIndex; i < strlen(input); i++)
    {
        input[i-commandEndIndex] = input[i+1];
    }

    input[i-commandEndIndex] = '\0';

    printf("The command is: %s\n", command);
    printf("The input is: %s\n", input);

    return commandIndex;
}