// A calculator for matrices.
// The main function, all the intercation with the user and the command analysis.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymat.h"

// read_mat - a matrix and a list of 16 numbers
// print_mat - a matrix
// add_mat - three matrices
// sub_mat - three matrices
// mul_mat - three matrices
// mul_scalar - a matrix, a scalar, a matrix
// trans_mat - two matrices

int main()
{
    // Create all the matrices and intialize them to zero.
    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;
    
    // Variables for the command.
    char command[10];
    char args[10];
    float list[16] = {0};

    // Initialize all the matrices to zero.
    read_mat(&MAT_A, list);
    read_mat(&MAT_B, list);
    read_mat(&MAT_C, list);
    read_mat(&MAT_D, list);
    read_mat(&MAT_E, list);
    read_mat(&MAT_F, list);

    mat* mats[] = {&MAT_A, &MAT_B, &MAT_C, &MAT_D, &MAT_E, &MAT_F};
    char matNames[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int matIndex = 0;

    // Getting all the inputs and analyse them
    do {
        scanf("%s", command);

        if(strcmp(command, "read_mat") == 0)
        {
            int i;

            if(scanf("%s", args) != 1)
            {
                printf("Missing argument\n");
                continue;
            }
            matIndex = args[0] - 'A';
            if(matIndex < 0 || matIndex >= 6)
            {
                printf("Undefined matrix name\n");
                continue;
            }
            for(int i = 0; i < 16; i++)
            {
                if(scanf("%f", &list[i]) != 1)
                {
                    printf("Argument is not a real number\n");
                    continue;
                }
            }
            read_mat(mats[matIndex], list);
        }
        else if(strcmp(command, "print_mat") == 0)
        {
            scanf("%s", args);
            matIndex = args[0] - 'A';
            if (matIndex >= 0 && matIndex < 6)
            {
                print_mat(mats[matIndex]);
            }
            else
            {
                printf("Undefined matrix name\n");
            }
        }
        else if(strcmp(command, "add_mat"))
        {
            mat* mats[3];
            int i;

            for(i = 0; i < 3; i++)
            {
                scanf("%s", args);
                matIndex = args[0] - 'A';
                if(matIndex < 0 || matIndex >= 6)
                {
                    printf("Undefined matrix name\n");
                    break;
                }
                mats[i] = mats[matIndex];
            }
            add_mat(mats[0], mats[1], mats[2]);
        }
        else if(strcmp(command, "sub_mat"))
        {
            mat* mats[3];
            int i;

            for(i = 0; i < 3; i++)
            {
                scanf("%s", args);
                matIndex = args[0] - 'A';
                if(matIndex < 0 || matIndex >= 6)
                {
                    printf("Undefined matrix name\n");
                    break;
                }
                mats[i] = mats[matIndex];
            }
            sub_mat(mats[0], mats[1], mats[2]);
        }
        else if(strcmp(command, "mul_mat"))
        {
            mat* mats[3];
            int i;

            for(i = 0; i < 3; i++)
            {
                scanf("%s", args);
                matIndex = args[0] - 'A';
                if(matIndex < 0 || matIndex >= 6)
                {
                    printf("Undefined matrix name\n");
                    break;
                }
                mats[i] = mats[matIndex];
            }
            mul_mat(mats[0], mats[1], mats[2]);
        }
        else if(strcmp(command, "mul_scalar"))
        {
            mat* mat;
            float scalar;
            scanf("%s", args);
            matIndex = args[0] - 'A';
            if(matIndex < 0 || matIndex >= 6)
            {
                printf("Undefined matrix name\n");
                continue;
            }
            mat = mats[matIndex];
            if(scanf("%f", &scalar) != 1)
            {
                printf("Argument is not a real number\n");
                continue;
            }
            mul_scalar(mat, scalar, mat);
        }
        else if(strcmp(command, "trans_mat"))
        {
            mat* mat;
            scanf("%s", args);
            matIndex = args[0] - 'A';
            if(matIndex < 0 || matIndex >= 6)
            {
                printf("Undefined matrix name\n");
                continue;
            }
            mat = mats[matIndex];
            trans_mat(mat);
        }
        else if(strcmp(command, "stop"))
        {
            return 0;
        }
        else
        {
            printf("Undefined command name\n");
        }


    } while(strcmp(command, "stop") != 0);
}