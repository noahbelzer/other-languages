/*
Compile:
gcc -fopenmp -Wall -Wextra -std=c17 -O2 -g -D_GNU_SOURCE toy_parallel_mat_vec_mult.c -o toy_parallel_mat_vec_mult -lm
*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 3
#define COLS 4


void mat_vec_mult(double A[ROWS][COLS], double v[COLS], double res[ROWS]) {
    #pragma omp parallel for
    for (int i = 0; i < ROWS; i++) {
        double sum = 0;
        for (int j = 0; j < COLS; j++) {
            sum += A[i][j] * v[j];
        }

        res[i] = sum;
    }

    for (int i = 0; i < ROWS; i++) {
        printf("%f\n", res[i]);
    }
}


int main() {
    // matrix   vector   =   vector
    // 3 x 4    4 x 1    =   3 x 1

    double matrix[ROWS][COLS] = {
        {1, 12, 3, 4},
        {2, 4, 62, 7},
        {1, 2,  3, 6}
    };

    double vec[COLS] = {3, 10, 9, 11};

    double res_vec[ROWS];

    mat_vec_mult(matrix, vec, res_vec);

    return 0;

    // Take a very large and dense matrix: 10,000 × 10,000+
    // Multiply this matrix by thousands of different vectors in a loop.
    double **matrix = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
        matrix[i] = malloc(cols * sizeof(double));
}