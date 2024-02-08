/* A calculator for matrices. */
/* The main function, all the intercation with the user and the command analysis. */
/* Input: <command> <arg1>, <arg2>, <arg3> (all the arguments are separated by commas and spaces) */

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

int getWords(char *input, char *command, int startingIndex, char stopChar); /* gets the command name from the input */
void handleReadMat(MatrixMap* matrixMap, char *input, int startingIndex); /* handles the read_mat command */
void handlePrintMat(MatrixMap* matrixMap, char *input, int startingIndex); /* handles the print_mat command */
void handleAddMat(MatrixMap* matrixMap, char *input, int startingIndex); /* handles the add_mat command */
void handleSubMat(MatrixMap* matrixMap, char *input, int startingIndex); /* handles the sub_mat command */
void handleMulMat(MatrixMap* matrixMap, char *input, int startingIndex); /* handles the mul_mat command */
void handleMulScalar(MatrixMap* matrixMap, char *input, int startingIndex); /* handles the mul_scalar command */
void handleTransMat(MatrixMap* matrixMap, char *input, int startingIndex); /* handles the trans_mat command*/
void clearBuffer(); /* clears the buffer */
void getLine(char *str); /* gets a line from the input */

#define SIZE_OF_MATRIX_NAME 6 /* the size of the matrix name (mat_A-F) */
#define AMOUNT_OF_NUMBERS 16 /* the amount of numbers in a matrix - for read_mat */
#define FALSE 0 /* a flag that indicates false */
#define TRUE 1 /* a flag that indicates true */
#define MAX_COMMAND_LENGTH 11 /* the maximum length of a command */
#define NO_STOP_CHAR 'N' /* a flag that indicates that there is no stop character */
#define MAX_FLOAT_LENGTH 24 /* the maximum length of a float including the decimal point */


/* The main function - initializes all the matrices to zero and gets the command from the user */
int main()
{
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F; /* Defining all the matrices */
    char input[] = "", command[MAX_COMMAND_LENGTH] = ""; /* The input from the user and the command */
    int startingIndex = 0; /* An index that indicates the starting index of the command */
    float numList[AMOUNT_OF_NUMBERS] = {0}; /* A number list of 0's for initializing the matrices */
    MatrixMap matrixMap[6]; /* A lookup table for the matrices */

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

    /* Get the command from the user and call the right function while the command is not "stop" */
    do {
        getLine(input); /* get the entire input line */
        startingIndex = getWords(input, command, 0, NO_STOP_CHAR); /* get the command name */

        if(strcmp(command, "read_mat") == 0) /* if the command is read_mat */
        {
            handleReadMat(matrixMap, input, startingIndex); /* call the handleReadMat function with an index that indicates the end of the command name */
        }
        else if(strcmp(command, "print_mat") == 0) /* if the command is print_mat */
        {
            handlePrintMat(matrixMap, input, startingIndex); /* call the handlePrintMat function with an index that indicates the end of the command name */
        }
        else if(strcmp(command, "add_mat") == 0) /* if the command is add_mat */
        {
            handleAddMat(matrixMap, input, startingIndex); /* call the handleAddMat function with an index that indicates the end of the command name */
        }
        else if(strcmp(command, "sub_mat") == 0) /* if the command is sub_mat */
        {
            handleSubMat(matrixMap, input, startingIndex); /* call the handleSubMat function with an index that indicates the end of the command name */
        }
        else if(strcmp(command, "mul_mat") == 0) /* if the command is mul_mat */
        {
            handleMulMat(matrixMap, input, startingIndex); /* call the handleMulMat function with an index that indicates the end of the command name */
        }
        else if(strcmp(command, "mul_scalar") == 0) /* if the command is mul_scalar */
        {
            handleMulScalar(matrixMap, input, startingIndex); /* call the handleMulScalar function with an index that indicates the end of the command name */
        }
        else if(strcmp(command, "trans_mat") == 0) /* if the command is trans_mat */
        {
            handleTransMat(matrixMap, input, startingIndex); /* call the handleTransMat function with an index that indicates the end of the command name */
        }
        else if(strcmp(command, "stop") == 0) /* if the command is stop */
        {
            return 0; /* exit the program */
        }
        else /* if the command is not recognized */
        {
            printf("Undefined command name\n"); /* print an error message */
            clearBuffer(); /* clear the buffer */
        }

    } while(TRUE);

    return 0;
}

