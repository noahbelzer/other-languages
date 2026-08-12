/*
# Compile
g++ -DEIGEN_USE_BLAS -std=c++17 -O3 -march=native -funroll-loops -Wall -Wextra -fopenmp -o mat_vec_multiplication mat_vec_multiplication.cpp mat_utils.cpp -lopenblas

# Run
OPENBLAS_NUM_THREADS=1 ./mat_vec_multiplication
*/

#include "mat_vec_multiplication.h"
#include "mat_utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <eigen3/Eigen/Dense>
#include <omp.h>

std::vector<double> serial_familiar_definition(const std::vector<std::vector<double>>& A, const std::vector<double>& x, int m, int n) {
    /*
    Numerical Linear Algebra by Trefethen and Bau - pg. 3, eq. 1.1
    */
    
    std::vector<double> b(m, 0.0);
	
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            b[i] += A[i][j] * x[j];
        }
    }

    return b;    
}


std::vector<double> parallel_familiar_definition(const std::vector<std::vector<double>>& A, const std::vector<double>& x, int m, int n) {
    /*
    Numerical Linear Algebra by Trefethen and Bau - pg. 3, eq. 1.1
    */

    std::vector<double> b(m, 0.0);

    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            b[i] += A[i][j] * x[j];
        }
    }

    return b; 
}


std::vector<double> serial_linear_combo_definition(const std::vector<std::vector<double>>& A, const std::vector<double>& x, int m, int n) {
    /*
    Numerical Linear Algebra by Trefethen and Bau - pg. 4, eq. 1.2
    */

    std::vector<double> b(m, 0.0);

    for (int j = 0; j < n; j++) {
        for (int k = 0; k < m; k++) {
            b[k] += x[j] * A[k][j];
        }
    }

    return b;
}


std::vector<double> parallel_linear_combo_definition(const std::vector<std::vector<double>>& A, const std::vector<double>& x, int m, int n) {
    /*
    Numerical Linear Algebra by Trefethen and Bau - pg. 4, eq. 1.2
    */

    std::vector<double> b(m, 0.0);

    for (int j = 0; j < n; j++) {
        #pragma omp parallel for
        for (int k = 0; k < m; k++) {
            b[k] += x[j] * A[k][j];
        }
    }

    return b;
}


void get_dimensions(const std::vector<std::vector<double>>& A, int& m, int& n) {
    if (A.empty())
        throw std::runtime_error("Matrix is empty");
    m = static_cast<int>(A.size());
    n = static_cast<int>(A[0].size());
}


void run_program() {
    //std::vector<std::vector<double>> matrix = read_matrix_csv("sample_mat_1.csv");

    //print_matrix(matrix);

    // Test Case Vector:
    //std::vector<double> vector = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    //int m, n;
    //get_dimensions(matrix, m, n);
    //std::cout << "Rows: " << m << ", Cols: " << n << std::endl;

    int m = 1000, n = 1000;
    // Create the matrix
    std::vector<std::vector<double>> matrix(m, std::vector<double>(n, 0.0));
    // Create the vector
    std::vector<double> vec(n, 0.0);
    // 1. Obtain a random seed from the hardware
    std::random_device rd;
    // 2. Initialize the Mersenne Twister PRNG
    std::mt19937 gen(rd());
    // 3. Define the continuous range [1.0, 50.0]
    std::uniform_real_distribution<double> distrib(1.0, 50.0);
    // 4. Fill the vector and matrix with random numbers
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = distrib(gen);
        }
    }
    for (int i = 0; i < n; i++) {
        vec[i] = distrib(gen);
    }

    std::vector<double> result(m, 0.0);

    // Serial - Familar Definition - eq. 1.1
    double serial_start = omp_get_wtime();    
    result = serial_familiar_definition(matrix, vec, m, n);
    double serial_time = omp_get_wtime() - serial_start;
    //std::cout << "Result: serial_familar_definition" << std::endl;
    //print_vector(result);

    // Parallel - Familiar Definition - eq. 1.1
    double parallel_start = omp_get_wtime();
    result = parallel_familiar_definition(matrix, vec, m, n);
    double parallel_time = omp_get_wtime() - parallel_start;
    //std::cout << "Result: parallel_familar_definition" << std::endl;
    //print_vector(result);

    std::cout << "Familiar Definition" << std::endl;
    std::cout << "Threads:       " << omp_get_max_threads() << std::endl;
    std::cout << "Serial time:   " << serial_time   << " seconds" << std::endl;
    std::cout << "Parallel time: " << parallel_time << " seconds" << std::endl;
    std::cout << "Speedup:       " << serial_time / parallel_time << "x" << std::endl;

    // Serial - Linear Combination Definition - eq. 1.2
    serial_start = omp_get_wtime();
    result = serial_linear_combo_definition(matrix, vec, m, n);
    serial_time = omp_get_wtime() - serial_start;
    //std::cout << "Result: serial_linear_combo_definition" << std::endl;
    //print_vector(result);

    // Parallel - Linear Combination Definition - eq. 1.1
    parallel_start = omp_get_wtime();
    result = parallel_linear_combo_definition(matrix, vec, m, n);
    parallel_time = omp_get_wtime() - parallel_start;
    //std::cout << "Result: parallel_linear_combo_familar_definition" << std::endl;
    //print_vector(result);

    std::cout << "Linear Combination Definition" << std::endl;
    std::cout << "Threads:       " << omp_get_max_threads() << std::endl;
    std::cout << "Serial time:   " << serial_time   << " seconds" << std::endl;
    std::cout << "Parallel time: " << parallel_time << " seconds" << std::endl;
    std::cout << "Speedup:       " << serial_time / parallel_time << "x" << std::endl;

    
    // Eigen Matrix-Vector Multiplication (using BLAS under the hood)
    Eigen::MatrixXd eig_A(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            eig_A(i, j) = matrix[i][j];

    Eigen::VectorXd eig_x = Eigen::Map<Eigen::VectorXd>(vec.data(), n);

    parallel_start = omp_get_wtime();
    Eigen::VectorXd eig_result = eig_A * eig_x;
    parallel_time = omp_get_wtime() - parallel_start;
    std::cout << "Eigen" << std::endl;
    std::cout << "Parallel time: " << parallel_time << " seconds" << std::endl;

    // Sample Solution:
    // b[1] =164.0000
    // b[2] =197.0000
    // b[3] =315.0000
    // b[4] =242.0000
    // b[5] =225.0000
    // b[6] =210.0000
    // b[7] =104.0000
    // b[8] =334.0000
    // b[9] =320.0000
    // b[10] =261.0000
}


int main() {
    run_program();
    std::cout << "AGAIN -----------" << std::endl;
    run_program();
}
