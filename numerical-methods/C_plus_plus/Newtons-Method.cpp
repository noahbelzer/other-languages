/*
Compile
g++ -Wall Newtons-Method.cpp -o Newtons-Method
*/

#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

double f(double x) {
    // f(x) = x^3 - 8x^2 + 4x + 10
    return pow(x, 3) - 8*pow(x, 2) + 4*x + 10;
}


double f_prime(double x) {
    // f'(x) = 3x^2 - 16x + 4
    return 3*pow(x, 2) - 16*x + 4;
}


double Algorithm(double x_n) {
    double x_n_plus_1 = x_n - (f(x_n) / f_prime(x_n));
    return x_n_plus_1;

}


int main () {
    // Initial guess
    double x_0 = 8.0; 

    // Set the tolerance: Currently set to 0.000001
    double root_tolerance = 1.0E-6;

    bool tolerance_reached = false;
    int num_of_iterations = 0;
    double x_n_plus_1; 

    do {
        x_n_plus_1 = Algorithm(x_0);
        ++num_of_iterations;

        cout << "Num of iterations: " << num_of_iterations << ", Current x_n_plus_1: " << setprecision(10) << x_n_plus_1 << endl;

        cout << "x_n_plus_1 - x_0: " << abs(x_n_plus_1 - x_0) << endl;
        if (abs(x_n_plus_1 - x_0) > root_tolerance) {
            x_0 = x_n_plus_1;
        } else {
            tolerance_reached = true;
        }
    } while (!tolerance_reached);

    cout << "Root: " << x_n_plus_1 << endl;

    return 0;
}