/* A function that gets something from the input from a starting index to a stop character and saves it in a string */
int getWords(char *input, char *command, int startingIndex, char stopChar)
{
    int i = 0; /* an index for the command */

    /* run on all the characters of the input */
    while(startingIndex < strlen(input) && (input[startingIndex] == ' ' || input[startingIndex] == '\t')) /* skip spaces and tabs */
    {
        startingIndex++;
    }

    /* run on all the characters of the input and save the command name */
    for(i = startingIndex; i < strlen(input); i++) /* run on all the characters of the input */
    {
        if(stopChar != NO_STOP_CHAR) /* if there is a stop character */
        {
            if(input[i] == stopChar) /* if the character is the stop character */
            {
                break; /* break the loop */
            }
        }
        else /* if there is no stop character */
        {
            if(input[i] == ' ' || input[i] == '\t' || input[i] == '\0' || input[i] == '\n') /* if the character is a space, a tab, the end of the string or a new line */
            {
                break; /* break the loop */
            }
        }
        command[i-startingIndex] = input[i]; /* save the character in the command string */
    }
    command[i-startingIndex] = '\0'; /* add the null terminator to the command string */

    return i; /* return the index of the stop character */
}

/* A function that handles the read_mat command */
void handleReadMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    /*
    Declare all the needed variables:
    numList is an array of 16 numbers - the numbers of the matrix
    matrixName is the name of the matrix
    number is a string that represents a number of the matrix
    matrix is the pointer to the matrix
    i is a counter
    */
    float numList[AMOUNT_OF_NUMBERS] = {0};
    char matrixName[SIZE_OF_MATRIX_NAME] = "", number[MAX_FLOAT_LENGTH] = "";
    mat *matrix = NULL;
    int i;

    startingIndex = getWords(input, matrixName, startingIndex, ','); /* get the matrix name - after the matrix name there is a comma */
    for(i = 0; i < strlen(input) - startingIndex - 1; i++) /* run on all the numbers but the last one */
    {
        startingIndex = getWords(input, number, startingIndex+1, ','); /* get the number - after the number there is a comma */
        numList[i] = atof(number); /* transform the number from a string to a float */
    }
    startingIndex = getWords(input, number, startingIndex+1, NO_STOP_CHAR); /* get the last number - after the last number there is no comma */
    numList[i] = atof(number); /* transform the number from a string to a float */

    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++) /* find the matrix in the lookup table */
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matrix = matrixMap[i].matrix; /* save the pointer to the matrix */
            break; /* break the loop */
        }
    }

    if(matrix == NULL) /* if the matrix was not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* return from the function */
    }

    /* call read_mat(mat*, float[16]) */
    read_mat(matrix, numList);
}

/* A function that handles the print_mat command */
void handlePrintMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    /* Initialize all needed variables, matrixName is the name of the matrix, matrix is the pointer to the matrix, i is a counter */
    char matrixName[SIZE_OF_MATRIX_NAME];
    mat *matrix = NULL;
    int i;

    /* get the matrix name - after the matrix name there is no stop character */
    getWords(input, matrixName, startingIndex, NO_STOP_CHAR);

    /* find the matrix in the lookup table */
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matrix = matrixMap[i].matrix; /* save the pointer to the matrix */
            break; /* break the loop */
        }
    }

    if(matrix == NULL) /* if the matrix was not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* return from the function */
    }

    /* call print_mat(mat*) */
    print_mat(matrix);
}

