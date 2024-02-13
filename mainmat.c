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

/* 
Declarations of the functions:
getInput - gets the input from the user and cleans it while checking for errors and returning the index of the command in the commandsNames array
getLine - gets a line from the input
parseArguments - parses the arguments from the input
breakInputReadMat - breaks the input for the read_mat command
breakInputPrintMat - breaks the input for the print_mat command
breakInputThreeMat - breaks the input for the add_mat, sub_mat and mul_mat commandsNames
breakInputMulScalar - breaks the input for the mul_scalar command
breakInputTransMat - breaks the input
strdup - duplicates a string
*/
int getInput(char* input, char* clearInput, char* commandsNames[]); 
void getLine(char* input);
void parseArguments(char* input, int command, MatrixMap matrixMap[]);
void breakInputReadMat(char* stringArguments[], MatrixMap matrixMap[]);
void breakInputPrintMat(char* stringArguments[], MatrixMap matrixMap[]);
void breakInputThreeMat(char* stringArguments[], MatrixMap matrixMap[], int command);
void breakInputMulScalar(char* stringArguments[], MatrixMap matrixMap[]);
void breakInputTransMat(char* stringArguments[], MatrixMap matrixMap[]);
char *strdup(const char *src);

/* Defining all needed constants */
#define TRUE 1 /* A constant for true */
#define FALSE 0 /* A constant for false */
#define AMOUNT_OF_NUMBERS 16 /* The amount of numbers in the input */
#define AMOUNT_OF_MATRICES 6 /* The amount of matrices in the program */
#define MAX_LENGTH_OF_CLEAN_INPUT 648 /* The maximal length of the input after cleaning. The maximal input is a read mat with the 16 longest float number possible (read_mat,MAT_A,-3.40282e+38,-3.402828e+38,...,-3.40282e+38) - the length of the input will be 24+16*39 = 648, The length of -3.40282e+38 is 39 in decimal and the amount of commas is 18, the amount of numbers is 16, the length of the command name and the matrix name is 24 */
#define MAX_LENGTH_OF_COMMAND 11 /* The maximal length of the command */
#define MAX_AMOUNT_OF_ARGUMENTS 17 /* The maximal amount of arguments in the input */
#define AMOUNT_OF_COMMANDS 8 /* The amount of commandsNames in the program */
#define STOP_COMMAND_INDEX 7 /* The index of the stop command in the commandsNames array */
#define MAX_ARG_LENGTH 39 /* The maximal length of an argument */
#define UNDEFINED_COMMAND -1 /* A constant for an undefined command */

/* The main function */
int main()
{
    /* 
    Declarations of all needed variables:
    input - the input from the user
    clearInput - the input after cleaning
    commandsNames - an array of the commandsNames in the program
    numList - an array of 16 zeros for initializing the matrices
    matrixMap - a lookup table for the matrices
    commandIndex - the index of the command in the commandsNames array
    */
    char input[] = "", clearInput[MAX_LENGTH_OF_CLEAN_INPUT] = "", *commandsNames[] = {"read_mat", "print_mat", "add_mat", "sub_mat", "mul_mat", "mul_scalar", "trans_mat", "stop"};
    float numList[AMOUNT_OF_NUMBERS] = {0};
    MatrixMap matrixMap[AMOUNT_OF_MATRICES];
    int commandIndex = UNDEFINED_COMMAND;

    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F; /* Defining all the matrices */

    /* Intialize all matrices to zero using an array of 16 zeros and the read_mat function */
    read_mat(&MAT_A, numList);
    read_mat(&MAT_B, numList);
    read_mat(&MAT_C, numList);
    read_mat(&MAT_D, numList);
    read_mat(&MAT_E, numList);
    read_mat(&MAT_F, numList);

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

    /* The main loop */
    while (TRUE)
    {
        /* Make input and clearInput empty again. */
        input[0] = '\0';
        clearInput[0] = '\0';

        /* Get the input from the user and clean it */
        commandIndex = getInput(input, clearInput, commandsNames);

        /* If the command is stop break the loop */
        if(commandIndex == STOP_COMMAND_INDEX)
        {
            break;
        }
        /* If the input is invalid continue to the next iteration */
        if(clearInput[0] == '\0')
        {
            continue;
        }

        parseArguments(clearInput, commandIndex, matrixMap); /* Parse the arguments from the input and execute the command */
    }

    return 0; /* End the program */
}

