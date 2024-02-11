/* A calculator for matrices. */
/* The main function, all the intercation with the user and the command analysis. */
/* Input: <command> <arg1>, <arg2>, <arg3> (all the arguments are separated by commas and spaces) */

/*
A blueprint for the program:
main:
    - call getInput. DONE
    - call parseCommand. DONE
    - call parsearguments.
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
parseArguments:
    - get the arguments from the input. DONE
    - check if the arguments are valid for the command. Finish by 11/3/2024
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
void getInput(char* input, char* clearInput); /* The function that gets the input from the user */
void getLine(char* input); /* A function that gets a line from the input */
int parseCommand(char* input, char* command, char* commands[]); /* A function that parses the command from the input */
void parseArguments(char* input, int command, float arguments[], MatrixMap matrixMap[]); /* A function that parses the arguments from the input */
void breakInputReadMat(char* stringArguments[], float arguments[], MatrixMap matrixMap[]); /* A function that breaks the input for the read_mat command */

/* Defining all needed constants */
#define TRUE 1 /* A constant for true */
#define FALSE 0 /* A constant for false */
#define AMOUNT_OF_NUMBERS 16 /* The amount of numbers in the input */
#define AMOUNT_OF_MATRICES 6 /* The amount of matrices in the program */
#define MAX_LENGTH_OF_CLEAN_INPUT 648 /* The maximal length of the input after cleaning. The maximal input is a read mat with the 16 longest float number possible (read_mat,MAT_A,-3.40282e+38,-3.402828e+38,...,-3.40282e+38) - the length of the input will be 24+16*39 = 648, The length of -3.40282e+38 is 39 in decimal and the amount of commas is 18, the amount of numbers is 16, the length of the command name and the matrix name is 24 */
#define MAX_LENGTH_OF_COMMAND 9 /* The maximal length of the command */
#define MAX_AMOUNT_OF_ARGUMENTS 17 /* The maximal amount of arguments in the input */
#define AMOUNT_OF_COMMANDS 8 /* The amount of commands in the program */
#define STOP_COMMAND_INDEX 7 /* The index of the stop command in the commands array */
#define MAX_ARG_LENGTH 39 /* The maximal length of an argument */

/* The main function */
int main()
{
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F; /* Defining all the matrices */
    char input[] = "", clearInput[MAX_LENGTH_OF_CLEAN_INPUT] = "", command[MAX_LENGTH_OF_COMMAND] = ""; /* The input from the user and the command */
    char* commands[] = {"read_mat", "print_mat", "add_mat", "sub_mat", "mul_mat", "mul_scalar", "trans_mat", "stop"};
    float arguments[MAX_AMOUNT_OF_ARGUMENTS] = {-1}; /* The arguments of the command */
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
        if(clearInput[0] == '\0') /* If the input is invalid skip the rest of the loop */
        {
            continue;
        }

        commandIndex = parseCommand(clearInput, command, commands); /* Parse the command from the input */
        if(command[0] == '\0' || commandIndex == -1) /* If the command is invalid skip the rest of the loop */
        {
            continue;
        }
        if(commandIndex == 7) /* If the command is stop break the loop */
        {
            break;
        }

        parseArguments(clearInput, commandIndex, arguments, matrixMap); /* Parse the arguments from the input */
    }
}

/* The function that gets the input from the user and cleans it while checking for errors */
void getInput(char* input, char* clearInput)
{
    /*
    Declarations:
    i, j - counters for several loops
    startingIndex - the index of the start of the current argument
    lastCommaIndex - the index of the last comma in the clearInput array
    isWaitingForComma - a flag that tells us if we are waiting for a comma
    isInCommand - a flag that tells us if we are in the command (the first argument)
    isInEndOfCommand - a flag that tells us if we are in the end of the command (the last argument)
    endOfCommandIndex - the index of the start of the last argument
    endOfInputIndex - the index of the end of the input
    */
    int i = 0, j = 0, startingIndex = 0, lastCommaIndex = 0, isWaitingForComma = FALSE, isInCommand = TRUE, isInEndOfCommand = FALSE, endOfCommandIndex = 0, endOfInputIndex = 0;
    
    getLine(input); /* Get the input from the user */

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

            /* Put a comma at the end for all other commands (for easier parsing) */
            clearInput[j+lastCommaIndex+1] = ',';

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

            break; /* Break the loop (return to the main function) */
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
                printf("Illegal comma\n"); /* Print an error message */
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
            }
            /* If we are not in the first argument */
            else
            {
                startingIndex = i; /* Save the starting index of the current argument */

                /* Go through the current argument and put it in the clearInput array - stop when we get to the end of the input or to a comma */
                for(j = 0; input[j+startingIndex] != '\0' && input[j+startingIndex] != ','; j++)
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

            /* Put a comma at the end for all other commands (for easier parsing) */
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
int parseCommand(char* input, char* command, char* commands[])
{
    /* 
    Declarations:
    i - a counter for several loops
    commandEndIndex - the index of the end of the command
    commandIndex - the index of the command in the commands array
    */
    int i = 0, commandEndIndex = 0, commandIndex = -1;

    /* Go through the input and put the command in the command array */
    for(i = 0; i < strlen(input); i++)
    {
        if(input[i] == ',') /* If we got to a comma we will break the loop */
        {
            break;
        }
        command[i] = input[i]; /* Insert the current character to the command array */
    }    
    command[i] = '\0'; /* Add a null terminator to the command string */
    commandEndIndex = i; /* Save the index of the end of the command */

    for(i = 0; i < AMOUNT_OF_COMMANDS; i++) /* Find the command in the commands array */
    {
        if(strcmp(command, commands[i]) == 0) /* If the command is found in the commands array */
        {
            commandIndex = i; /* Save the index of the command in the commands array */
            break; /* Break the loop */
        }
    }

    /* If the command was not found in the commands array */
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
        return commandIndex; /* Return the index of the command in the commands array */
    }

    /* remove the command from the cleaned input */
    for(i = commandEndIndex; i < strlen(input); i++)
    {
        input[i-commandEndIndex] = input[i+1]; /* Move every char commandEndIndex places to the left */
    }

    /* Add a null terminator to the end of the cleaned input */
    input[i-commandEndIndex] = '\0';

    /* Return the index of the command in the commands array */
    return commandIndex;
}

void parseArguments(char* input, int command, float arguments[], MatrixMap matrixMap[])
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
    int i = 0, j = 0, argumentsIndex = 0, tempIndex = 0, isDigit = TRUE;
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
            breakInputReadMat(stringArguments, arguments, matrixMap); /* Call the function that breaks the input for the read_mat command */
            break;
    }
}

/* A function that breaks the input for the read_mat command */
void breakInputReadMat(char* stringArguments[], float arguments[], MatrixMap matrixMap[])
{
    /*  i, j - counters for several loops */
    int i = 0, j = 0;
    float numList[AMOUNT_OF_NUMBERS] = {0}; /* A number list of 0's for initializing the matrices */
    
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
    printf("NumList: \n");
    for(i = 0; i < AMOUNT_OF_NUMBERS; i++)
    {
        printf("%f\n", numList[i]);
    }
    print_mat(matrixMap[(int)arguments[0]].matrix); /* print the matrix */
}  