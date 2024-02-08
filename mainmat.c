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
void cleanBuffer(); /* clears the buffer */
void getLine(char *str); /* gets a line from the input */

#define SIZE_OF_MATRIX_NAME 6 /* the size of the matrix name (mat_A-F) */
#define AMOUNT_OF_NUMBERS 16 /* the amount of numbers in a matrix - for read_mat */
#define FALSE 0 /* a flag that indicates false */
#define TRUE 1 /* a flag that indicates true */

/* The main function - initializes all the matrices to zero and gets the command from the user */
int main()
{
    // Create all the matrices using the "mat" struct created in mymat.h
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;
    char input[] = "";
    char command[11] = "";
    int startingIndex = 0;

    // Create a lookup table for the matrices - using the MatrixMap struct
    MatrixMap matrixMap[] ={
        {"MAT_A", &MAT_A},
        {"MAT_B", &MAT_B},
        {"MAT_C", &MAT_C},
        {"MAT_D", &MAT_D},
        {"MAT_E", &MAT_E},
        {"MAT_F", &MAT_F}
    };

    // Intialize all matrices to zero using an array of 16 zeros and the read_mat function
    float numList[AMOUNT_OF_NUMBERS] = {0};
    read_mat(&MAT_A, numList);
    read_mat(&MAT_B, numList);
    read_mat(&MAT_C, numList);
    read_mat(&MAT_D, numList);
    read_mat(&MAT_E, numList);
    read_mat(&MAT_F, numList);

    // Get the command from the user and call the right function while the command is not "stop"
    do {
        // A string that holds the command - 11 is the maximum length of a command ()
        getLine(input);
        startingIndex = getWords(input, command, 0, 'N');

        if(strcmp(command, "read_mat") == 0)
        {
            handleReadMat(matrixMap, input, startingIndex);
        }
        else if(strcmp(command, "print_mat") == 0)
        {
            handlePrintMat(matrixMap, input, startingIndex);
        }
        else if(strcmp(command, "add_mat") == 0)
        {
            handleAddMat(matrixMap, input, startingIndex);
        }
        else if(strcmp(command, "sub_mat") == 0)
        {
            handleSubMat(matrixMap, input, startingIndex);
        }
        else if(strcmp(command, "mul_mat") == 0)
        {
            handleMulMat(matrixMap, input, startingIndex);
        }
        else if(strcmp(command, "mul_scalar") == 0)
        {
            handleMulScalar(matrixMap, input, startingIndex);
        }
        else if(strcmp(command, "trans_mat") == 0)
        {
            handleTransMat(matrixMap, input, startingIndex);
        }
        else if(strcmp(command, "stop") == 0)
        {
            // stop
            return 0;
        }
        else
        {
            printf("Undefined command name\n");
            
            cleanBuffer();
        }

    } while(1);

    return 0;
}

int getWords(char *input, char *command, int startingIndex, char stopChar)
{
    int i = 0;

    // run on all the characters of the input
    while(startingIndex < strlen(input) && (input[startingIndex] == ' ' || input[startingIndex] == '\t'))
    {
        startingIndex++;
    }

    // run on all the characters of the input and save the command name
    for(i = startingIndex; i < strlen(input); i++)
    {
        if(stopChar != 'N')
        {
            if(input[i] == stopChar)
            {
                break;
            }
        }
        else
        {
            if(input[i] == ' ' || input[i] == '\t' || input[i] == '\0' || input[i] == '\n')
            {
                break;
            }
        }
        command[i-startingIndex] = input[i];
    }
    command[i-startingIndex] = '\0';

    return i;
}

/* A function that handles the read_mat command */
void handleReadMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    // read_mat <matrix> <16 numbers>
    /*
    Initialize all needed variables:
    matrixName is the name of the matrix
    matrix is the pointer to the matrix
    numList is the list of numbers
    allNumbersAreValid is a flag that indicates if all the numbers are valid
    */
    float numList[AMOUNT_OF_NUMBERS] = {0};
    char matrixName[SIZE_OF_MATRIX_NAME] = "", number[24] = "";
    mat *matrix = NULL;
    int i;
    char c;

    startingIndex = getWords(input, matrixName, startingIndex, ',');
    for(i = 0; i < AMOUNT_OF_NUMBERS-1; i++)
    {
        startingIndex = getWords(input, number, startingIndex+1, ',');
        numList[i] = atof(number);
    }
    startingIndex = getWords(input, number, startingIndex+1, 'N');
    numList[AMOUNT_OF_NUMBERS-1] = atof(number);

    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0)
        {
            matrix = matrixMap[i].matrix;
        }
    }

    if(matrix == NULL)
    {
        printf("Undefined matrix name\n");
        return;
    }

    // call read_mat(mat*, float[16])
    read_mat(matrix, numList);
}

