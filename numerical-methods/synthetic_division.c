/*
Compile:
gcc -Wall -Wextra -Wpedantic -std=c17 -O2 -g synthetic_division.c rational_roots_thm.c newtons_method.c -o synthetic_division -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "synthetic_division.h"

/*
Synthetic Division: (k⁴ + 8k³ + 8k² - 32k - 48) ÷ (k - 2)
Since we're dividing by (k - 2), we use r = 2.
Write just the coefficients of the polynomial, then follow a simple multiply-and-add pattern:

2  |  1    8    8   -32   -48
   |       2   20    56    48
   ─────────────────────────
      1   10   28    24  |  0
*/
double* synthetic_div(double r, double* coeffs, int num_of_coeffs, int* multiplicity) {
    // Base case.
    if (num_of_coeffs <= 1) {
        return coeffs;
    }
    
    double* new_coefficients = (double*)malloc(num_of_coeffs * sizeof(double));
    
    new_coefficients[0] = coeffs[0];

    for (int i = 1; i < num_of_coeffs; i++) {
        new_coefficients[i] = (new_coefficients[i-1] * r) + coeffs[i];
    }

    // Take the last number in the array. If the remainder is 0, do synthetic division again.
    if (new_coefficients[num_of_coeffs - 1] == 0) {
        (*multiplicity)++;

        // Recursvie case.
        double* result =  synthetic_div(r, new_coefficients, num_of_coeffs - 1, multiplicity);
        free(new_coefficients);
        return result;
    }

    return new_coefficients;
}


int main() {
    // k^4 + 8k^3 + 8k^2 - 32k - 48
    // Expected factors: (k-2)(k+2)^2(k+6)
    double coeffs[] = {1, 8, 8, -32, -48};
    int num_coeffs = 5;

    printf("=== Testing synthetic_div ===\n");
    printf("Polynomial: k^4 + 8k^3 + 8k^2 - 32k - 48\n");
    printf("Expected: (k-2)(k+2)^2(k+6)\n\n");

    // Test r = 2 (should give multiplicity 1)
    printf("--- Testing r = 2 ---\n");
    int multiplicity_2 = 0;
    double* result_2 = synthetic_div(2.0, coeffs, num_coeffs, &multiplicity_2);
    printf("Final multiplicity of r=2: %d\n", multiplicity_2);
    printf("Reduced polynomial coeffs: ");
    for (int i = 0; i < num_coeffs - 1 - (multiplicity_2 - 1); i++) {
        printf("%.3f ", result_2[i]);
    }
    printf("\n\n");

    // Test r = -2 on reduced polynomial (should give multiplicity 2)
    printf("--- Testing r = -2 on reduced polynomial ---\n");
    int multiplicity_neg2 = 0;
    int reduced_size = num_coeffs - multiplicity_2;
    double* result_neg2 = synthetic_div(-2.0, result_2, reduced_size, &multiplicity_neg2);
    printf("Final multiplicity of r=-2: %d\n", multiplicity_neg2);
    printf("Reduced polynomial coeffs: ");
    for (int i = 0; i < reduced_size - multiplicity_neg2; i++) {
        printf("%.3f ", result_neg2[i]);
    }
    printf("\n\n");

    // Test r = -6 on remaining polynomial (should give multiplicity 1)
    printf("--- Testing r = -6 on reduced polynomial ---\n");
    int multiplicity_neg6 = 0;
    int reduced_size_2 = reduced_size - multiplicity_neg2;
    double* result_neg6 = synthetic_div(-6.0, result_neg2, reduced_size_2, &multiplicity_neg6);
    printf("Final multiplicity of r=-6: %d\n", multiplicity_neg6);
    printf("\n");

    printf("=== Final Factored Form ===\n");
    printf("(k - 2)^%d * (k + 2)^%d * (k + 6)^%d\n", 
            multiplicity_2 + 1, multiplicity_neg2 + 1, multiplicity_neg6 + 1);

    // Free memory
    free(result_2);
    free(result_neg2);
    free(result_neg6);

    return 0;
}
