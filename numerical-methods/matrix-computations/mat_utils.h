#ifndef MAT_UTILS_H
#define MAT_UTILS_H

#include <vector>
#include <string>

void print_matrix(const std::vector<std::vector<double>>& matrix);
void print_vector(const std::vector<double>& vec);
std::vector<std::vector<double>> read_matrix_csv(const std::string& filename);

#endif
