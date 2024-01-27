// All arithmetic functions for the matices.

#include <stdio.h>
#include "mymat.h"

#define LENGTH_OF_MATRIX_LINE 4

// read_mat - gets an array of 16 floats and a matrix, and puts the floats in the matrix
void read_mat(mat* matrix, float list[16])
{   
    int i, j;
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE ; j++)
        {
            matrix->data[i][j] = list[i * LENGTH_OF_MATRIX_LINE + j];
        }
    }
}

// print_mat - prints a given matrix
void print_mat(mat* matrix)
{
    int i, j;
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE ; j++)
        {
            printf("%7.2f \t\t", matrix->data[i][j]);
        }
        printf("\n");
    }
}

// add_mat - gets two matrices and adds them
void add_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix)
{
    int i, j;

    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            resultMatrix->data[i][j] = firstMatrix->data[i][j] + secondMatrix->data[i][j];
        }
    }
}

// sub_mat - gets two matrices and substracts them from each other
void sub_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix)
{
    int i, j;

    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            resultMatrix->data[i][j] = firstMatrix->data[i][j] - secondMatrix->data[i][j];
        }
    }
}

// mul_mat - multiplys two matrices
void mul_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix)
{
    int i, j;

    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            resultMatrix->data[i][j] = firstMatrix->data[i][j] * secondMatrix->data[j][i];
        }
    }
}

// mul_scalar - multiplys a matrix by a scalar
void mul_scalar(mat* matrix, float scalar, mat* resultMatrix)
{
    int i, j;

    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            resultMatrix->data[i][j] = matrix->data[i][j] * scalar;
        }
    }
}

// trans_mat - transpose a matrix
void trans_mat(mat* matrix, mat* resultMatrix, mat* tempMatrix)
{
    int i, j;

    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            tempMatrix->data[j][i] = matrix->data[i][j];
        }
    }

    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            resultMatrix->data[i][j] = tempMatrix->data[i][j];
        }
    }
}