/* The function that gets the input from the user and cleans it while checking for errors */
int getInput(char* input, char* clearInput, char* commandsNames[])
{
    /*
    Declarations:
    i, j - counters for several loops
    startingIndex - the starting index of the current argument
    lastCommaIndex - the index of the last comma in the clearInput array
    isWaitingForComma - a flag that tells us if we are waiting for a comma
    endOfCommandIndex - the index of the end of the command
    endOfInputIndex - the index of the end of the input
    commandIndex - the index of the command in the commandsNames array
    */
    int i = 0, j = 0, startingIndex = 0, lastCommaIndex = 0, isWaitingForComma = FALSE, endOfCommandIndex = 0, endOfInputIndex = 0, commandIndex = UNDEFINED_COMMAND;

    /* Get the input from the user */
    getLine(input);

    /* Find the command in the input */
    for(i = 0; i < AMOUNT_OF_COMMANDS; i++)
    {
        /* If the command is found in the input we will save the index of the command in the commandIndex variable */
        if(strstr(input, commandsNames[i]) != NULL)
        {
            /* Save the index of the command in the commandIndex variable */
            commandIndex = i;

            /* Remove the command from the input so we can clean the input from spaces and tabs */
            for(j = 0; j < strlen(commandsNames[i]); j++)
            {
                input[j] = ' ';
            }

            /* If the command is stop we will check for extraneous text after the end of the command */
            if(commandIndex == 7)
            {
                /* Check for extreneous text after the command */
                for(j = strlen(commandsNames[i]); j < strlen(input); j++)
                {
                    /* If there is a char after the end of the command we will print an error message and clear the clearInput array */
                    if(input[j] != ' ' && input[j] != '\t' && input[j] != '\n' && input[j] != '\0') 
                    {
                        printf("Extraneous text after end of command\n"); /* Print an error message */
                        clearInput[0] = '\0'; /* Clear the clearInput array */
                        return UNDEFINED_COMMAND; /* Break the function */
                    }
                }
                /* Return the index of the command (7 for the stop command) */
                return commandIndex;
            }
            break; /* Break the loop */
        }
    }

    /* If the command is not found in the input we will print an error message and clear the clearInput array */
    if(commandIndex == UNDEFINED_COMMAND)
    {
        printf("Undefined command name\n"); /* Print an error message */
        clearInput[0] = '\0'; /* Clear the clearInput array */
        return UNDEFINED_COMMAND; /* Break the function */
    }

    /* Find the end of the command */
    for(i = strlen(input) - 1; i > 0; i--)
    {   
        /* If we are at a space or a tab we will continue to the next character */
        if(input[i] == ' ' || input[i] == '\t')
        {
            continue;
        }
        /* If we are at a char (a letter or a number) and the previous char is a space or a tab (which means we are at the start of the last argument) we will break the loop */
        else
        {
            if(input[i-1] == ' ' || input[i-1] == '\t') /* If the previous character is a space or a tab we will break the loop */
            {
                endOfCommandIndex = i; /* Save the index of the end of the command */
                break;
            }
        }
    }

    /* Clean the input from spaces and tabs while checking for errors */
    for(i = 0; input[i] != '\0'; i++) /* Go through the input */
    { 
        /* If we got to the last argument we will get the last argument and break the loop */
        if(i == endOfCommandIndex)
        {
            /* We keep the starting index of the last argument */
            startingIndex = endOfCommandIndex;

            /* Go through the last argument and put it in the clearInput array */
            for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ' ' && input[j+startingIndex] != '\t'; j++)
            {
                clearInput[j+lastCommaIndex] = input[j+startingIndex];
            }

            /* Keep the index of the end of the last argument */
            endOfInputIndex = j+startingIndex;
            /* Move j to the last character of the last argument (not anything after it) */
            j--; 

            /* Remove spaces and tabs from the end of the last argument at the clearInput array */
            while(input[j+startingIndex] == ' ' || input[j+startingIndex] == '\t')
            {
                j--;
            }

            /* If the last character of the last argument is a comma we will remove it */
            if(input[endOfInputIndex - 1] == ',')
            {
                endOfInputIndex--;
            }

            /* Put a comma at the end for all other commandsNames (for easier parsing) */
            clearInput[j+lastCommaIndex+1] = ',';
            /* Add a null terminator to the clearInput array */
            clearInput[j+lastCommaIndex+2] = '\0';

            /* Check for extreneous text after the command */
            for(j = endOfInputIndex; input[j] != '\0'; j++)
            {
                /* If there is a char after the end of the command we will print an error message and clear the clearInput array */
                if(input[j] != ' ' && input[j] != '\t' && input[j] != '\n' && j != startingIndex)
                {
                    printf("Extraneous text after end of command\n"); /* Print an error message */
                    clearInput[0] = '\0'; /* Clear the clearInput array */
                    return commandIndex; /* Break the function */
                }
            }
            
            /* If we are still waiting for a comma we will print an error message and clear the clearInput array */
            if(isWaitingForComma == TRUE)
            {
                printf("Missing comma\n");
                clearInput[0] = '\0';
                return commandIndex;
            }

            /* Return the index of the command */
            return commandIndex; 
        }
        /* Skip spaces and tabs */
        else if(input[i] == ' ' || input[i] == '\t')
        {
            continue;
        }
        /* If we got to a comma, we'll check if it's legal */
        else if(input[i] == ',')
        {
             /* If we weren't waiting for a comma we will print an error message and clear the clearInput array */
            if(isWaitingForComma == FALSE)
            {
                if(input[i-1] == ',')
                {
                    printf("Multiple consecutive commas\n");
                }
                else
                {
                    printf("Illegal comma\n"); /* Print an error message */
                }
                clearInput[0] = '\0'; /* Clear the clearInput array */
                return commandIndex; /* Break the function */
            }
            isWaitingForComma = FALSE; /* If we were waiting for a comma we will set the flag to false */
        }
        /* If we got to a char (a letter or a number) and the previous char was a space or a tab (which means we are at the start of an argument) and we weren't waiting for a comma we will put the argument in the clearInput array */
        else if(input[i] != ' ' && input[i] != '\t' && (i-1 < 0 || input[i-1] == ' ' || input[i-1] == '\t') && isWaitingForComma == FALSE)
        {
            /* Save the starting index of the current argument */
            startingIndex = i;

            /* Go through the current argument and put it in the clearInput array - stop when we get to the end of the input or to a comma */
            for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ',' && input[j+startingIndex] != ' '; j++)
            {
                clearInput[j+lastCommaIndex] = input[j+startingIndex]; /* Insert the current argument to the clearInput array */
            }

            /* Decrease the index by 1 so we'll get the last character of the current argument (not the comma after it) */
            j--;
             /* Remove spaces and tabs from the end of the current argument at the clearInput array */
            while(input[j+startingIndex] == ' ' || input[j+startingIndex] == '\t')
            {
                j--;
            }
            /* Make sure we are waiting for a comma - we need a comma after each argument (except for the last one) */
            isWaitingForComma = TRUE; 
            /* Put a comma at the end of the argument in the clearInput array for easier parsing */
            clearInput[j+lastCommaIndex+1] = ',';
            /* Move the index to the end of the current argument in the clearInput array (j+2 because of the comma and the null terminator) */
            lastCommaIndex += j+2;
            /* Move the index to the end of the current argument */
            i = j + startingIndex;
        }
        /* Check if there is a missing comma */
        else if(input[i] != ' ' && input[i] != '\t' && (i-1<0 || input[i-1] == ' ' || input[i-1] == '\t') && isWaitingForComma == TRUE)
        {
            printf("Missing comma\n"); /* Print an error message */
            clearInput[0] = '\0'; /* Clear the clearInput array */
            return commandIndex; /* Break the function */
        }
    }
    return commandIndex; /* Return the index of the command (will never get here - just to avoid a warning) */
}