/* A function that handles the print_mat command */
void handlePrintMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    // print_mat <matrix>
    // Initialize all needed variables, matrixName is the name of the matrix, matrix is the pointer to the matrix
    char matrixName[SIZE_OF_MATRIX_NAME];
    mat *matrix = NULL;

    // get the matrix (every matrix name is mat_{A-F})
    getWords(input, matrixName, startingIndex, 'N');

    // find the matrix in the lookup table
    int i;
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0)
        {
            matrix = matrixMap[i].matrix;
        }
    }

    if(matrix == NULL)
    {
        printf("Undefined matrix name\n");
        return;
    }

    // call print_mat(mat*)
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
    */
    char firstMatrix[SIZE_OF_MATRIX_NAME] = "", secondMatrix[SIZE_OF_MATRIX_NAME] = "", resMatrix[SIZE_OF_MATRIX_NAME] = "";
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;
    int i = 0, j = 0;

    startingIndex = getWords(input, firstMatrix, startingIndex, ',');
    startingIndex = getWords(input, secondMatrix, startingIndex+1, ',');
    startingIndex = getWords(input, resMatrix, startingIndex+1, 'N');

    // find all the matrices in the lookup table
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(firstMatrix, matrixMap[i].matrixName) == 0)
        {
            firstMatPointer = matrixMap[i].matrix;
        }
        if(strcmp(secondMatrix, matrixMap[i].matrixName) == 0)
        {
            secondMatPointer = matrixMap[i].matrix;
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0)
        {
            resMatPointer = matrixMap[i].matrix;
        }
    }

    if(firstMatPointer == NULL || secondMatPointer == NULL || resMatPointer == NULL)
    {
        printf("Undefined matrix name\n");
        return;
    }

    // call add_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table
    add_mat(firstMatPointer, secondMatPointer, resMatPointer);
}

/* A function that handles the sub_mat command */
void handleSubMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    // sub_mat <matrix> <matrix> <result matrix>
    /*
    Initialize all needed variables:
    firstMatrix is the name of the first matrix
    secondMatrix is the name of the second matrix
    resMatrix is the name of the result matrix
    firstMatPointer is the pointer to the first matrix
    secondMatPointer is the pointer to the second matrix
    resMatPointer is the pointer to the result matrix
    */
    char firstMatrix[SIZE_OF_MATRIX_NAME] = "", secondMatrix[SIZE_OF_MATRIX_NAME] = "", resMatrix[SIZE_OF_MATRIX_NAME] = "";
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;
    int i = 0, j = 0;

    startingIndex = getWords(input, firstMatrix, startingIndex, ',');
    startingIndex = getWords(input, secondMatrix, startingIndex+1, ',');
    startingIndex = getWords(input, resMatrix, startingIndex+1, 'N');

    // find all the matrices in the lookup table
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(firstMatrix, matrixMap[i].matrixName) == 0)
        {
            firstMatPointer = matrixMap[i].matrix;
        }
        if(strcmp(secondMatrix, matrixMap[i].matrixName) == 0)
        {
            secondMatPointer = matrixMap[i].matrix;
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0)
        {
            resMatPointer = matrixMap[i].matrix;
        }
    }

    if(firstMatPointer == NULL || secondMatPointer == NULL || resMatPointer == NULL)
    {
        printf("Undefined matrix name\n");
        return;
    }

    // call sub_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table
    sub_mat(firstMatPointer, secondMatPointer, resMatPointer);
}

