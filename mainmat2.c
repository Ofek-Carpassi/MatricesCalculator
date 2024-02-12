/* A calculator for matrices. */
/* The main function, all the intercation with the user and the command analysis. */
/* Input: <command> <arg1>, <arg2>, <arg3> (all the arguments are separated by commas and spaces) */

/*
A blueprint for the program:
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
void getInput(char* input, char* clearInput, char* commandsNames[]); /* A function that gets the input from the user and cleans it while checking for errors */
void getLine(char* input); /* A function that gets a line from the input */
int parseCommand(char* input, char* command, char* commandsNames[]); /* A function that parses the command from the input */
void parseArguments(char* input, int command, MatrixMap matrixMap[]); /* A function that parses the arguments from the input */
void breakInputReadMat(char* stringArguments[], MatrixMap matrixMap[]); /* A function that breaks the input for the read_mat command */
void breakInputPrintMat(char* stringArguments[], MatrixMap matrixMap[]); /* A function that breaks the input for the print_mat command */
void breakInputThreeMat(char* stringArguments[], MatrixMap matrixMap[], int command); /* A function that breaks the input for the add_mat, sub_mat and mul_mat commandsNames */
void breakInputMulScalar(char* stringArguments[], MatrixMap matrixMap[]); /* A function that breaks the input for the mul_scalar command */
void breakInputTransMat(char* stringArguments[], MatrixMap matrixMap[]); /* A function that breaks the input */
char *strdup(const char *src); /* A function that duplicates a string */

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

/* The main function */
int main()
{
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F; /* Defining all the matrices */
    char input[] = "", clearInput[MAX_LENGTH_OF_CLEAN_INPUT] = "", command[MAX_LENGTH_OF_COMMAND] = "", *commandsNames[] = {"read_mat", "print_mat", "add_mat", "sub_mat", "mul_mat", "mul_scalar", "trans_mat", "stop"};
    float numList[AMOUNT_OF_NUMBERS] = {0}; /* A number list of 0's for initializing the matrices */
    MatrixMap matrixMap[AMOUNT_OF_MATRICES]; /* A lookup table for the matrices */
    int commandIndex = -1, i = 0; /* The index of the command in the commandsNames array */

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
        print_mat(&MAT_F);
        /* Make input and clearInput empty again. */
        for(i = 0; i < strlen(input); i++)
        {
            input[i] = '\0';
        }
        for(i = 0; i < strlen(clearInput); i++)
        {
            clearInput[i] = '\0';
        }
        getInput(input, clearInput, commandsNames); /* Get the input from the user and clean it */
        if(clearInput[0] == '\0') /* If the input is invalid skip the rest of the loop */
        {
            continue;
        }
        commandIndex = parseCommand(clearInput, command, commandsNames); /* Parse the command from the input */
        if(command[0] == '\0' || commandIndex == -1) /* If the command is invalid skip the rest of the loop */
        {
            continue;
        }
        if(commandIndex == 7) /* If the command is stop break the loop */
        {
            break;
        }
        parseArguments(clearInput, commandIndex, matrixMap); /* Parse the arguments from the input */
    }

    return 0; /* End the program */
}

