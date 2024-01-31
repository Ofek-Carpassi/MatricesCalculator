// A calculator for matrices.
// The main function, all the intercation with the user and the command analysis.
// Input: <command> <arg1>, <arg2>, <arg3> (all the arguments are separated by commas and spaces)

/*
Importing all the needed libraries:
stdio.h - for input and output
string.h - for string manipulation
ctype.h - for character manipulation
mymat.h - for the matrix functions (mymat.h is a header file that I created)
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mymat.h"

// A struct that maps between a matrix name and a pointer to the matrix
typedef struct {
    char* matrixName;
    mat* matrix;
} MatrixMap;

/*
All handling commands functions - all the functions get a pointer to the lookup table:
handleReadMat - handles the read_mat command
handlePrintMat - handles the print_mat command
handleAddMat - handles the add_mat command
handleSubMat - handles the sub_mat command
handleMulMat - handles the mul_mat command
handleMulScalar - handles the mul_scalar command
handleTransMat - handles the trans_mat command
*/
void handleReadMat(MatrixMap* matrixMap);
void handlePrintMat(MatrixMap* matrixMap);
void handleAddMat(MatrixMap* matrixMap);
void handleSubMat(MatrixMap* matrixMap);
void handleMulMat(MatrixMap* matrixMap);
void handleMulScalar(MatrixMap* matrixMap);
void handleTransMat(MatrixMap* matrixMap);
void updateAllMatrices(char *firstMatrix, char *secondMatrix, char *resMatrix);

// The size of the matrix name (mat_{A-F})
#define SIZE_OF_MATRIX_NAME 6
#define AMOUNT_OF_NUMBERS 16
#define FALSE 0 
#define TRUE 1

// The main function - initializes all the matrices to zero and gets the command from the user
int main()
{
    // Create all the matrices using the "mat" struct created in mymat.h
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;

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
        char command[9];
        scanf(" %s", command);

        if(strcmp(command, "read_mat") == 0)
        {
            handleReadMat(matrixMap);
        }
        else if(strcmp(command, "print_mat") == 0)
        {
            handlePrintMat(matrixMap);
        }
        else if(strcmp(command, "add_mat") == 0)
        {
            handleAddMat(matrixMap);
        }
        else if(strcmp(command, "sub_mat") == 0)
        {
            handleSubMat(matrixMap);
        }
        else if(strcmp(command, "mul_mat") == 0)
        {
            handleMulMat(matrixMap);
        }
        else if(strcmp(command, "mul_scalar") == 0)
        {
            handleMulScalar(matrixMap);
        }
        else if(strcmp(command, "trans_mat") == 0)
        {
            handleTransMat(matrixMap);
        }
        else if(strcmp(command, "stop") == 0)
        {
            // stop
            return 0;
        }
        else
        {
            printf("Undefined command name\n");
            // clear the buffer
            char c;
            while((c = getchar()) != '\n' && c != EOF);
        }

    } while(1);

    return 0;
}

void handleReadMat(MatrixMap* matrixMap)
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
    char matrixName[SIZE_OF_MATRIX_NAME] = "";
    mat *matrix = NULL;
    int i;
    char c;

    // get the matrix (every matrix name is mat_{A-F})
    scanf(" %[^,]", matrixName);
    //printf("\n%s\n", matrixName);

    // Find the matrix in the lookup table
    for(i = 0; i < SIZE_OF_MATRIX_NAME; i++)
    {
        if(strcmp(matrixName, matrixMap[i].matrixName) == 0)
        {
            matrix = matrixMap[i].matrix;
            break;
        }
    }
    
    // If the matrix was not found, print an error and return
    if(matrix == NULL)
    {
        printf("Undefined matrix name\n");

        while((c = getchar()) != '\n' && c != EOF);
        
        return;
    }

    // get all the numbers
    for(i = 0; i < AMOUNT_OF_NUMBERS; i++)
    {
        scanf(", %f", &numList[i]);
        
        if(numList[i] == '\n' && numList[i] != 10)
        {
            break;
        }
        
        // make sure the number is a real number
        if(isdigit(numList[i]) == 1)
        {
            printf("Argument is not a real number\n");
        
            while((c = getchar()) != '\n' && c != EOF);

            return;
        }
    }

    // call read_mat(mat*, float[16])
    read_mat(matrix, numList);
}

