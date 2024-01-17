// A calculator for matrices.
// The main function, all the intercation with the user and the command analysis.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymat.h"

typedef struct {
    char* matrixName;
    mat* matrix;
} MatrixMap;

void handleReadMat(MatrixMap* matrixMap, int size);

int main()
{
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;
    int i;

    MatrixMap matrixMap[] ={
        {"MAT_A", &MAT_A},
        {"MAT_B", &MAT_B},
        {"MAT_C", &MAT_C},
        {"MAT_D", &MAT_D},
        {"MAT_E", &MAT_E},
        {"MAT_F", &MAT_F}
    };

    // Intialize all matrices to zero using read_mat(mat*, float[16])
    float numList[16] = {0};
    read_mat(&MAT_A, numList);
    read_mat(&MAT_B, numList);
    read_mat(&MAT_C, numList);
    read_mat(&MAT_D, numList);
    read_mat(&MAT_E, numList);
    read_mat(&MAT_F, numList);

    // Get the command from the user
    do {
        char command[11];
        scanf("%s", command);

        if(strcmp(command, "read_mat") == 0)
        {
            handleReadMat(matrixMap, 6);
        }
        else if(strcmp(command, "print_mat") == 0)
        {
            // print_mat <matrix>
            // Initialize all needed variables, matrixName is the name of the matrix, matrix is the pointer to the matrix
            char matrixName[6];
            mat *matrix = NULL;

            // get the matrix (every matrix name is mat_{A-F})
            scanf("%s", matrixName);

           // find the matrix in the lookup table
            for(i = 0; i < 6; i++)
            {
                if(strcmp(matrixName, matrixMap[i].matrixName) == 0)
                {
                    matrix = matrixMap[i].matrix;
                }
            }

            // call print_mat(mat*) using the lookup table
            // if the matrix is not found, print "Undefined matrix name"
            if(matrix != NULL)
            {
                print_mat(matrix);
            }
            else
            {
                printf("Undefined matrix name\n");
            }
        }
        else if(strcmp(command, "add_mat") == 0)
        {
            // add_mat <matrix> <matrix> <result matrix>
            /*
            Initialize all needed variables:
            firstMatrix is the name of the first matrix
            secondMatrix is the name of the second matrix
            resMatrix is the name of the result matrix
            firstMatPointer is the pointer to the first matrix
            secondMatPointer is the pointer to the second matrix
            resMatPointer is the pointer to the result matrix
            */
            char firstMatrix[6], secondMatrix[6], resMatrix[6];
            mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;

            // get the matrices (every matrix name is mat_{A-F})
            scanf("%s %s %s", firstMatrix, secondMatrix, resMatrix);

            // find all the matrices in the lookup table
            for(i = 0; i < 6; i++)
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

            // call add_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table
            if(firstMatPointer != NULL && secondMatPointer != NULL && resMatPointer != NULL)
            {
                add_mat(firstMatPointer, secondMatPointer, resMatPointer);
            }
            else
            {
                printf("Undefined matrix name\n");
            }
        }
        else if(strcmp(command, "sub_mat") == 0)
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
            char firstMatrix[6], secondMatrix[6], resMatrix[6];
            mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;

            // get the matrices names (every matrix name is mat_{A-F})
            scanf("%s %s %s", firstMatrix, secondMatrix, resMatrix);

            // find all the matrices in the lookup table
            for(i = 0; i < 6; i++)
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

            // call sub_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table
            if(firstMatPointer != NULL && secondMatPointer != NULL && resMatPointer != NULL)
            {
                sub_mat(firstMatPointer, secondMatPointer, resMatPointer);
            }
            else
            {
                printf("Undefined matrix name\n");
            }
        }
        else if(strcmp(command, "mul_mat") == 0)
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
            char firstMatrix[6], secondMatrix[6], resMatrix[6];
            mat *firstMatPointer = NULL, *secondMatPointer = NULL, *resMatPointer = NULL;

            // get the matrices names (every matrix name is mat_{A-F})
            scanf("%s %s %s", firstMatrix, secondMatrix, resMatrix);

            // find all the matrices in the lookup table
            for(i = 0; i < 6; i++)
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

            // call mul_mat(mat*, mat*, mat*) if all the matrices are found in the lookup table
            if(firstMatPointer != NULL && secondMatPointer != NULL && resMatPointer != NULL)
            {
                mul_mat(firstMatPointer, secondMatPointer, resMatPointer);
            }
            else
            {
                printf("Undefined matrix name\n");
            }
        }
        else if(strcmp(command, "mul_scalar") == 0)
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
            char matrixName[6], resMatrix[6];
            float scalar;
            int isScalarValid = 1;
            mat *matPointer = NULL, *resMatPointer = NULL;

            // get the matrix name
            scanf("%s ", matrixName);

            // make sure the scalar is a real number
            if(scanf("%f", &scalar) != 1)
            {
                printf("Argument is not a real number\n");
                isScalarValid = 0;
                continue;
            }

            if(isScalarValid)
            {
                // get the result matrix name
                scanf("%s", resMatrix);

                // find all the matrices in the lookup table
                for(i = 0; i < 6; i++)
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

                // call mul_scalar(mat*, float, mat*) if all the matrices are found in the lookup table
                if(matPointer != NULL && resMatPointer != NULL)
                {
                    mul_scalar(matPointer, scalar, resMatPointer);
                }
                else
                {
                    printf("Undefined matrix name\n");
                }
            }
        }
        else if(strcmp(command, "trans_mat") == 0)
        {
            // trans_mat <matrix> <result matrix>
            /*
            Initialize all needed variables:
            matrixName is the name of the matrix
            resMatrix is the name of the result matrix
            matPointer is the pointer to the matrix
            resMatPointer is the pointer to the result matrix
            */
            char matrixName[6], resMatrix[6];
            mat *matPointer = NULL, *resMatPointer = NULL;

            // get the matrices names (every matrix name is mat_{A-F})
            scanf("%s %s", matrixName, resMatrix);

            // find all the matrices in the lookup table
            for(i = 0; i < 6; i++)
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

            if(matPointer != NULL && resMatPointer != NULL)
            {
                trans_mat(matPointer, resMatPointer);
            }
            else
            {
                printf("Undefined matrix name\n");
            }
        }
        else if(strcmp(command, "stop") == 0)
        {
            // stop
            return 0;
        }
        else
        {
            printf("Undefined command name\n");
        }

    } while(1);

    return 0;
}

void handleReadMat(MatrixMap* matrixMap, int size)
{
    // read_mat <matrix> <16 numbers>
    /*
    Initialize all needed variables:
    matrixName is the name of the matrix
    matrix is the pointer to the matrix
    numList is the list of numbers
    allNumbersAreValid is a flag that indicates if all the numbers are valid
    */
    float numList[16] = {0};
    char matrixName[6];
    mat *matrix = NULL;
    int i;

    // get the matrix name
    scanf("%s", matrixName);

    // Find the matrix in the lookup table
    for(i = 0; i < size; i++)
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

        // skip the rest of the line
        char c;
        while((c = getchar()) != '\n' && c != EOF);
        
        return;
    }

    // get all the numbers
    for(i = 0; i < 16; i++)
    {
        if(scanf("%f", &numList[i]) != 1)
        {
            printf("Argument is not a real number\n");
            return;
        }
    }

    // call read_mat(mat*, float[16])
    read_mat(matrix, numList);
}


//gcc mainmat.c mymat.c -o mainmat