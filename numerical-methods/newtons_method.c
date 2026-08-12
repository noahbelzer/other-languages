/*
Compile:
gcc -Wall -Wextra -Wpedantic -std=c17 -O2 -g newtons_method.c -o newtons_method -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "newtons_method.h"

double f(double x) {
    // f(x) = x^3 - 8x^2 + 4x + 10
    return pow(x, 3) - 8*pow(x, 2) + 4*x + 10;
}


double f_prime(double x) {
    // f'(x) = 3x^2 - 16x + 4
    return 3*pow(x, 2) - 16*x + 4;
}


double newtons_method_algorithm(double x_n) {
    double x_n_plus_1 = x_n - (f(x_n) / f_prime(x_n));
    return x_n_plus_1;

}

/*
int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("Arg %d: %s ", i, argv[i]);
    }
    printf("\n");

    // Initial guess
    double x_0 = 8.0; 

    // Set the tolerance: Currently set to 0.000001
    double root_tolerance = 1.0E-6;

    bool tolerance_reached = false;
    int num_of_iterations = 0;
    double x_n_plus_1; 

    do {
        x_n_plus_1 = newtons_method_algorithm(x_0);
        ++num_of_iterations;

        printf("Num of iterations: %d, Current x_n_plus_1: %.6f\n", num_of_iterations, x_n_plus_1);

        double abs_val = fabs(x_n_plus_1 - x_0);

        printf("x_n_plus_1 - x_0: %.6f\n", abs_val);

        
        if (abs_val > root_tolerance) {
            x_0 = x_n_plus_1;
        } else {
            tolerance_reached = true;
        }
    } while (!tolerance_reached);

    // True answer for this example. Initial point at 8: 7.25921
    printf("Root: %.6f\n", x_n_plus_1);

    return 0;
}
*/