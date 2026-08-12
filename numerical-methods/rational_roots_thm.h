#ifndef RATIONAL_ROOTS_THM_H
#define RATIONAL_ROOTS_THM_H

int is_whole_number(double x);
int* find_factors(double num, int* size);
double* rational_roots_theorem(int* constant_factors, int constant_size, int* leading_coeff_factors, int lead_coeff_size, int* size);

#endif 