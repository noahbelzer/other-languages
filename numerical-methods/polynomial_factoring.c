/*
Compile:
gcc -Wall -Wextra -Wpedantic -std=c17 -O2 -g polynomial_factoring.c rational_roots_thm.c newtons_method.c -o polynomial_factoring -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "newtons_method.h"
#include "rational_roots_thm.h"

// Hardcoded
#define NUM_OF_COEFFICIENTS 5

struct Polynomial {
    // Contains 3 parts: function, array of coefficients, constant term
    double function;
    double coefficients[NUM_OF_COEFFICIENTS];
    double constant;
};


double func(double x) {
    // Hardcoded
    // f(x) = x^4 - 4x^2 + 8x^3 - 32x + 12x^2 - 48
    // Solution: (x-2) (x+2)^2 (x+6)
    return pow(x, 4) - 4*pow(x, 2) + 8*pow(x, 3) - 32 * x + 12*pow(x, 2) - 48;
}


struct Polynomial get_polynomial(double x) {
    // Hardcoded function and coefficients provided. A robust parser may be required for autmoatic reading.
    double coefficients[] = {1, -4, 8, -32, 12};
    double constant = -48;

    // Create a Polynomial and assign our hardcoded function
    struct Polynomial poly;
    
    poly.function = f(x);
    for (int i = 0; i < NUM_OF_COEFFICIENTS; i++) {
        poly.coefficients[i] = coefficients[i];
    }
    poly.constant = constant;

    return poly;
}


int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("Arg %d: %s ", i, argv[i]);
    }
    printf("\n\n");

    // Arbitrary initialization for x
    double x = 0.0;

    struct Polynomial f = get_polynomial(x);

    int num_of_constant_factors = 0;
    int* constant_factors = find_factors(f.constant, &num_of_constant_factors);

    int num_of_leading_coeff_factors = 0;
    int* leading_coeff_factors = find_factors(f.coefficients[0], &num_of_leading_coeff_factors);

    //printf("constant: %.3f, coeff: %.3f\n", f.constant, f.coefficients[0]);

    int num_of_potential_rational_roots = 0;
    double* potential_rational_roots = rational_roots_theorem(
        constant_factors,
        num_of_constant_factors,
        leading_coeff_factors,
        num_of_leading_coeff_factors,
        &num_of_potential_rational_roots
    );


    printf("num_of_constant_factors: %d\n", num_of_constant_factors);
    printf("num_of_leading_coeff_factors: %d\n", num_of_leading_coeff_factors);

    int index = 0;
    while (potential_rational_roots[index] != '\0') {
        printf("%.3f ", potential_rational_roots[index]);
        index++;
    }
    printf("\n");

    for (int i = 0; i < num_of_potential_rational_roots; i++) {
        synthetic_div(i, f.coefficients, NUM_OF_COEFFICIENTS, potential_rational_roots);
    }
    

    free(constant_factors);
    free(leading_coeff_factors);
    

    return 0;
}