/* A function that gets a line from the input */
void getLine(char *str)
{
    /* Go through the input and put it in the str array */
    while((*str = getchar()) != EOF && *str != '\n')
    {
        str++;
    }
    /* Add a null terminator to the str array */
    *str = '\0';
}

void parseArguments(char* input, int command, MatrixMap matrixMap[])
{
    /*
    Declarations:
    i - a counter for the loop
    argumentsIndex - the index of the current argument in the stringArguments array
    tempIndex - a counter for the temp string
    temp - a temporary string that holds the current argument
    stringArguments - an array of the strings representing the arguments
    */
    int i = 0, argumentsIndex = 0, tempIndex = 0;
    char temp[MAX_ARG_LENGTH] = "", *stringArguments[MAX_AMOUNT_OF_ARGUMENTS] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

    /* Go through the input and put the arguments in the stringArguments array */
    for(i = 0; i < strlen(input); i++, tempIndex++)
    {
        /* If we got to a comma (end of argument) we will put the current argument in the stringArguments array */
        if(input[i] == ',')
        {
            /* Add a null terminator to the temp string (for strdup) */
            temp[tempIndex] = '\0';
            /* Insert the current argument to the stringArguments array using strdup */
            stringArguments[argumentsIndex] = strdup(temp);
            /* Add a null terminator to the current argument */
            stringArguments[argumentsIndex][tempIndex] = '\0';
            /* Move to the next argument */
            argumentsIndex++;
            /* Reset the tempIndex (it will be 0 because of the for loop) */
            tempIndex = -1;
            /* Continue to the next character */
            continue;
        }
         /* Insert the current character to the temp string */
        temp[tempIndex] = input[i];
    }

    /* Call the right function for the command */
    switch (command)
    {
        /* Call the function that breaks the input for the read_mat command */
        case 0:
            breakInputReadMat(stringArguments, matrixMap);
            break;
        /* Call the function that breaks the input for the print_mat command */
        case 1:
            breakInputPrintMat(stringArguments, matrixMap);
            break;
        /* add_mat, sub_mat, mul_mat all take input in the same format */
        case 2:
        case 3:
        case 4:
            /* Call the function that breaks the input for the add_mat, sub_mat and mul_mat commandsNames */
            breakInputThreeMat(stringArguments, matrixMap, command);
            break;
        /* Call the function that breaks the input for the mul_scalar command */
        case 5:
            breakInputMulScalar(stringArguments, matrixMap);
            break;
        /* Call the function that breaks the input for trans_mat command */
        case 6:
            breakInputTransMat(stringArguments, matrixMap);
            break; 
    }
}

