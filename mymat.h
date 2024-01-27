// Defenition of the typedef mat, all the declarations of the functions in mymat.c

#ifndef MYMAT_H
#define MYMAT_H

typedef struct mat {
    float data[4][4];
} mat;

void read_mat(mat* matrix, float list[16]);
void print_mat(mat* matrix);
void add_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix);
void sub_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix);
void mul_mat(mat* firstMatrix, mat* secondMatrix, mat* resultMatrix);
void mul_scalar(mat* matrix, float scalar, mat* resultMatrix);
void trans_mat(mat* matrix, mat* resultMatrix, mat* tempMatrix);

#endif