/*
Compile:
g++ -Wall eigen_for_2_by_2.cpp Gauss-Jordan-with-Partial-Pivoting.cpp -o eigen_for_2_by_2
*/

#include "eigen_for_2_by_2.h"
#include "Gauss-Jordan-with-Partial-Pivoting.h"

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


vector<vector<double>> eigen(const vector<vector<double>>& passed_matrix) {
    
    // Initialize the matrix A - λI, where λ is the eigenvalue and I is the identity matrix
    vector<vector<double>> A_minus_lambda_I = passed_matrix;

    for (int i = 0; i < A_minus_lambda_I.size(); ++i) {
        A_minus_lambda_I[i][i] -= 1;  // Subtract λ from the diagonal elements
    }
}


int main() {
    
    // Matrix Analysis and Applied Linear Algebra, pg. 500, 7.1.1
    vector<vector<double>> typed_matrix = {
        {-10, -7},
        {14, 11}
    };


    cout << "Matrix A: " << endl;
    Print_Matrix(typed_matrix);

    //vector<vector<double>> result = Gauss_Jordan(typed_matrix);

    //cout << "Finished Matrix: " << endl;
    //Print_Matrix(result);
    
    return 0;
}