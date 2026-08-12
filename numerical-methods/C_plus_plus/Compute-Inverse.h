#ifndef COMPUTE_INVERSE_H
#define COMPUTE_INVERSE_H

#include <vector>
using namespace std;

// Function declarations (prototypes)
vector<vector<double>> Create_Identity_Matrix(size_t rows_of_nonsingular, size_t cols_of_nonsingular);
vector<vector<double>> Create_Augmented_Matrix(const vector<vector<double>>& passed_matrix, vector<vector<double>> identity_matrix, size_t rows_of_nonsingular, size_t cols_of_nonsingular);

#endif 