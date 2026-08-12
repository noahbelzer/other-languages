/*
Compile
g++ -Wall Secant-Method.cpp -o Secant-Method
*/

#include <iostream>
#include <cmath>
#include <iomanip>
#include "Secant-Method.h"
using namespace std;

// We are using f(x) instead of f(x) so it does not get confused with the function f from Bisection-Method
double f(double x) {
    // f(x) = x^2 - 2
    // Root = 1.41421
    return pow(x, 2) - 2;
}


double Secant_Method_Algorithm(double x_n, double x_n_minus_1) {
    /*
    Take Newton's Method:
    x_{n+1} = x_n - (f(x_n) / f'(x_n))

    Use:
    f'(x_n) ~= (f(x_n) - f(x_{n-1})) / (x_n - x_{n-1})
    */
    double x_n_plus_1 = x_n - (f(x_n) * ((x_n - x_n_minus_1) / (f(x_n) - f(x_n_minus_1))));
    return x_n_plus_1;
}


pair<double, double> Initial_Points() {
    /*
    Currently, this function will establish initial points, but it
    will be for the root that is closest to zero.
    */

    // Arbitrary staring points close to zero. Note: x0 and x1 may need to be closer to zero if f(x) is a parabola that is very narrow near the origin.
    double x0 = -0.25;
    double x1 = 0.5;

    double f_of_x0 = f(x0);
    double f_of_x1 = f(x1);

    // Find the initial interval [x0, x1] where f(x0) and f(x1) have opposite signs
    // Continue adjusting x0 and x1 until f(x0) and f(x1) have opposite signs
    while ((f_of_x0 > 0.0 && f_of_x1 > 0.0) || (f_of_x0 < 0.0 && f_of_x1 < 0.0)) {
        // Arbitrary steps: Move x0 1 to the left and x1 1 to the right until f(x0) and f(x1) have opposite signs. These can be modified, and may need to be smaller if f(x) is a parabola that is very narrow near the origin.
        x0 -= 1.0;
        x1 += 1.0;
        
        f_of_x0 = f(x0);
        f_of_x1 = f(x1);
    }

    cout << "Initial Points:" << endl;
    cout << "x_0 = " << x0 << endl;
    cout << "x_1 = " << x1 << endl;
    return {x0, x1};
}


int main () {
    // 2 initial guesses: Use pair<double, double> Initial_Points;
    // This is not required. We can choose any two distinct real numbers close to the root, but will help give us a good initial 2 guesses without needing to see the graph.
    pair<double, double> initial_x_values = Initial_Points();
    double x_0 = initial_x_values.first;
    double x_1 = initial_x_values.second;
   
    // Set the tolerance: Currently set to 0.000001
    double root_tolerance = 1.0E-6;

    bool tolerance_reached = false;
    int num_of_iterations = 0;

    double x_n_plus_1; 
    double x_n = x_1;
    double x_n_minus_1 = x_0;

    do {
        x_n_plus_1 = Secant_Method_Algorithm(x_n, x_n_minus_1);
        ++num_of_iterations;

        cout << "Num of iterations: " << num_of_iterations << ", Current x_n_plus_1: " << setprecision(10) << x_n_plus_1 << endl;

        cout << "x_n_plus_1 - x_n: " << abs(x_n_plus_1 - x_n) << endl;
        if (abs(x_n_plus_1 - x_n) > root_tolerance) {
            // When the next iteration runs, x_n from this iteration will be x_n_minus_1
            x_n_minus_1 = x_n;

            // When the next iteration runs, x_n will be this current x_n_plus_1
            x_n = x_n_plus_1;
        } else {
            tolerance_reached = true;
        }
    } while (!tolerance_reached);

    cout << "Root: " << x_n_plus_1 << endl;
    return 0;
}