/* A function that breaks the input for the read_mat command */
void breakInputReadMat(char* stringArguments[], MatrixMap matrixMap[])
{
    /*
    Declarations:
    i, j - counters for the loops
    matrixIndex - the index of the matrix in the lookup table
    numList - an array of 16 zeros for initializing the matrices
    */
    int i = 0, j = 0, matrixIndex = -1;
    float numList[AMOUNT_OF_NUMBERS] = {0};

    /* find the matrix in the lookup table */
    for(i = 0; i < AMOUNT_OF_MATRICES; i++)
    {
        /* if the matrix name is found in the lookup table */
        if(strcmp(stringArguments[0], matrixMap[i].matrixName) == 0)
        {
            /* Save the index of the matrix in the matrixIndex variable */
            matrixIndex = i; 
            break;
        }
    }

    /* if the matrix name is not found in the lookup table */
    if(matrixIndex == -1) 
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

    /* Check for missing arguments */
    for(i = 0; i < 2; i++)
    {
        /* if the current argument is an empty string */
        if(strcmp(stringArguments[i], "") == 0)
        {
            printf("Missing argument\n"); /* print an error message */
            return; /* break the function */
        }
    }

    /* check if the rest of the arguments are valid (real numbers) and put them in the arguments array */
    for(i = 1; i < MAX_AMOUNT_OF_ARGUMENTS && stringArguments[i] != NULL && strcmp(stringArguments[i], "") != 0; i++)
    {
        /* go through the current argument */
        for(j = 0; j < strlen(stringArguments[i]); j++)
        {
            /* if the current character is a digit, a dot or a minus, everything is fine */
            if(isdigit(stringArguments[i][j]) || stringArguments[i][j] == '.' || stringArguments[i][j] == '-')
            {
                continue;
            }
            /* if the current character is not a part of a real number */
            else
            {
                printf("Argument is not a real number\n"); /* print an error message */
                return; /* break the function */
            }
        }
        /* put the current argument in the numList array */
        numList[i-1] = atof(stringArguments[i]);
    }

    /* call the read_mat function */
    read_mat(matrixMap[matrixIndex].matrix, numList);
}  

