/*
Numerical Analysis 2nd Edition - Tim Sauer
pg. 377 (pg. 375 - 377)

Compile:
    g++ -Wall Forward-Difference-Method.cpp -o Forward-Difference-Method
    ./Forward-Difference-Method
*/

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

void Print_Matrix(const vector<vector<double>>& matrix) {
    /*
    Print a 2D vector
    */

    cout << "Matrix: " << endl;
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            // Fix floating point precision printing: Print very small values as zero (to handle -0 printing instead of 0)
            double val = (abs(elem) < 1e-12) ? 0.0 : elem;

            if (val >= 0)
                cout << val << "  ";  // extra space for alignment
            else
                cout << val << " ";
        }
        cout << endl;
    }

    cout << "" << endl;
}

double f(double x) {
    // (sin(2 * pi * x))^2
    return pow(sin(2* M_PI * x), 2);
}

int main() {
    //double t;               // Time

    // From the picture on pg. 376
    double T = 5;               // End of the time interval [0, T]
    int M = 8;                  // Total number of steps in the x direction
    int N = 5;                  // Total number of steps in the t direction

    // Boundary conditions: u(0,t) = u(1,t) = 0 for all time t
    int a = 0;        
    int b = 0;

    double D = 1;           // Diffusion constant
    double h = (b - a) / M; // h
    double k = T / N;       // k
    int m = M - 1;          // The matrix A is m x m where m = M - 1
    int n = N;
    double sigma = (D * k) / pow(h, 2);

    cout << 1 - 2 * sigma << endl;
    cout << sigma << endl;



    // Put Matrix A together. It contains 1 - 2 * sigma on the main diagonal, and sigma on the off-diagonal.
    vector<vector<double>> matrix_A(m, vector<double>(m));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i == j) {
                matrix_A[i][j] = 1 - 2 * sigma;
            } else if ((j = i - 1) || (j = i + 1)) {
                matrix_A[i][j] = sigma;
            }
        }
    }

    //Print_Matrix(matrix_A);

    return 0;
}