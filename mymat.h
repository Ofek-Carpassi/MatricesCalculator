/* Defenition of the typedef mat, all the declarations of the functions in mymat.c */

#ifndef MYMAT_H
#define MYMAT_H

/* Defenition of the typedef mat */
typedef struct mat {
    float data[4][4]; /* 4x4 matrix */
} mat;

void read_mat(mat* matrix, float list[16]); /* Read the matrix from the user */
void print_mat(mat* matrix); /* Print the matrix */
void add_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix); /* Add two matrices */
void sub_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix); /* Subtract two matrices */
void mul_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix); /* Multiply two matrices */
void mul_scalar(mat* matrix, float scalar, mat* resultMatrix); /* Multiply a matrix by a scalar */
void trans_mat(mat* matrix, mat* resultMatrix, mat* tempMatrix); /* Transpose a matrix */

#endif