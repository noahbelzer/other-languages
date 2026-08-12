/*
Compile:
g++ -Wall Bisection-Method.cpp -o Bisection-Method
*/

#include <iostream>
#include <cmath>
#include "Bisection-Method.h"
using namespace std;

double f(double x) {
    // f(x) = x^3 - x - 2
    return pow(x, 3) - x - 2; 
}

double Algorithm(double x0, double x1) {
    double f_of_x0 = f(x0);
    double f_of_x1 = f(x1);
    double x2;
    double f_of_x2;

    // Arbitrary initialization to enter the loop
    double correction_term = 1.0;
    
    // Continue the loop until the correction term is less than the arbitrary tolerance level
    while (abs(correction_term) > 1.0E-6) {
        f_of_x0 = f(x0);
        f_of_x1 = f(x1);

        // First, if either endpoint is already root, return it and stop
        if (f_of_x0 == 0.0) {
            cout << "Bisection Method Root Found at x0 = " << x0 << " | f(x0) = " << f(x0) << endl;
            return x0;
        } else if (f_of_x1 == 0.0) {
            cout << "Bisection Method Root Found at x1 = " << x1 << " | f(x1) = " << f(x1) << endl;
            return x1;
        }

        // Ensure that f(x0) and f(x1) have opposite signs before proceeding
        if ((f_of_x0 > 0 && f_of_x1 < 0) || (f_of_x0 < 0 && f_of_x1 > 0)) {
            // x2 = (x0 + x1) / 2.0 = x1 - (x1 - x0) / 2.0;
            x2 = x1 - (x1 - x0) / 2.0;

            correction_term = (x1 - x0) / 2.0;
            f_of_x2 = f(x2);

            // Swap x2 with either x0 or x1 (whichever has the same sign as f(x2))
            if (f_of_x0 < 0 && f_of_x2 < 0) {
                x0 = x2; 
            } else if (f_of_x1 < 0 && f_of_x2 < 0) {
                x1 = x2; 
            } else if (f_of_x0 > 0 && f_of_x2 > 0) {
                x0 = x2; 
            } else if (f_of_x1 > 0 &&f_of_x2 > 0) {
                x1 = x2; 
            }
        }
    }

    // Return either x0 or x1 as the root approximation (whichever gives an f(x) value closer to zero)
    if (abs(f(x0)) < abs(f(x1))) {
        cout << "Bisection Method Root Approximation: x0 = " << x0 << " | f(x0) = " << f(x0) << endl;
        return x0;
    } else {
        cout << "Bisection Method Root Approximation: x1 = " << x1 << " | f(x1) = " << f(x1) << endl;
        return x1;
    }
}

pair<double, double> Initial_Interval() {
    /*
    Currently, this function will establish an initial interval to begin the Bisection Method, but it
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


int main() {
    pair<double, double> initial_interval = Initial_Interval();

    double root = Algorithm(initial_interval.first, initial_interval.second);

    cout << root << endl;

    return 0;
}