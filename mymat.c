/* All arithmetic functions for the matices. */

#include <stdio.h>
#include "mymat.h"

#define LENGTH_OF_MATRIX_LINE 4 /* The length of the matrix line (4x4) */

/* read_mat - gets an array of 16 floats and a matrix, and puts the floats in the matrix */
void read_mat(mat* matrix, float list[16])
{   
    int i, j; /* i - the row index, j - the column index */
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE ; j++)
        {
            /* Save each float in the matrix */
            /* The matrix is a 4x4 matrix, so the index in the list is i * 4 + j */
            matrix->data[i][j] = list[i * LENGTH_OF_MATRIX_LINE + j];
        }
    }
}

/* print_mat - prints a given matrix */
void print_mat(mat* matrix)
{
    /* i - the row index, j - the column index */
    int i, j;
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE ; j++)
        {
            /* Print each float in the matrix - with 2 digits after the point */
            printf("%7.2f \t\t", matrix->data[i][j]);
        }
        /* Print a new line after each row */
        printf("\n");
    }
}

/* add_mat - gets two matrices and adds them */
void add_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix)
{
    /* i - the row index, j - the column index */
    int i, j;
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            /* Add each float in the matrices and save it in the result matrix */
            resultMatrix->data[i][j] = firstMatrix->data[i][j] + secondMatrix->data[i][j];
        }
    }
}

/* sub_mat - gets two matrices and substracts them from each other */
void sub_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix)
{
    /* i - the row index, j - the column index */
    int i, j;
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            /* Substract each float in the matrices and save it in the result matrix */
            resultMatrix->data[i][j] = firstMatrix->data[i][j] - secondMatrix->data[i][j];
        }
    }
}

/* mul_mat - multiplys two matrices */
void mul_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix)
{
    /* i - the row index, j - the column index */
    int i, j;
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            /* Multiply each float in the matrices and save it in the result matrix */
            resultMatrix->data[i][j] = firstMatrix->data[i][j] * secondMatrix->data[i][j];
        }
    }
}

/* mul_scalar - multiplys a matrix by a scalar */
void mul_scalar(mat* matrix, float scalar, mat* resultMatrix)
{
    /* i - the row index, j - the column index */
    int i, j;
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            /* Multiply each float in the matrix by the scalar and save it in the result matrix */
            resultMatrix->data[i][j] = matrix->data[i][j] * scalar;
        }
    }
}

/* trans_mat - transpose a matrix */
void trans_mat(mat* matrix, mat* resultMatrix, mat* tempMatrix)
{
    /* i - the row index, j - the column index */
    int i, j;
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            /* Transpose the matrix and save it in a temporary matrix (to avoid transposing the same value twice) */
            tempMatrix->data[j][i] = matrix->data[i][j];
        }
    }

    /* Copy the temporary matrix to the result matrix */
    for(i = 0; i < LENGTH_OF_MATRIX_LINE; i++)
    {
        for(j = 0; j < LENGTH_OF_MATRIX_LINE; j++)
        {
            /* Copy each float in the temporary matrix to the result matrix */
            resultMatrix->data[i][j] = tempMatrix->data[i][j];
        }
    }
}