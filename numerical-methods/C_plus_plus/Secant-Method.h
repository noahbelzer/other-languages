#ifndef SECANT_METHOD
#define SECANT_METHOD

#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// Function declarations (prototypes)
double f(double x);
double Secant_Method_Algorithm(double x_n, double x_n_minus_1);
pair<double, double> Initial_Points();

#endif 