/* A function that handles the mul_mat command */
void handleMulMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    // mul_mat <matrix> <matrix> <result matrix>
    /*
    Initialize all needed variables:
    firstMatrix is the name of the first matrix
    secondMatrix is the name of the second matrix
    resMatrix is the name of the result matrix
    firstMatPointer is the pointer to the first matrix
    secondMatPointer is the pointer to the second matrix
    resMatPointer is the pointer to the result matrix
    */
    char firstMatrix[SIZE_OF_MATRIX_NAME] = "", secondMatrix[SIZE_OF_MATRIX_NAME] = "", resMatrix[SIZE_OF_MATRIX_NAME] = "";
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;
    int i = 0, j = 0;

    startingIndex = getWords(input, firstMatrix, startingIndex, ',');
    startingIndex = getWords(input, secondMatrix, startingIndex+1, ',');
    startingIndex = getWords(input, resMatrix, startingIndex+1, 'N');

    // find all the matrices in the lookup table
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(firstMatrix, matrixMap[i].matrixName) == 0)
        {
            firstMatPointer = matrixMap[i].matrix;
        }
        if(strcmp(secondMatrix, matrixMap[i].matrixName) == 0)
        {
            secondMatPointer = matrixMap[i].matrix;
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0)
        {
            resMatPointer = matrixMap[i].matrix;
        }
    }

    if(firstMatPointer == NULL || secondMatPointer == NULL || resMatPointer == NULL)
    {
        printf("Undefined matrix name\n");
        return;
    }

    // call mul_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table
    mul_mat(firstMatPointer, secondMatPointer, resMatPointer);
}

/* A function that handles the mul_scalar command */
void handleMulScalar(MatrixMap* matrixMap, char *input, int startingIndex)
{
    // mul_scalar <matrix> <scalar> <result matrix>
    /*
    Initialize all needed variables:
    matrixName is the name of the matrix
    scalar is the scalar
    resMatrix is the name of the result matrix
    matPointer is the pointer to the matrix
    resMatPointer is the pointer to the result matrix
    */
    char matrixName[SIZE_OF_MATRIX_NAME] = "", resMatrix[SIZE_OF_MATRIX_NAME] = "", scalarString[24] = "";
    float scalar;
    mat *matPointer = NULL, *resMatPointer = NULL;
    int i;

    startingIndex = getWords(input, matrixName, startingIndex, ',');
    startingIndex = getWords(input, scalarString, startingIndex+1, ',');
    scalar = atof(scalarString);
    startingIndex = getWords(input, resMatrix, startingIndex+1, 'N');

    // find all the matrices in the lookup table
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0)
        {
            matPointer = matrixMap[i].matrix;
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0)
        {
            resMatPointer = matrixMap[i].matrix;
        }
    }
    
    if(matPointer == NULL || resMatPointer == NULL)
    {
        printf("Undefined matrix name\n");
        return;
    }

    mul_scalar(matPointer, scalar, resMatPointer);
}

/* A function that handles the trans_mat command */
void handleTransMat(MatrixMap* matrixMap, char *input, int startingIndex)
{
    // trans_mat <matrix> <result matrix>
    /*
    Initialize all needed variables:
    matrixName is the name of the matrix
    resMatrix is the name of the result matrix
    matPointer is the pointer to the matrix
    resMatPointer is the pointer to the result matrix
    */
    char matrixName[SIZE_OF_MATRIX_NAME], resMatrix[SIZE_OF_MATRIX_NAME];
    mat *matPointer = NULL, *resMatPointer = NULL, tempMatrix;
    int i;
    float numbers[AMOUNT_OF_NUMBERS] = {0};

    read_mat(&tempMatrix, numbers);

    startingIndex = getWords(input, matrixName, startingIndex, ',');
    startingIndex = getWords(input, resMatrix, startingIndex+1, 'N');

    // find all the matrices in the lookup table
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0)
        {
            matPointer = matrixMap[i].matrix;
        }
        if(strcmp(resMatrix, matrixMap[i].matrixName) == 0)
        {
            resMatPointer = matrixMap[i].matrix;
        }
    }
    
    if(matPointer == NULL || resMatPointer == NULL)
    {
        printf("Undefined matrix name\n");
        return;
    }

    trans_mat(matPointer, resMatPointer, &tempMatrix);
}

/* A function that clears the buffer */
void cleanBuffer()
{
    while(getchar() != '\n'); // clear the buffer
}

void getLine(char *str)
{
    while((*str = getchar()) != EOF && *str != '\n')
    {
        str++;
    }
}

//gcc mainmat.c mymat.c -o mainmat