/* A function that handles the add_mat command */
void handleAddMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    /*
    Initialize all needed variables:
    firstMatrix is the name of the first matrix
    secondMatrix is the name of the second matrix
    resMatrix is the name of the result matrix
    firstMatPointer is the pointer to the first matrix
    secondMatPointer is the pointer to the second matrix
    resMatPointer is the pointer to the result matrix
    i is a counter
    */
    char firstMatrix[SIZE_OF_MATRIX_NAME] = "", secondMatrix[SIZE_OF_MATRIX_NAME] = "", resMatrix[SIZE_OF_MATRIX_NAME] = "";
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;
    int i = 0;

    startingIndex = getWords(input, firstMatrix, startingIndex, ','); /* get the first matrix name - after the first matrix name there is a comma */
    startingIndex = getWords(input, secondMatrix, startingIndex + 1, ','); /* get the second matrix name - after the second matrix name there is a comma */
    startingIndex = getWords(input, resMatrix, startingIndex + 1, NO_STOP_CHAR); /* get the result matrix name - after the result matrix name there is no stop character */

    /* find all the matrices in the lookup table */
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(firstMatrix, matrixMap[i].matrixName) == 0) /* if the first matrix has been found */
        {
            firstMatPointer = matrixMap[i].matrix; /* save the pointer to the first matrix */
        }
        if(strcmp(secondMatrix, matrixMap[i].matrixName) == 0) /* if the second matrix has been found */
        {
            secondMatPointer = matrixMap[i].matrix; /* save the pointer to the second matrix */
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0) /* if the result matrix has been found */
        {
            resMatPointer = matrixMap[i].matrix; /* save the pointer to the result matrix */
        }
    }

    if(firstMatPointer == NULL || secondMatPointer == NULL || resMatPointer == NULL) /* if one of the matrices was not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* return from the function */
    }

    /* call add_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table */
    add_mat(firstMatPointer, secondMatPointer, resMatPointer);
}

/* A function that handles the sub_mat command */
void handleSubMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    /*
    Initialize all needed variables:
    firstMatrix is the name of the first matrix
    secondMatrix is the name of the second matrix
    resMatrix is the name of the result matrix
    firstMatPointer is the pointer to the first matrix
    secondMatPointer is the pointer to the second matrix
    resMatPointer is the pointer to the result matrix
    i is a counter
    */
    char firstMatrix[SIZE_OF_MATRIX_NAME] = "", secondMatrix[SIZE_OF_MATRIX_NAME] = "", resMatrix[SIZE_OF_MATRIX_NAME] = "";
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;
    int i = 0;

    startingIndex = getWords(input, firstMatrix, startingIndex, ','); /* get the first matrix name - after the first matrix name there is a comma */
    startingIndex = getWords(input, secondMatrix, startingIndex+1, ','); /* get the second matrix name - after the second matrix name there is a comma */
    startingIndex = getWords(input, resMatrix, startingIndex+1, NO_STOP_CHAR); /* get the result matrix name - after the result matrix name there is no stop character */

    /* find all the matrices in the lookup table */
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(firstMatrix, matrixMap[i].matrixName) == 0) /* if the first matrix has been found */
        {
            firstMatPointer = matrixMap[i].matrix; /* save the pointer to the first matrix */
        }
        if(strcmp(secondMatrix, matrixMap[i].matrixName) == 0) /* if the second matrix has been found */
        {
            secondMatPointer = matrixMap[i].matrix; /* save the pointer to the second matrix */
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0) /* if the result matrix has been found */
        {
            resMatPointer = matrixMap[i].matrix; /* save the pointer to the result matrix */
        }
    }

    if(firstMatPointer == NULL || secondMatPointer == NULL || resMatPointer == NULL) /* if one of the matrices was not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* return from the function */
    }

    /* call sub_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table */
    sub_mat(firstMatPointer, secondMatPointer, resMatPointer);
}

/* A function that handles the mul_mat command */
void handleMulMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    /*
    Initialize all needed variables:
    firstMatrix is the name of the first matrix
    secondMatrix is the name of the second matrix
    resMatrix is the name of the result matrix
    firstMatPointer is the pointer to the first matrix
    secondMatPointer is the pointer to the second matrix
    resMatPointer is the pointer to the result matrix
    i is a counter
    */
    char firstMatrix[SIZE_OF_MATRIX_NAME] = "", secondMatrix[SIZE_OF_MATRIX_NAME] = "", resMatrix[SIZE_OF_MATRIX_NAME] = "";
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;
    int i = 0;

    startingIndex = getWords(input, firstMatrix, startingIndex, ','); /* get the first matrix name - after the first matrix name there is a comma */
    startingIndex = getWords(input, secondMatrix, startingIndex + 1, ','); /* get the second matrix name - after the second matrix name there is a comma */
    startingIndex = getWords(input, resMatrix, startingIndex + 1, NO_STOP_CHAR); /* get the result matrix name - after the result matrix name there is no stop character */

    /* find all the matrices in the lookup table */
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(firstMatrix, matrixMap[i].matrixName) == 0) /* if the first matrix has been found */
        {
            firstMatPointer = matrixMap[i].matrix; /* save the pointer to the first matrix */
        }
        if(strcmp(secondMatrix, matrixMap[i].matrixName) == 0) /* if the second matrix has been found */
        {
            secondMatPointer = matrixMap[i].matrix; /* save the pointer to the second matrix */
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0) /* if the result matrix has been found */
        {
            resMatPointer = matrixMap[i].matrix; /* save the pointer to the result matrix */
        }
    }

    if(firstMatPointer == NULL || secondMatPointer == NULL || resMatPointer == NULL) /* if one of the matrices was not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* return from the function */
    }

    /* call mul_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table */
    mul_mat(firstMatPointer, secondMatPointer, resMatPointer);
}

