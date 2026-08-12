#ifndef GAUSS_JORDAN_WITH_PARTIAL_PIVOTING_H
#define GAUSS_JORDAN_WITH_PARTIAL_PIVOTING_H

#include <vector>
using namespace std;

// Function declarations (prototypes)
void Print_Matrix(const vector<vector<double>>& matrix);
vector<vector<double>> Gauss_Jordan(const vector<vector<double>>& passed_matrix);

#endif 