/*
Compile:
    g++ -Wall Matrix-Multiplication-Serial.cpp -o Matrix-Multiplication-Serial
    ./Matrix-Multiplication-Serial
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;

void PrintMatrix(const vector<vector<int>>& matrix) {
    /*
    Print a 2D vector
    */

    cout << "Matrix: " << endl;

    // For row in matrix
    for (const auto& row : matrix) {
        // For element in row
        for (const auto& elem : row) {
            // Fix floating point precision printing: Print very small values as zero (to handle -0 printing instead of 0)
            double val = (abs(elem) < 1e-12) ? 0.0 : elem;
            
            // Give 2 spaces for positive numbers, and one for negatuves, for alignment
            if (val >= 0)
                cout << val << "  ";  // extra space for alignment
            else
                cout << val << " ";
        }
        cout << endl;
    }

    cout << "" << endl;
}

vector<vector<int>> Multiplication(const vector<vector<int>>& matrix_A, const vector<vector<int>>& matrix_B) {
    int dot_product = 0;

    // matrix_C has the same number of rows as matrix_A
    vector<vector<int>> matrix_C(matrix_A.size());
    
    // Loop over the number of rows in A
    for (int i = 0; i < matrix_A.size(); ++i) {

        // Loop over the number of cols in B
        for (int k = 0; k < matrix_B[0].size(); ++k) {
            dot_product = 0;

            // Loop over the number of rows in B. Remember, the number of cols in A must match the number of rows in B.
            for (int j = 0; j < matrix_B.size(); ++j) {
                dot_product += matrix_A[i][j] * matrix_B[j][k]; 
            }

            // C has the same number of rows as A, so we can use i, which counts the number of rows in A
            cout << "Dot Product for C row " << i << ": " << dot_product << endl;
            matrix_C[i].push_back(dot_product);
            PrintMatrix(matrix_C);
        }
    }

    return matrix_C;
}

int main(int argc, char* argv[]) {
    vector<vector<int>> matrix_1 = {
        {2,  1,  0, 3},
        {1, -4,  1, 2},
        {3,  0, -1, 9}
    };

    vector<vector<int>> matrix_2 = {
        {1, 1,  6},
        {3, 0, -1},
        {0, 5,  4},
        {2, 7, -3}
    };

    //cout << matrix_2.size() << " and " << matrix_2[0].size() << endl;
    vector<vector<int>> result = Multiplication(matrix_1, matrix_2);
    PrintMatrix(result);

    return 0;
}