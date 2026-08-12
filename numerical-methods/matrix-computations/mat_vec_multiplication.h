#ifndef MAT_VEC_MULTIPLICATION_H
#define MAT_VEC_MULTIPLICATION_H

#include <vector>

std::vector<double> serial_familiar_definition(const std::vector<std::vector<double>>& A, const std::vector<double>& x, int m, int n);
void get_dimensions(const std::vector<std::vector<double>>& A, int& m, int& n);

#endif
