/*
Compile:
g++ -Wall Compute-Inverse.cpp Gauss-Jordan-with-Partial-Pivoting.cpp -o Compute-Inverse
./Compute-Inverse
*/

#include "Compute-Inverse.h"
#include "Gauss-Jordan-with-Partial-Pivoting.h"

#include <iostream>
using namespace std;

vector<vector<double>> Create_Identity_Matrix(size_t rows_of_nonsingular, size_t cols_of_nonsingular) {
    vector<vector<double>> identity_matrix(rows_of_nonsingular, vector<double>(cols_of_nonsingular));

    // Loop over the number of rows of the passed_matrix
    for (size_t i = 0; i < rows_of_nonsingular; ++i) {
        // Loop over the number of cols of the passed_matrix
        for (size_t j = 0; j < cols_of_nonsingular; ++j) {
            if (i == j) {
                // Set diagonal to 1
                identity_matrix[i][j] = 1;  
            } else {
                // Set off-diagonal to 0
                identity_matrix[i][j] = 0;  
            }
        }
    }

    return identity_matrix;
}


vector<vector<double>> Create_Augmented_Matrix(const vector<vector<double>>& passed_matrix, vector<vector<double>> identity_matrix, size_t rows_of_nonsingular, size_t cols_of_nonsingular) {
    // Declare the size of our augmented matrix. Since we are adding the identity matrix to the right, the number of columns doubles.
    vector<vector<double>> augmented_matrix(rows_of_nonsingular, vector<double>(cols_of_nonsingular * 2));  
    
    for (size_t i = 0; i < rows_of_nonsingular; ++i) {
        for (size_t j = 0; j < cols_of_nonsingular * 2; ++j) {
            /*
            // Built-in funciton, good for serial
            augmented_matrix[i].insert(augmented_matrix[i].end(), identity_matrix[i].begin(), identity_matrix[i].end());
            */

            // j ranges through the the cols of A and the identity matrix, so the first half is A, and the second half is I. cols_of_nonsingular indicates this halfway point.
            if (j < cols_of_nonsingular) {
                augmented_matrix[i][j] = passed_matrix[i][j];
            } else {
                augmented_matrix[i][j] = identity_matrix[i][j - cols_of_nonsingular];
            }
        }
    }

    return augmented_matrix;
}


int main(int argc, char* argv[]) {
    vector<vector<double>> nonsingular_matrix = {
        {1, 1, 1},
        {1, 2, 2},
        {1, 2, 3}
    };

    // These should be the same for square matrices
    size_t rows_of_nonsingular = nonsingular_matrix.size();
    size_t cols_of_nonsingular = nonsingular_matrix[0].size();

    vector<vector<double>> identity = Create_Identity_Matrix(rows_of_nonsingular, cols_of_nonsingular);
    vector<vector<double>> augmented_matrix = Create_Augmented_Matrix(nonsingular_matrix, identity, rows_of_nonsingular, cols_of_nonsingular);
    vector<vector<double>> result = Gauss_Jordan(augmented_matrix);

    cout << "Augmented Matrix: " << endl;
    Print_Matrix(result);

    vector<vector<double>> inverse_matrix(rows_of_nonsingular, vector<double>(cols_of_nonsingular));  

    // Store the inverse matrix
    for (size_t i = 0; i < rows_of_nonsingular; ++i) {
        for (size_t j = 0; j < cols_of_nonsingular; ++j) {
            // We have to skip past the identity matrix portion, which is on the left-hand side of the result
            inverse_matrix[i][j] = result[i][j + cols_of_nonsingular];
        }
    }

    cout << "Inverse Matrix: " << endl;
    Print_Matrix(inverse_matrix);

    /* 
    Solution:
     2  -1   0
    -1   2  -1
     0  -1   1
    
    */

    return 0;
}
