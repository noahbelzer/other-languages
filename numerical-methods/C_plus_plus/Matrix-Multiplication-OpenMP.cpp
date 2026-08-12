/*
cols_in_A, which must be equal to rows_in_B, could be huge while rows_in_A and cols_in_B are small, like 3.
So, the resulting matrix could be 3 by 3, but the calculation could benefit from OpenMP parallelization.

Compile:
    g++ -fopenmp Matrix-Multiplication-OpenMP.cpp -o Matrix-Multiplication-OpenMP
    ./Matrix-Multiplication-OpenMP
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
    int rows_in_A = matrix_A.size();
    int cols_in_B = matrix_B[0].size();
    int rows_in_B = matrix_B.size();

    // matrix_C has the same number of rows as matrix_A
    vector<vector<int>> matrix_C(matrix_A.size(), vector<int>(matrix_B[0].size()));
    
    // Loop over the number of rows in A
    for (size_t i = 0; i < rows_in_A; ++i) {

        // Loop over the number of cols in B
        for (size_t k = 0; k < cols_in_B; ++k) {
            int dot_product = 0;

            // Loop over the number of rows in B. Remember, the number of cols in A must match the number of rows in B.
            /*
            ===== Why reduction(+:dot_product)? =====:
            dot_product is shared by default, because it’s declared outside the loop.
            Multiple threads try to write to the same memory location at the same time → results are unpredictable.
            This is called a data race.

            reduction tells OpenMP:
            “Make a private copy of this variable for each thread, perform the operation locally, and then combine the results at the end.”

            Syntax:
            #pragma omp parallel for reduction(+:dot_product)

            +:dot_product → the operation is addition (+), which will combine all thread-local copies into the original variable after the loop finishes.
            Each thread keeps its own private dot_product, sums locally, then OpenMP safely adds them together at the end.

            Step-by-step behavior
            Each thread gets a private copy of dot_product initialized to 0 (the neutral value for +).
            Each thread computes its part of the sum:
            thread0: sum0
            thread1: sum1
            thread2: sum2

            After the loop, OpenMP adds them together:
            dot_product = sum0 + sum1 + sum2

            No thread ever writes to another thread’s memory during the loop → no data race.

            At the end of this loop, the outer dot_product variable is modified once.


            ===== Here’s what happens internally =====:
            OpenMP creates one private copy of dot_product per thread.
            Each thread starts its copy at 0 (neutral value for +).
            Threads execute iterations of j in parallel, updating their private copy only.

            At the end of the loop, OpenMP does a single combine step:
            dot_product = sum(thread0_dot + thread1_dot + thread2_dot + ...)

            Only one write to the “outer” dot_product occurs at the very end.
            This avoids data races.
            */
            #pragma omp parallel for reduction(+:dot_product)
            for (size_t j = 0; j < rows_in_B; ++j) {
                dot_product += matrix_A[i][j] * matrix_B[j][k]; 
            }

            // C has the same number of rows as A, so we can use i, which counts the number of rows in A
            //cout << "Dot Product for C row " << i << ": " << dot_product << endl;
            matrix_C[i][k] = dot_product;
            //PrintMatrix(matrix_C);
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

    /* Solution:
    11 23   2
    -7 20   8
    21 61 -13
    */
    vector<vector<int>> result = Multiplication(matrix_1, matrix_2);
    PrintMatrix(result);

    return 0;
}

/*
Learn:

====== Context ======
1️⃣ #pragma omp parallel for

This is the standard parallelization pragma:

#pragma omp parallel for
for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
        work(i, j);
    }
}


What happens:

OpenMP only splits the outer loop (i) across threads.

Each thread gets a subset of i values.

Inner loop (j) runs serially inside each thread.

This is usually safe and simple, especially if M is small compared to the number of threads.



2️⃣ #pragma omp parallel for collapse(2)

The collapse(n) clause flattens n nested loops into a single loop and distributes all iterations evenly across threads:

#pragma omp parallel for collapse(2)
for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++)
        work(i, j);


What happens:

OpenMP treats the i × j iteration space as one big 1D loop of size N * M.

Iterations are divided more evenly among threads, which can improve load balancing if M is large or N is small.



1️⃣ Why collapse(2) can help

Imagine your nested loops like this:

for (int i = 0; i < 4; i++)      // outer loop
    for (int j = 0; j < 1000; j++)  // inner loop
        work(i, j);

Scenario with #pragma omp parallel for only:

OpenMP splits the outer loop (i) across threads.

Suppose 4 threads: thread 0 → i=0, thread 1 → i=1, etc.

Each thread does all 1000 iterations of j serially.

Problem: threads finish at different times if work(i,j) is uneven or i small → poor load balancing.

Scenario with collapse(2):
#pragma omp parallel for collapse(2)
for (int i = 0; i < 4; i++)
    for (int j = 0; j < 1000; j++)
        work(i, j);


OpenMP treats i × j as one big 1D loop (size 4*1000 = 4000).

Iterations are evenly split across threads, e.g., 1000 iterations per thread if 4 threads.

✅ Much better load balancing.






===== If rows_in_B is huge, but rows_in_A and cols_in_B are small =====

#pragma omp parallel for
for (size_t i = 0; i < rows_in_A; ++i) {  // typically small, e.g., 3
    for (size_t k = 0; k < cols_in_B; ++k) {  // also small, e.g., 3
        int dot_product = 0;
        for (size_t j = 0; j < rows_in_B; ++j) {  // could be huge
            dot_product += matrix_A[i][j] * matrix_B[j][k];
        }
        matrix_C[i][k] = dot_product;
    }
}
Key observations
#pragma omp parallel for is applied only to the outer loop (i).

rows_in_A and cols_in_B are small (3).

This means only 3 iterations in the outer loop.

If you have, say, 8 threads, 5 threads will sit idle, because only 3 iterations are distributed.

rows_in_B is huge (e.g., millions).

But the inner loop over j is serial for each thread.

This is where most of the work happens, but OpenMP isn’t parallelizing it.

✅ The code is correct, but it’s not efficiently parallelized. Most CPU cores won’t be fully used.


2️⃣ What you should do for this case

Since outer loops are small and inner loop is huge, the most efficient approach is:

Option 1: Parallelize the inner loop manually
for (size_t i = 0; i < rows_in_A; ++i) {
    for (size_t k = 0; k < cols_in_B; ++k) {
        int dot_product = 0;

        #pragma omp parallel for reduction(+:dot_product)
        for (size_t j = 0; j < rows_in_B; ++j) {
            dot_product += matrix_A[i][j] * matrix_B[j][k];
        }

        matrix_C[i][k] = dot_product;
    }
}


✅ Benefits:

All threads are now working on the huge j loop, fully utilizing CPU.

dot_product is safely reduced across threads.

Works even when rows_in_A and cols_in_B are small.
*/