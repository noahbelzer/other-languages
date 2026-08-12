/*
Compile:
gcc -Wall -Wextra -Wpedantic -std=c17 -O2 -g rational_roots_thm.c -o rational_roots_thm -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "rational_roots_thm.h"


int is_whole_number(double x) {
    //printf("x: %.2f, ", x);
    //printf("(int)x: %d\n", (int)x);
    return x == (int)x;
}

int* find_factors(double num, int* size) {
    // Assumptions: We do not pass 0 into this function, as it has infinite factors
    // We must pass num as a double even though it should be an int because we must have double / double for num / i.
    
    if (num < 0) {
        num = -1 * num;
    }

    // 1. Determine how many factors we have.
    int counter = 0;

    for (int i = 1; i <= num; i++) {
        if (is_whole_number(num / (double)i)) {
            counter++;
        }
    }

    //printf("counter = %d\n", counter);
   
    // 2. Allocate memory.
    // (int*): Cast the memory to int* to tell the compiler: "treat this memory as a pointer to integers"
    // int* factors: declares factors as an integer pointer — it holds the memory address of the first element of your array.
    // Do counter * 2 since we are adding factors and their negative versions.
    int* factors = (int*)malloc(counter * 2 * sizeof(int));

    // 3. Add all factors to an array called factors.
    int index = 0;
    for (int i = 1; i <= num; i++) {
        if (is_whole_number(num / (double)i)) {
            //printf("num / %d = %.3f\n", i, num / (double)i);
            //printf("i = %d\n", i);
            // Add the negative and positive version of the factor to the array.
            factors[index] = -i;
            index++;
            factors[index] = i;
            index++;
        }
    }

    // We multiply by 2 because we are also including the negative version of each number.
    *size = counter * 2;

    // 4. Return the pointer to the first integer of the array (essentially returning the array).
    return factors;
}


double* rational_roots_theorem(int* constant_factors, int constant_size, int* leading_coeff_factors, int lead_coeff_size, int* size) {
    // Possible Rational Roots = (factors of the constant) / (factors of the lead coefficient)
    
    // The maximum number of different numbers we can get by dividing each number in the numerator by each number in the denomicator is constant_size * lead_coeff_size
    // Note, since this is the maximum number of numbers we may have, there might be extra space in this array that is filled with no numbers.
    double* possible_rational_roots = (double*)malloc(constant_size * lead_coeff_size * sizeof(double));
    
    int counter = 0;

    for (int i = 0; i < constant_size; i++) {
        for (int j = 0; j < lead_coeff_size; j++) {
            //printf("constant_factors[i]: %d\n", constant_factors[i]);
            //printf("leading_coeff_factors[j]: %d\n", leading_coeff_factors[j]);

            double element_to_add = (constant_factors[i] / leading_coeff_factors[j]);
            bool element_already_in = false;

            for (int k = 0; k < counter; k++) {
                if (element_to_add == possible_rational_roots[k]) {
                    element_already_in = true;
                }
            }

            if (element_already_in == false) {
                possible_rational_roots[counter] = element_to_add;
                counter++;
            }
  
            // Reset before the next iteration
            element_already_in = false;
        }
    }

    *size = counter;
    
    return possible_rational_roots;
}

/*
int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("Arg %d: %s ", i, argv[i]);
    }
    printf("\n");

    int* facs = find_factors(512);
    int index = 0;
    while (facs[index] != '\0') {
        printf("%d ", facs[index]);
        index++;
    }
    printf("\n");

    free(facs);

    return 0;
}
*/