/* The function that gets the input from the user and cleans it while checking for errors */
void getInput(char* input, char* clearInput, char* commandsNames[])
{
    /*
    Declarations:
    i, j - counters for several loops
    startingIndex - the index of the start of the current argument
    lastCommaIndex - the index of the last comma in the clearInput array
    isWaitingForComma - a flag that tells us if we are waiting for a comma
    isInCommand - a flag that tells us if we are in the command (the first argument)
    endOfCommandIndex - the index of the start of the last argument
    endOfInputIndex - the index of the end of the input
    */
    int i = 0, j = 0, startingIndex = 0, lastCommaIndex = 0, isWaitingForComma = FALSE, isInCommand = TRUE, endOfCommandIndex = 0, endOfInputIndex = 0, isThereCommand = FALSE;
    
    /*
    read_mat MAT_A, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    read_mat MAT_C, 100, 100, 100
    read_mat MAT_A, 5, 5, 5
    */

    getLine(input); /* Get the input from the user */

    for(i = 0; i < AMOUNT_OF_COMMANDS; i++)
    {
        if(strstr(input, commandsNames[i]) != NULL)
        {
            isThereCommand = TRUE;
            break;
        }
    }

    if(isThereCommand == FALSE)
    {
        printf("Undefined command name\n");
        clearInput[0] = '\0';
        return;
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

            /* We go through the last argument and put it in the clearInput array */
            for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ' ' && input[j+startingIndex] != '\t'; j++)
            {
                /* Insert the last argument to the clearInput array */
                clearInput[j+lastCommaIndex] = input[j+startingIndex];
            }
            /* We keep the index of the end of the last argument */
            endOfInputIndex = j+startingIndex;
            j--; /* Move the index to the end of the current argument */
            /* The loop is used to remove spaces and tabs from the end of the last argument in the clearInput array */
            while(input[j+startingIndex] == ' ' || input[j+startingIndex] == '\t')
            {
                j--;
            }

            /* If the last character of the last argument is a comma we will remove it - we deacrease the endOfInputIndex by 1 so we'll get an error */
            if(input[endOfInputIndex - 1] == ',')
            {
                endOfInputIndex--;
            }

            /* Put a comma at the end for all other commandsNames (for easier parsing) */
            clearInput[j+lastCommaIndex+1] = ',';
            clearInput[j+lastCommaIndex+2] = '\0';

            /* Check for extreneous text after the command */
            for(j = endOfInputIndex; input[j] != '\0'; j++)
            {
                /* If there is a char after the end of the command we will print an error message and clear the clearInput array */
                if(input[j] != ' ' && input[j] != '\t' && input[j] != '\n' && j != startingIndex)
                {
                    printf("Extraneous text after end of command\n"); /* Print an error message */
                    clearInput[0] = '\0'; /* Clear the clearInput array */
                    return; /* Break the function */
                }
            }
            /* Add the null terminator to the clearInput array */
            clearInput[j+lastCommaIndex+1] = '\0';
            if(isWaitingForComma == TRUE)
            {
                printf("Missing comma\n");
                clearInput[0] = '\0';
                return;
            }
            return; /* Break the loop (return to the main function) */
        }
        /* Skip spaces and tabs */
        else if(input[i] == ' ' || input[i] == '\t')
        {
            continue;
        }
        /* If we got to a comma, we'll check if it's legal */
        else if(input[i] == ',')
        {
            if(isWaitingForComma == FALSE) /* If we weren't waiting for a comma we will print an error message and clear the clearInput array */
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
                return; /* Break the function */
            }
            isWaitingForComma = FALSE; /* If we were waiting for a comma we will set the flag to false */
        }
        /* If we got to a char (a letter or a number) and the previous char was a space or a tab (which means we are at the start of an argument) and we weren't waiting for a comma we will put the argument in the clearInput array */
        else if(input[i] != ' ' && input[i] != '\t' && (i-1 < 0 || input[i-1] == ' ' || input[i-1] == '\t') && isWaitingForComma == FALSE)
        {
            if(isInCommand == TRUE) /* If we are in the first argument - we will use a different way to get the argument */
            {
                isInCommand = FALSE; /* Set the flag to false - we won't be in the command anymore */
                startingIndex = i; /* Save the starting index of the current argument */

                /* Go through the current argument and put it in the clearInput array */
                for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ' '; j++)
                {
                    if(input[j+startingIndex] == ',') /* After the command we shouldn't have a comma */
                    {
                        printf("Illegal comma\n"); /* Print an error message */
                        clearInput[0] = '\0'; /* Clear the clearInput array */
                        return; /* Break the function */
                    }
                    clearInput[j] = input[j+startingIndex]; /* Insert the current argument to the clearInput array */
                }

                /* Decrease the index by 1 so we'll get the last character of the current argument (not the space or the tab after it) */
                j--;
                while(input[j+startingIndex] == ' ' || input[j+startingIndex] == '\t') /* Remove spaces and tabs from the end of the current argument */
                {
                    j--;
                }
                isWaitingForComma = FALSE; /* Make sure we are not waiting for a comma - no need for a comma after the command */

                clearInput[j+1] = ','; /* Put a comma at the end for all other commandsNames (for easier parsing) */
            }
            /* If we are not in the first argument */
            else
            {
                startingIndex = i; /* Save the starting index of the current argument */

                /* Go through the current argument and put it in the clearInput array - stop when we get to the end of the input or to a comma */
                for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ',' && input[j+startingIndex] != ' '; j++)
                {
                    clearInput[j+lastCommaIndex] = input[j+startingIndex]; /* Insert the current argument to the clearInput array */
                }

                /* Decrease the index by 1 so we'll get the last character of the current argument (not the comma after it) */
                j--;
                while(input[j+startingIndex] == ' ' || input[j+startingIndex] == '\t') /* Remove spaces and tabs from the end of the current argument */
                {
                    j--;
                }
                isWaitingForComma = TRUE; /* Make sure we are waiting for a comma - we need a comma after the current argument */
            }

            /* Put a comma at the end for all other commandsNames (for easier parsing) */
            clearInput[j+lastCommaIndex+1] = ',';
            /* Move the index to the end of the current argument in the clearInput array (j+2 because of the comma and the null terminator) */
            lastCommaIndex += j+2;
            /* Move the index to the end of the current argument */
            i = j + startingIndex;
        }
        /* Check if there is a missing comma */
        else if(input[i] != ' ' && input[i] != '\t' && (i-1<0 || input[i-1] == ' ' || input[i-1] == '\t') && isWaitingForComma == TRUE)
        {
            printf("Missing comma\n");
            clearInput[0] = '\0';
            for(lastCommaIndex = 0; lastCommaIndex < strlen(clearInput); lastCommaIndex++)
            {
                if(clearInput[lastCommaIndex] == '\0')
                {
                    break;
                }
                clearInput[lastCommaIndex] = '\0';
            }
            return;
        }
    }
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

