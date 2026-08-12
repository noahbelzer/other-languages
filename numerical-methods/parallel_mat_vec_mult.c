/*
Compile:
gcc -fopenmp -Wall -Wextra -std=c17 -O2 -g -D_GNU_SOURCE parallel_mat_vec_mult.c -o parallel_mat_vec_mult -lm

Run:
./parallel_mat_vec_mult datefile.csv
*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Fallback
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif


#define ROWS 30
#define COLS 30


double box_muller(double mean, double std) {
    // Sample from the normal distribution

    double u1 = ((double)rand() + 1.0) / (RAND_MAX + 2.0);  // guard against log(0)
    double u2 = (double)rand() / RAND_MAX;  // uniform sample 2 → used for angle

    // r = sqrt(-2 * ln(u1))  — Rayleigh-distributed radius
    // θ = 2π * u2            — uniform angle
    // Z = r * cos(θ)         — back to Cartesian = standard normal
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

    return mean + std * z;  // scale to desired mean and std
}


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


int main(int argc, char *argv[]) {
    // Take a very large and dense matrix: 10,000 × 10,000+
    // Multiply this matrix by thousands of different vectors in a loop.

    const char *filename = argv[1];

    FILE *f = fopen(filename, "w");

    setvbuf(f, NULL, _IOFBF, 1024 * 1024);

    srand(1);  // swap to time(NULL) for different results each run

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            double val = box_muller(0.0, 10.0);
            
            if (j < COLS - 1)
                fprintf(f, "%10.4f,", val);
            else
                fprintf(f, "%10.4f", val);
        }
        
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Generated matrix: %s\n", filename);


    /*

    double vec[COLS] = {3, 10, 9, 11};

    double res_vec[ROWS];

    mat_vec_mult(matrix, vec, res_vec);
    */

    return 0;


}