/* A function that breaks the input for the print_mat command */
void breakInputPrintMat(char* stringArguments[], MatrixMap matrixMap[])
{
    /* i - a counter for the loop, matrixIndex - the index of the matrix in the lookup table */
    int i = 0, matrixIndex = -1;

    /* If the matrix name is an empty string */
    if(strcmp(stringArguments[0], "") == 0)
    {
        printf("Missing argument\n"); /* print an error message */
        return;
    }

    /* find the matrix in the lookup table */
    for(i = 0; i < AMOUNT_OF_MATRICES; i++)
    {
        /* if the matrix name is found in the lookup table */
        if(strcmp(stringArguments[0], matrixMap[i].matrixName) == 0)
        {
            matrixIndex = i; /* Save the index of the matrix in the matrixIndex variable */
            break;
        }
    }

    /* if the matrix name was not found in the lookup table */
    if(matrixIndex == -1)
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

    /* check if there is extraneous text after the end of the command */
    for(i = 1; i < MAX_AMOUNT_OF_ARGUMENTS; i++)
    {  
        /* if the current argument is not an empty string */
        if(stringArguments[i] != NULL && strcmp(stringArguments[i], "") != 0)
        {
            printf("Extraneous text after end of command\n"); /* print an error message */
            return;
        }
    }

    /* call the print_mat function */
    print_mat(matrixMap[matrixIndex].matrix);
}