void handlePrintMat(MatrixMap* matrixMap)
{
    // print_mat <matrix>
    // Initialize all needed variables, matrixName is the name of the matrix, matrix is the pointer to the matrix
    char matrixName[SIZE_OF_MATRIX_NAME];
    mat *matrix = NULL;

    // get the matrix (every matrix name is mat_{A-F})
    scanf("%s", matrixName);

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

void handleAddMat(MatrixMap* matrixMap)
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
    char firstMatrix[SIZE_OF_MATRIX_NAME], secondMatrix[SIZE_OF_MATRIX_NAME], resMatrix[SIZE_OF_MATRIX_NAME], matrices[SIZE_OF_MATRIX_NAME*3 + 2];
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;
    int i, j;

    // get the matrices into matrices (every matrix name is mat_{A-F})
    updateAllMatrices(firstMatrix, secondMatrix, resMatrix);

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

void handleSubMat(MatrixMap* matrixMap)
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
    char firstMatrix[SIZE_OF_MATRIX_NAME], secondMatrix[SIZE_OF_MATRIX_NAME], resMatrix[SIZE_OF_MATRIX_NAME], matrices[SIZE_OF_MATRIX_NAME*3 + 2];
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;
    int i, j;

    // get the matrices into matrices (every matrix name is mat_{A-F})
    updateAllMatrices(firstMatrix, secondMatrix, resMatrix);

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

void handleMulMat(MatrixMap* matrixMap)
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
    char firstMatrix[SIZE_OF_MATRIX_NAME], secondMatrix[SIZE_OF_MATRIX_NAME], resMatrix[SIZE_OF_MATRIX_NAME];
    mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;

    // get the matrices into matrices (every matrix name is mat_{A-F})
    updateAllMatrices(firstMatrix, secondMatrix, resMatrix);

    // find all the matrices in the lookup table
    int i;
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

void handleMulScalar(MatrixMap* matrixMap)
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
    char matrixName[SIZE_OF_MATRIX_NAME], resMatrix[SIZE_OF_MATRIX_NAME];
    float scalar;
    mat *matPointer = NULL, *resMatPointer = NULL;

    // get the matrix name
    scanf(" %[^,]", matrixName);

    // make sure the scalar is a real number
    if(scanf(", %f", &scalar) != 1)
    {
        printf("Argument is not a real number\n");
        // clear the buffer
        char c;
        while((c = getchar()) != '\n' && c != EOF);

        return;
    }

    // get the result matrix name
    scanf(", %s", resMatrix);

    // find all the matrices in the lookup table
    int i;
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

void handleTransMat(MatrixMap* matrixMap)
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

    // get the matrix name
    scanf(" %[^,], %s", matrixName, resMatrix);

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

void updateAllMatrices(char *firstMatrix, char *secondMatrix, char *resMatrix)
{
    char matrices[SIZE_OF_MATRIX_NAME*5 + 2];
    int i, j;
    int placementOfFirstComma = 5, placementOfSecondComma = 0, placementOfCommandEnd = 0, startOfThirdMatrixName = 0;
    int hasSeenFirstComma = FALSE, hasSeenSecondComma = FALSE, isInThirdMatrixName = FALSE;

    // get the matrices into matrices (every matrix name is mat_{A-F})
    scanf("%[^\n]", matrices);

    for(i = 0; i < strlen(matrices) && matrices[i] != '\0'; i++)
    {
        if(hasSeenFirstComma == TRUE && hasSeenSecondComma == TRUE && isInThirdMatrixName != TRUE && matrices[i] == 'M')
        {
            isInThirdMatrixName = TRUE;
            startOfThirdMatrixName = i;
        }

        if((matrices[i] == ' ' || matrices[i] == '\t') && hasSeenFirstComma == FALSE)
        {
            placementOfFirstComma += 1;
        }
        else if((matrices[i] == ' ' || matrices[i] == '\t') && hasSeenFirstComma == TRUE && hasSeenSecondComma == FALSE)
        {
            placementOfSecondComma += 1;
        }
        else if((matrices[i] == ' ' || matrices[i] == '\t') && hasSeenFirstComma == TRUE && hasSeenSecondComma == TRUE && isInThirdMatrixName == FALSE)
        {
            placementOfCommandEnd += 1;
        }

        if(matrices[i] == ',')
        {
            if(matrices[i-1] == ',' || matrices[i+1] == ',')
            {
                printf("Multiple consecutive commas\n");

                while((matrices[i] = getchar()) != '\n' && matrices[i] != EOF);

                return;
            }

            if(i == placementOfFirstComma)
            {
                for(j = 0; j < SIZE_OF_MATRIX_NAME-1; j++)
                {
                    *(firstMatrix+j) = matrices[j+placementOfFirstComma-5];
                }
                *(firstMatrix+SIZE_OF_MATRIX_NAME-1) = '\0';
                hasSeenFirstComma = TRUE;
                placementOfSecondComma = placementOfFirstComma+6;
            }
            else if(i == placementOfSecondComma)
            {
                for(j = 0; j < SIZE_OF_MATRIX_NAME-1; j++)
                {
                    *(secondMatrix+j) = matrices[j+placementOfSecondComma-5];
                }
                *(secondMatrix+SIZE_OF_MATRIX_NAME-1) = '\0';
                hasSeenSecondComma = TRUE;
                placementOfCommandEnd = placementOfSecondComma+6;
            }
            else
            {
                printf("First comma: %d, Second comma: %d", placementOfFirstComma, placementOfSecondComma);
                printf(", i: %d\n", i);
                printf("Illegal comma\n");

                while((matrices[i] = getchar()) != '\n' && matrices[i] != EOF);

                return;
            }
        }
    }

    for(j = 0; j < placementOfCommandEnd - startOfThirdMatrixName + 1; j++ && isInThirdMatrixName)
    {
        *(resMatrix+j) = matrices[j+startOfThirdMatrixName];
    }
    *(resMatrix+SIZE_OF_MATRIX_NAME-1) = '\0';


    if(matrices[placementOfFirstComma] != ',' || matrices[placementOfSecondComma] != ',')
    {
        printf("Missing comma\n");

        while((matrices[i] = getchar()) != '\n' && matrices[i] != EOF);

        return;
    }
}

//gcc mainmat.c mymat.c -o mainmat