/* A function that handles the mul_scalar command */
void handleMulScalar(MatrixMap* matrixMap, char *input, int startingIndex)
{
    /*
    Initialize all needed variables:
    matrixName is the name of the matrix
    resMatrix is the name of the result matrix
    scalarString is a string that represents a scalar
    matPointer is the pointer to the matrix
    resMatPointer is the pointer to the result matrix
    scalar is a float that represents the scalar
    i is a counter
    */
    char matrixName[SIZE_OF_MATRIX_NAME] = "", resMatrix[SIZE_OF_MATRIX_NAME] = "", scalarString[MAX_FLOAT_LENGTH] = "";
    mat *matPointer = NULL, *resMatPointer = NULL;
    float scalar;
    int i;

    startingIndex = getWords(input, matrixName, startingIndex, ','); /* get the matrix name - after the matrix name there is a comma */
    startingIndex = getWords(input, scalarString, startingIndex + 1, ','); /* get the scalar - after the scalar there is a comma */
    scalar = atof(scalarString); /* transform the scalar from a string to a float */
    startingIndex = getWords(input, resMatrix, startingIndex + 1, NO_STOP_CHAR); /* get the result matrix name - after the result matrix name there is no stop character */

    /* find all the matrices in the lookup table */
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        { 
            matPointer = matrixMap[i].matrix; /* save the pointer to the matrix */
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0) /* if the result matrix name is found in the lookup table */
        {
            resMatPointer = matrixMap[i].matrix; /* save the pointer to the result matrix */
        }
    }
    
    if(matPointer == NULL || resMatPointer == NULL) /* if the matrix was not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* return from the function */
    }

    mul_scalar(matPointer, scalar, resMatPointer); /* call mul_scalar(mat*, float, mat*) */
}

/* A function that handles the trans_mat command */
void handleTransMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    /*
    Initialize all needed variables:
    matrixName is the name of the matrix
    resMatrix is the name of the result matrix
    matPointer is the pointer to the matrix
    resMatPointer is the pointer to the result matrix
    tempMatrix is a temporary matrix
    numbers is an array of 16 numbers - the numbers of the matrix
    i is a counter
    */
    char matrixName[SIZE_OF_MATRIX_NAME], resMatrix[SIZE_OF_MATRIX_NAME];
    mat *matPointer = NULL, *resMatPointer = NULL, tempMatrix;
    float numbers[AMOUNT_OF_NUMBERS] = {0};
    int i;

    read_mat(&tempMatrix, numbers); /* initialize the temporary matrix to zero */

    startingIndex = getWords(input, matrixName, startingIndex, ','); /* get the matrix name - after the matrix name there is a comma */
    startingIndex = getWords(input, resMatrix, startingIndex + 1, NO_STOP_CHAR); /* get the result matrix name - after the result matrix name there is no stop character */

    /* find all the matrices in the lookup table */
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0) /* if the matrix name is found in the lookup table */
        {
            matPointer = matrixMap[i].matrix; /* save the pointer to the matrix */
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0) /* if the result matrix name is found in the lookup table */
        {
            resMatPointer = matrixMap[i].matrix; /* save the pointer to the result matrix */
        }
    }
    
    if(matPointer == NULL || resMatPointer == NULL) /* if the matrix was not found in the lookup table */
    {
        printf("Undefined matrix name\n"); /* print an error message */
        return; /* return from the function */
    }

    trans_mat(matPointer, resMatPointer, &tempMatrix); /* call trans_mat(mat*, mat*, mat*) */
}

/* A function that clears the buffer */
void clearBuffer()
{
    while(getchar() != '\n'); /* clear the buffer */
}

/* A function that gets a line from the input */
void getLine(char *str)
{
    while((*str = getchar()) != EOF && *str != '\n')
    {
        str++;
    }
}

/* gcc -Wall -pedantic -ansi mainmat.c mymat.c -o mainmat */