/* A function that parses the command from the input */
int parseCommand(char* input, char* command, char* commandsNames[])
{
    /* 
    Declarations:
    i - a counter for several loops
    commandEndIndex - the index of the end of the command
    commandIndex - the index of the command in the commandsNames array
    */
    int i = 0, commandEndIndex = 0, commandIndex = -1;

    /* Go through the input and put the command in the command array */
    for(i = 0; i < strlen(input); i++)
    {
        if(input[i] == ',') /* If we got to a comma we will break the loop */
        {
            break;
        }
        /* Insert the current character to the command array - make sure the memory is different */
        command[i] = input[i];
    }
    command[i] = '\0'; /* Add a null terminator to the command string */
    commandEndIndex = i; /* Save the index of the end of the command */

    for(i = 0; i < AMOUNT_OF_COMMANDS; i++) /* Find the command in the commandsNames array */
    {
        if(strcmp(command, commandsNames[i]) == 0) /* If the command is found in the commandsNames array */
        {
            commandIndex = i; /* Save the index of the command in the commandsNames array */
            break; /* Break the loop */
        }
    }

    /* If the command was not found in the commandsNames array */
    if(commandIndex == -1)
    {
        printf("Undefined command name\n"); /* Print an error message */
        command[0] = '\0'; /* Clear the command array */
        return -1; /* Return -1 (an error) */
    }

    if(commandIndex == STOP_COMMAND_INDEX)
    {
        for(i = commandEndIndex+1; i < strlen(input); i++) /* Check if there is extraneous text after the command */
        {
            /* If there is a char that is not a white space or a null terminator we will print an error message and clear the command array */
            if(input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\0')
            {
                command[0] = '\0'; /* Clear the command array */
                printf("Extraneous text after the command\n"); /* Print an error message */
                return -1; /* Return -1 (an error) */
            }
        }
        return commandIndex; /* Return the index of the command in the commandsNames array */
    }

    /* remove the command from the cleaned input */
    for(i = commandEndIndex; i < strlen(input); i++)
    {
        input[i-commandEndIndex] = input[i+1]; /* Move every char commandEndIndex places to the left */
    }

    /* Add a null terminator to the end of the cleaned input */
    input[i-commandEndIndex] = '\0';

    /* Return the index of the command in the commandsNames array */
    return commandIndex;
}

void parseArguments(char* input, int command, MatrixMap matrixMap[])
{
    /*
    Declarations:
    i - a counter for several loops
    j - a counter for several loops
    argumentsIndex - the index of the current argument in the stringArguments array
    tempIndex - the index of the current character in the temp array
    isDigit - a flag that tells us if the current character is a digit
    temp - a temporary string that holds the current argument
    stringArguments - an array of strings that holds the arguments
    */
    int i = 0, argumentsIndex = 0, tempIndex = 0;
    char temp[MAX_ARG_LENGTH] = "", *stringArguments[MAX_AMOUNT_OF_ARGUMENTS] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
    /* Go through the input and put the arguments in the stringArguments array */
    for(i = 0; i < strlen(input); i++, tempIndex++)
    {
        if(input[i] == ',') /* If we got to a comma (end of argument) we will put the current argument in the stringArguments array */
        {
            temp[tempIndex] = '\0'; /* Add a null terminator to the current argument */
            stringArguments[argumentsIndex] = strdup(temp); /* Insert the current argument to the stringArguments array */
            stringArguments[argumentsIndex][tempIndex] = '\0'; /* Add a null terminator to the current argument */
            argumentsIndex++; /* Move to the next argument */
            tempIndex = -1; /* Reset the tempIndex */
            continue; /* Continue to the next character */
        }
        temp[tempIndex] = input[i]; /* Insert the current character to the temp array */
    }

    /* Call the right function for the command */
    switch (command)
    {
        case 0:
            breakInputReadMat(stringArguments, matrixMap); /* Call the function that breaks the input for the read_mat command */
            break;
        case 1:
            breakInputPrintMat(stringArguments, matrixMap); /* Call the function that breaks the input for the print_mat command */
            break;
        case 2:
        case 3:
        case 4:
            breakInputThreeMat(stringArguments, matrixMap, command); /* Call the function that breaks the input for the add_mat, sub_mat and mul_mat commandsNames */
            break;
        case 5:
            breakInputMulScalar(stringArguments, matrixMap); /* Call the function that breaks the input for the mul_scalar command */
            break;
        case 6:
            breakInputTransMat(stringArguments, matrixMap);
            break; 
    }
}

/* A function that breaks the input for the read_mat command */
void breakInputReadMat(char* stringArguments[], MatrixMap matrixMap[])
{
    /*  i, j - counters for several loops */
    int i = 0, j = 0;
    float numList[AMOUNT_OF_NUMBERS] = {0}; /* A number list of 0's for initializing the matrices */
    float arguments[MAX_AMOUNT_OF_ARGUMENTS] = {-1}; /* An array of -1's for the arguments */

    for(i = 0; i < AMOUNT_OF_MATRICES; i++) /* find the matrix in the lookup table */
    {
        if(strcmp(stringArguments[0], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            arguments[0] = i; /* put the index of the matrix in the arguments array */
            break; /* break the loop */
        }
    }

    if(arguments[0] == -1) /* if the matrix name is not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

    for(i = 0; i < 2; i++)
    {
        if(strcmp(stringArguments[i], "") == 0) /* if the current argument is an empty string */
        {
            printf("Missing argument\n");
            return;
        }
    }

    /* check if the rest of the arguments are valid (real numbers) and put them in the arguments array */
    for(i = 1; i < MAX_AMOUNT_OF_ARGUMENTS && stringArguments[i] != NULL && strcmp(stringArguments[i], "") != 0; i++)
    {
        for(j = 0; j < strlen(stringArguments[i]); j++) /* go through the current argument */
        {
            if(isdigit(stringArguments[i][j]) || stringArguments[i][j] == '.' || stringArguments[i][j] == '-') /* if the current character is a digit, a dot or a minus */
            {
                continue; /* continue to the next character */
            }
            else /* if the current character is not a digit, a dot or a minus */
            {
                printf("Argument is not a real number\n"); /* print an error message */
                return; /* break the function */
            }
        }
        arguments[i] = atof(stringArguments[i]); /* put the current argument in the arguments array */
    }

    for(i = 1; i < MAX_AMOUNT_OF_ARGUMENTS; i++) /* put the rest of the arguments in the numList array */
    {
        if(arguments[i] == -1) /* if the current argument is not a real number */
        {
            break; /* break the loop */
        }
        numList[i-1] = arguments[i];
    }

    read_mat(matrixMap[(int)arguments[0]].matrix, numList); /* call the read_mat function */
}  

/* A function that breaks the input for the print_mat command */
void breakInputPrintMat(char* stringArguments[], MatrixMap matrixMap[])
{
    int i = 0, j = 0;
    /* A variable that holds the index of the matrix in the lookup table */
    int matrixIndex = -1;

    if(strcmp(stringArguments[0], "") == 0) /* if the current argument is an empty string */
    {
        printf("Missing argument\n");
        return;
    }

    for(i = 0; i < AMOUNT_OF_MATRICES; i++) /* find the matrix in the lookup table */
    {
        if(strcmp(stringArguments[0], matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matrixIndex = i; /* put the index of the matrix in the matrixIndex variable */
            break; /* break the function */
        }
    }

    for(j = 1; j < MAX_AMOUNT_OF_ARGUMENTS; j++) /* check if there is extraneous text after the end of the command */
    {
        if(stringArguments[j] != NULL && strcmp(stringArguments[j], "") != 0)
        {
            printf("Extraneous text after end of command\n");
            return;
        }
    }

    if(matrixIndex == -1) /* if the matrix name is not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

    print_mat(matrixMap[matrixIndex].matrix); /* call the print_mat function */
}

void breakInputThreeMat(char* stringArguments[], MatrixMap matrixMap[], int command)
{
    int i = 0, j = 0;
    /* A variable that holds the index of the matrix in the lookup table */
    int matrixIndexOne = -1, matrixIndexTwo = -1, matrixIndexThree = -1;

    if(strcmp(stringArguments[0], "") == 0 || strcmp(stringArguments[1], "") == 0 || strcmp(stringArguments[2], "") == 0) /* if the current argument is an empty string */
    {
        printf("Missing argument\n");
        return;
    }

    for(i = 0; i < AMOUNT_OF_MATRICES; i++) /* find the matrix in the lookup table */
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

    for(j = 3; j < MAX_AMOUNT_OF_ARGUMENTS; j++) /* check if there is extraneous text after the end of the command */
    {
        if(stringArguments[j] != NULL && strcmp(stringArguments[j], "") != 0)
        {
            printf("Extraneous text after end of command\n");
            return;
        }
    }

    if(matrixIndexOne == -1 || matrixIndexTwo == -1 || matrixIndexThree == -1) /* if the matrix name is not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

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

void breakInputMulScalar(char* stringArguments[], MatrixMap matrixMap[])
{
    int i = 0;
    int matOneIndex = -1, matTwoIndex = -1;

    if(strcmp(stringArguments[0], "") == 0 || strcmp(stringArguments[1], "") == 0 || strcmp(stringArguments[2], "") == 0) /* if the current argument is an empty string */
    {
        printf("Missing argument\n");
        return;
    }

    for(i = 0; i < AMOUNT_OF_MATRICES; i++) /* find the matrix in the lookup table */
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

    for(i = 3; i < MAX_AMOUNT_OF_ARGUMENTS; i++) /* check if there is extraneous text after the end of the command */
    {
        if(stringArguments[i] != NULL && strcmp(stringArguments[i], "") != 0)
        {
            printf("Extraneous text after end of command\n");
            return;
        }
    }

    if(matOneIndex == -1 || matTwoIndex == -1) /* if the matrix name is not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

    /* check if the scalar is a scalar */
    for(i = 0; i < strlen(stringArguments[1]); i++)
    {
        if(isdigit(stringArguments[1][i]) || stringArguments[1][i] == '.' || stringArguments[1][i] == '-') /* if the current character is a digit, a dot or a minus */
        {
            continue; /* continue to the next character */
        }
        else /* if the current character is not a digit, a dot or a minus */
        {
            printf("Argument is not a scalar\n"); /* print an error message */
            return; /* break the function */
        }
    }

    mul_scalar(matrixMap[matOneIndex].matrix, atof(stringArguments[1]), matrixMap[matTwoIndex].matrix);
}

void breakInputTransMat(char* stringArguments[], MatrixMap matrixMap[])
{
    int i = 0;
    int matOneIndex = -1, matTwoIndex = -1;
    mat tempMat;
    float numList[AMOUNT_OF_NUMBERS] = {0};

    read_mat(&tempMat, numList);

    if(strcmp(stringArguments[0], "") == 0 || strcmp(stringArguments[1], "") == 0) /* if the current argument is an empty string */
    {
        printf("Missing argument\n");
        return;
    }

    for(i = 0; i < AMOUNT_OF_MATRICES; i++) /* find the matrix in the lookup table */
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

    for(i = 2; i < MAX_AMOUNT_OF_ARGUMENTS; i++) /* check if there is extraneous text after the end of the command */
    {
        if(stringArguments[i] != NULL && strcmp(stringArguments[i], "") != 0)
        {
            printf("Extraneous text after end of command\n");
            return;
        }
    }

    if(matOneIndex == -1 || matTwoIndex == -1) /* if the matrix name is not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* break the function */
    }

    trans_mat(matrixMap[matOneIndex].matrix, matrixMap[matTwoIndex].matrix, &tempMat);
}

char *strdup(const char *src) {
    char *dst = malloc(strlen (src) + 1);
    if (dst == NULL) return NULL;
    strcpy(dst, src);
    return dst;
}