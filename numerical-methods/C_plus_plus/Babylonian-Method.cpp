/*
Helpful Source: https://youtu.be/4SnKNfMGAMY?si=H6ZK76p-Hqk_WyyI

g++ -Wall Babylonian-Method.cpp -o Babylonian-Method
*/

#include <iostream>
#include <cmath>
using namespace std;


double Absolute_Relative_Error(double x_n, double x_n_minus_1) {
    return abs((x_n - x_n_minus_1) / x_n);
}


double Babylonian(double val, double initial, double tol) {
    double x_n;
    double x_n_minus_1 = initial;

    // Arbitrary initialization
    double current_err = 1.0;

    while(current_err > tol) {
        x_n = (x_n_minus_1 + (val / x_n_minus_1)) / 2.0;
        current_err = Absolute_Relative_Error(x_n, x_n_minus_1);

        if (current_err <= tol) {
            return x_n;
        } else {
            // For the next iteration, x_n_minus_1 will be this current x_n.
            x_n_minus_1 = x_n;
        }
    }

    // Safety return statement (should never run)
    return 0.0;
}


int main() {
    double initial_estimate = 1.0;
    double tolerance = 0.001;

    // Set the value that you want to find the square root of
    double value = 23.67;

    double result = Babylonian(value, initial_estimate, tolerance);

    cout << "The result is: " << result << endl;

    return 0;
}