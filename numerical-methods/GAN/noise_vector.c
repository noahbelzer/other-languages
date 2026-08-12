/*
Compile:
gcc -Wall -Wextra -std=c17 -O2 -g -D_GNU_SOURCE noise_vector.c -o noise_vector -lm

-D_GNU_SOURCE unlocks all GNU extensions including M_PI.
The #ifndef M_PI block is skipped when this works.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Fallback
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif


#define VEC_SIZE 10


double normal_distribution(double mean, double std, int x) {
    double result = (1 / (std * sqrt(2 * M_PI))) * exp(-0.5 * pow(((x - mean) / std), 2));

    return result;
}

double box_muller(double mean, double std) {
    // Sample from the normal distribution

    double u1 = (double)rand() / RAND_MAX;  // uniform sample 1 → used for radius
    double u2 = (double)rand() / RAND_MAX;  // uniform sample 2 → used for angle

    // r = sqrt(-2 * ln(u1))  — Rayleigh-distributed radius
    // θ = 2π * u2            — uniform angle
    // Z = r * cos(θ)         — back to Cartesian = standard normal
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

    return mean + std * z;  // scale to desired mean and std
}

int main() {

    double latent_vec_z[VEC_SIZE];
    /*
    int random_num;

    // 1. Seed the random number generator with current time.
    // Seed it with 1 to get reproducible results, but later switch to srand(time(NULL));
    srand(1);

    for (int i = 0; i < VEC_SIZE; i++) {
        // 2. Generate a random number
        random_num = rand();

        vec[i] = random_num;
    }
    */


    for (int i = 0; i < VEC_SIZE; i++) {
        latent_vec_z[i] = box_muller(0.0, 1.0);
    }
    
    // Print the vector
    printf("[ ");
    for (int i = 0; i < VEC_SIZE; i++) {
        printf("%f ", latent_vec_z[i]);
    }
    printf("]\n");

    /*
    double r = normal_distribution(0, 1, 3);
    printf("\n%f\n", r);
    */


    
    return 0;
}