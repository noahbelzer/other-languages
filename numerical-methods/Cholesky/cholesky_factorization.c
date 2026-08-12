/*
Compile:
gcc -Wall -Wextra -Wpedantic -std=c17 -O2 -g cholesky_factorization.c -o cholesky_factorization -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void print_matrix(const char *name, int rows, int cols, double *matrix) {
    printf("%s =\n", name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.2f  ", matrix[i + j * rows]);
        }
        printf("\n");
    }
    printf("\n");
}


void dot_products(int rows, int cols, double *A, double *L, double *L_Transpose) {
    // Store our computations
    double *intermediate = malloc(rows * cols * sizeof(double));

    char *new_name = "L";
    
    // Loop over the number of rows in L
    for (int i = 0; i < rows; ++i) {

        // Loop over the number of cols in L^T
        for (int j = 0; j < cols; ++j) {

            // Loop over the number of rows in L^T. 
            for (int k = 0; k < rows; ++k) {


    
                double sum = 


                L[i + j * rows] = sqrt(A[i + j * rows] - );

                
                print_matrix(new_name, rows, cols, L);
            }

                
                    
            // Basic dot product formula: dot_product += L[i + k * rows] * L_Transpose[k + j * rows]; 
        }
    }
}


void fill_matrix(int rows, int cols, double *matrix) {

    // --------- LL^T = A ---------
    // L: Lower Triangular
    double *L = malloc(rows * cols * sizeof(double));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i >= j) {
                L[i + j * rows] = NAN;
            } else {
                L[i + j * rows] = 0;
            }
                
        }
    }

    char *name = "L";
    print_matrix(name, rows, cols, L);

    // L^T: Upper Triangular
    double *L_Transpose = malloc(rows * cols * sizeof(double));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            L_Transpose[i + j * rows] = L[j + i * rows];
        }
    }

    char *new_name = "L^T";
    print_matrix(new_name, rows, cols, L_Transpose);


    dot_products(rows, cols, matrix, L, L_Transpose);
}


int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("Arg %d: %s ", i, argv[i]);
    }
    printf("\n\n");

    // Since your .bin file was written column-major (with order='F' in Python), you must always use i + j * rows when reading it in C.
    FILE *file = fopen("matrix.bin", "rb");

    int rows;
    int cols;

    fread(&rows, sizeof(int), 1, file);
    fread(&cols, sizeof(int), 1, file);

    double *matrix_A = malloc(rows * cols * sizeof(double));

    fread(matrix_A, sizeof(double), rows * cols, file);

    char* A_name = "A";
    print_matrix(A_name, rows, cols, matrix_A);

    fill_matrix(rows, cols, matrix_A);

    fclose(file);
    free(matrix_A);
    return 0;

    // For solving, there are only cases wher you are    (known num)^2 = num.     Or, you multiplying   (known num) * unknown = known
}