/* A function that breaks the input for the add_mat, sub_mat and mul_mat commandsNames */
void breakInputThreeMat(char* stringArguments[], MatrixMap matrixMap[], int command)
{
    /* i - counter for the loops, matrixIndexOne, matrixIndexTwo, matrixIndexThree - the indexes of the matrices in the lookup table */
    int i = 0, matrixIndexOne = -1, matrixIndexTwo = -1, matrixIndexThree = -1;

    /* if any of the arguments is an empty string */
    if(strcmp(stringArguments[0], "") == 0 || strcmp(stringArguments[1], "") == 0 || strcmp(stringArguments[2], "") == 0)
    {
        printf("Missing argument\n"); /* print an error message */
        return;
    }

    /* find the matrices in the lookup table */
    for(i = 0; i < AMOUNT_OF_MATRICES; i++)
    {
        if(strcmp(stringArguments[0], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matrixIndexOne = i; /* put the index of the matrix in the matrixIndex variable */
        }
        if(strcmp(stringArguments[1], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matrixIndexTwo = i; /* put the index of the matrix in the matrixIndex variable */
        }
        if(strcmp(stringArguments[2], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matrixIndexThree = i; /* put the index of the matrix in the matrixIndex variable */
        }
    }

    /* if the matrix name was not found in the lookup table */
    if(matrixIndexOne == -1 || matrixIndexTwo == -1 || matrixIndexThree == -1)
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

    /* check if there is extraneous text after the end of the command */
    for(i = 3; i < MAX_AMOUNT_OF_ARGUMENTS; i++)
    {
        /* if the current argument is not an empty string */
        if(stringArguments[i] != NULL && strcmp(stringArguments[i], "") != 0)
        {
            printf("Extraneous text after end of command\n");
            return;
        }
    }

    /* call the right function for the command */
    switch(command)
    {
        case 2:
            add_mat(matrixMap[matrixIndexOne].matrix, matrixMap[matrixIndexTwo].matrix, matrixMap[matrixIndexThree].matrix);
            break;
        case 3:
            sub_mat(matrixMap[matrixIndexOne].matrix, matrixMap[matrixIndexTwo].matrix, matrixMap[matrixIndexThree].matrix);
            break;
        case 4:
            mul_mat(matrixMap[matrixIndexOne].matrix, matrixMap[matrixIndexTwo].matrix, matrixMap[matrixIndexThree].matrix);
            break;
    }
}

/* A function that breaks the input for the mul_scalar command */
void breakInputMulScalar(char* stringArguments[], MatrixMap matrixMap[])
{
    /* i - counter for the loops, matOneIndex, matTwoIndex - the indexes of the matrices in the lookup table */
    int i = 0, matOneIndex = -1, matTwoIndex = -1;

    /* if the current argument is an empty string */
    if(strcmp(stringArguments[0], "") == 0 || strcmp(stringArguments[1], "") == 0 || strcmp(stringArguments[2], "") == 0)
    {
        printf("Missing argument\n"); /* print an error message */
        return;
    }

     /* find the matrix in the lookup table */
    for(i = 0; i < AMOUNT_OF_MATRICES; i++)
    {
        if(strcmp(stringArguments[0], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matOneIndex = i; /* put the index of the matrix in the matrixIndex variable */
        }
        if(strcmp(stringArguments[2], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matTwoIndex = i; /* put the index of the matrix in the matrixIndex variable */
        }
    }

    /* if the matrix name is not found in the lookup table */
    if(matOneIndex == -1 || matTwoIndex == -1)
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }
    
    /* check if the scalar is a scalar */
    for(i = 0; i < strlen(stringArguments[1]); i++)
    {   
        /* if the current character is a digit, a dot or a minus */
        if(isdigit(stringArguments[1][i]) || stringArguments[1][i] == '.' || stringArguments[1][i] == '-')
        {
            continue; /* continue to the next character */
        }
        /* if the current character is not a part of a real number */
        else
        {
            printf("Argument is not a scalar\n"); /* print an error message */
            return; /* break the function */
        }
    }

    /* check if there is extraneous text after the end of the command */
    for(i = 3; i < MAX_AMOUNT_OF_ARGUMENTS; i++)
    {
        /* if the current argument is not an empty string */
        if(stringArguments[i] != NULL && strcmp(stringArguments[i], "") != 0)
        {
            printf("Extraneous text after end of command\n");
            return;
        }
    }

    /* call the mul_scalar function */
    mul_scalar(matrixMap[matOneIndex].matrix, atof(stringArguments[1]), matrixMap[matTwoIndex].matrix);
}

/* A function that breaks the input for the trans_mat command */
void breakInputTransMat(char* stringArguments[], MatrixMap matrixMap[])
{
    /*
    Declarations:
    i - counter for the loops, matOneIndex, matTwoIndex - the indexes of the matrices in the lookup table
    tempMat - a temporary matrix for the trans_mat function
    numList - an array of 16 zeros for initializing the tempMat matrix
    */
    int i = 0, matOneIndex = -1, matTwoIndex = -1;
    mat tempMat;
    float numList[AMOUNT_OF_NUMBERS] = {0};

    /* initialize the tempMat matrix to zero using an array of 16 zeros and the read_mat function */
    read_mat(&tempMat, numList);

    /* if the current argument is an empty string */
    if(strcmp(stringArguments[0], "") == 0 || strcmp(stringArguments[1], "") == 0)
    {
        printf("Missing argument\n"); /* print an error message */
        return;
    }

    /* find the matrix in the lookup table */
    for(i = 0; i < AMOUNT_OF_MATRICES; i++)
    {
        if(strcmp(stringArguments[0], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matOneIndex = i; /* put the index of the matrix in the matrixIndex variable */
        }
        if(strcmp(stringArguments[1], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matTwoIndex = i; /* put the index of the matrix in the matrixIndex variable */
        }
    }

    /* if the matrix name is not found in the lookup table */
    if(matOneIndex == -1 || matTwoIndex == -1)
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

    /* check if there is extraneous text after the end of the command */
    for(i = 2; i < MAX_AMOUNT_OF_ARGUMENTS; i++)
    {
        /* if the current argument is not an empty string */
        if(stringArguments[i] != NULL && strcmp(stringArguments[i], "") != 0)
        {
            printf("Extraneous text after end of command\n");
            return;
        }
    }

    /* call trans_mat function */
    trans_mat(matrixMap[matOneIndex].matrix, matrixMap[matTwoIndex].matrix, &tempMat);
}

/* A function that duplicates a string */
char *strdup(const char *src) {
    /* dst - a pointer to the new string */
    char *dst = malloc(strlen (src) + 1);
    /* if the new string is NULL */
    if (dst == NULL) return NULL;
    /* copy the src string to the dst string */
    strcpy(dst, src);
    /* return the dst string */
    return dst;
}