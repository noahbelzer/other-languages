/*
Matrix Analysis Textbook probelm 3.9.1. (c) and (d): pg. 148, 749

Note: Partial pivoting (which we implemented) can lead to a different matrix P that is still valid
because both sequences of elementary row operations correspond to different nonsingular matrices P,
but it still satisfies PA = E_A.

In C, a "string" is a pointer to the first character (char*) of a null-terminated character array
When argv is of type char**, it indicates that it is a pointer to an array of elements of type char*, and each of those elements is a pointer to a C string.
Check: [char*, char*, char*, ...]
          |    
          V  
        ['1', 'a', ..., '\0']

Compile: g++ -Wall -Wextra -std=c++17 -o rank_normal rank_normal.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cstdlib>
#include <vector>
#include <string>

#define ROWS 3
#define COLS 4


struct Matrix {
    double *data;
    int rows;
    int cols;
    
    double* operator[](int i) {
        return data + i * cols;
    }
};


void print_matrix(const std::string& name, Matrix matrix) {
    printf("%s =\n", name.c_str());
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            printf("%.2f  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void gauss_jordan(int n, Matrix A, Matrix E_A) {

    std::string matrix_name = "Matrix";
    print_matrix(matrix_name, A);

    // Initialize arrays that will be used next
    // Temporary 1D arrays of length n to store matrix rows.
    std::vector<double> pivot_row(n);
    std::vector<double> row_to_swap(n);

    // Initial values
    int pivot_row_index = -1; 
    int pivot_col_index = -1;
    

    do {
        // Reset this whenever we run a new iteration
        double larger_num = 0.0;
        int larger_num_row_index = 0;

        // Initially, assume all values below the pivot are zero, unless we find a non-zero number.
        bool all_zeros = true;

        // If moving the pivot down 1 row is still within bounds, move it down 1 and to the right 1.
        pivot_row_index++;
        pivot_col_index++;

        double pivot = A[pivot_row_index][pivot_col_index];
        printf("Current Pivot: %.2f at %d, %d\n", pivot, pivot_row_index, pivot_col_index);
        printf("pivot_row_index = %d\n", pivot_row_index);

        // Check beneath the pivot (for partial pivoting), and find the largest number
        for (int i = pivot_row_index; i < ROWS; i++) {
            if (A[i][pivot_col_index] != 0.0) {
                all_zeros = false;
            }

            if (A[i][pivot_col_index] > pivot) {
                larger_num = A[i][pivot_col_index];
                larger_num_row_index = i;
            }
        }

        printf("Larger Num: %.2f\n", larger_num);
        printf("Larger Num Row Index: %d\n", larger_num_row_index);
        printf("Larger Number at: %d, %d\n", larger_num_row_index, pivot_col_index);

        // If all numbers in the pivot column are zero including the pivot itself, jump to the while condition again.
        if (all_zeros == true) {
            // Set the row back one because your do logic will increment this again. This will enable us to move one position to the right and retain the same row.
            pivot_row_index--;
            printf("Breaking the loop\n");
            continue;
        }

        // If larger_num is different from its initialized value: If we found a larger number underneath the original pivot, we will interchange rows (for partial pivoting).
        if (larger_num != 0) {
            // Swap the pivot row with the row containing the higehst number in the first col
            for (int j = 0; j < n; j++) {
                pivot_row[j] = A[pivot_row_index][j];
                row_to_swap[j] = A[larger_num_row_index][j];

                A[pivot_row_index][j] = row_to_swap[j];
                A[larger_num_row_index][j] = pivot_row[j];
            }

            // Reassign the pivot to adjust to this larger value that moved into the pivot position.
            printf("pivot_row_index = %d\n", pivot_row_index);
            printf("pivot_col_index = %d\n", pivot_col_index);
            pivot = A[pivot_row_index][pivot_col_index];
            printf("Pivot changed to: %.2f\n", pivot);

            print_matrix(matrix_name, A);
        }
        
        printf("pivot_row_index = %d\n", pivot_row_index);

        // Scale all numbers in the pivot row to make them 1: Divide all values in the new pivot row by the pivot
        for (int j = 0; j < n; j++) {
            printf("NUM: %.2f\n", A[pivot_row_index][j]);
            A[pivot_row_index][j] = (A[pivot_row_index][j] / pivot);
        }

        printf("Numbers scaled to 1:\n");
        print_matrix(matrix_name, A);

        // Make the numbers below and above the pivot zero
        for (int i = 0; i < ROWS; i++) {
            // If we are not on the pivot row, proceed.
            if (i != pivot_row_index) {
                double coefficient = A[i][pivot_col_index];
                // If the coefficient is not zero, proceed.
                if (coefficient != 0) {
                    for (int j = 0; j < n; j++) {
                        A[i][j] = A[i][j] - (coefficient * A[pivot_row_index][j]);
                    }
                }
            }
        }

        printf("Numbers above and below the pivot are now 0:\n");
        print_matrix(matrix_name, A);

    // Continue if moving the column index one to the right will still be within the bounds of A.
    } while (pivot_col_index + 1 < COLS);

    // Now, we have [E_A | P].
    // Store matrix E_A.
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            E_A[i][j] = A[i][j];
        }
    }

    // Print matrix E_A.
    matrix_name = "Matrix E_A";
    print_matrix(matrix_name, E_A);

    // Store matrix P.
    // P is the product of the elementary matrices corresponsding to the operations used in the reduction, and PA = E_A.
    int min_dim = (ROWS < COLS) ? ROWS : COLS;
    std::vector<double> matrix_P_data(min_dim * min_dim);
    Matrix matrix_P = {matrix_P_data.data(), min_dim, min_dim};
    
    for (int i = 0; i < min_dim; i++) {
        for (int j = 0; j < min_dim; j++) {
            matrix_P[i][j] = A[i][COLS + j];
        }
    }

    // Print matrix P
    matrix_name = "Matrix [E_A over I_4]";
    print_matrix(matrix_name, matrix_P);
}


void column_reduction(int total_rows, Matrix matrix, Matrix Q) {

    // Print matrix [E_A over I_4].
    std::string matrix_name = "Matrix [E_A over I_4]";
    print_matrix(matrix_name, matrix);

    // Initialize arrays that will be used next
    std::vector<double> pivot_col(total_rows);
    std::vector<double> col_to_swap(total_rows);

    // Initial values start at position 0,0 becuase the first column of E_A will be 1
    //                                                                              0
    //                                                                              0
    //                                                                              .
    //                                                                              .
    //  
    // So, inside the do statement, we will increment the pivot to position 1,1 (which is the next diagonal entry)          
    int pivot_row_index = 0; 
    int pivot_col_index = 0;
    

    do {
        // Reset this whenever we run a new iteration
        int larger_num_col_index = 0;

        // Initially, assume all values below the pivot are zero, unless we find a non-zero number.
        bool all_zeros = true;

        // If moving the pivot down 1 row is still within bounds, move it down 1 and to the right 1.
        pivot_row_index++;
        pivot_col_index++;

        double pivot = matrix[pivot_row_index][pivot_col_index];
        printf("Current Pivot: %.2f at %d, %d\n", pivot, pivot_row_index, pivot_col_index);
        printf("pivot_row_index = %d\n", pivot_row_index);

        // If the pivot is one, then we break the loop and move the pivot to the next diagonal entry
        if (pivot == 1.0) {
            printf("Breaking the loop\n");
            continue;

          
        } else {
            // If the pivot is 0
            for (int j = pivot_col_index; j < COLS; j++) {
                if (matrix[pivot_row_index][j] != 0.0) {
                    all_zeros = false;
                    larger_num_col_index = j;
                }
            }

        }
        
        // If all numbers in the pivot row are zeros including the pivot itself, then we should already have N_r.
        if (all_zeros == true) {
            printf("Exit the loop entirely\n");
            break;
        }

        
        // Swap the pivot column with the column containing the 1
        for (int i = 0; i < total_rows; i++) {
            pivot_col[i] = matrix[i][pivot_col_index];
            col_to_swap[i] = matrix[i][larger_num_col_index];

            matrix[i][pivot_col_index] = col_to_swap[i];
            matrix[i][larger_num_col_index] = pivot_col[i];
        }

        // Reassign the pivot to adjust to this larger value that moved into the pivot position.
        printf("pivot_row_index = %d\n", pivot_row_index);
        printf("pivot_col_index = %d\n", pivot_col_index);
        pivot = matrix[pivot_row_index][pivot_col_index];
        printf("Pivot changed to: %.2f\n", pivot);

        print_matrix(matrix_name, matrix);
        printf("pivot_row_index = %d\n", pivot_row_index);

    // Continue if moving the column index one to the right will still be within the bounds of the matrix.
    } while (pivot_col_index + 1 < COLS);



    // Make the non-zero numbers in the non-pivot positions zero, if they exist.
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // If there is a number in a row that is greater that 1 (except for the pivot)...
            if (i != j) {
                double non_zero_num = matrix[i][j];
                if (non_zero_num > 1) {
                    
                    // Swap the column containing the non-zero value with the column that has a 1 on the same row.
                    for (int k = 0; k < total_rows; k++) {
                        
                        // We use i at the end here because if there are non-zero numbers on the first row, we subtract by a multiple of the first column. If there are non-zero numbers on the second row, we subtract by a multiple of the second column, etc.
                        printf("matrix[k][j] = %.2f - (%.2f * %.2f)\n", matrix[k][j], non_zero_num, matrix[k][i]);
                        matrix[k][j] = matrix[k][j] - (non_zero_num * matrix[k][i]);
                    }
                }
            }
        }
    }

    printf("Non-pivot numbers are now 0:\n");
    print_matrix(matrix_name, matrix);

    // Now, we have [E_A over I_4].
    // Store matrix Q.
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("matrix[i + ROWS][j] = %.2f\n", matrix[i + ROWS][j]);
            Q[i][j] = matrix[i + ROWS][j];
        }
    }
}



int main(int argc, char** argv) {
    /*
    Steps:
    1. Define the matrix A, and define its rows and columns as constants. For this code, we assume that Column 1 is not all zeros.
    2. Find a nonsingular matrix P such that PA = E_A.
    3. Find nonsingular matrices P and Q such that PAQ is in rank normal form.
    */

    printf("Number of arguments: %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("Arguments: %s\n", argv[i]);
    }
    printf("\n");



    double matrix_A[ROWS][COLS] = {
        {1, 2, 3, 4},
        {2, 4, 6, 7},
        {1, 2, 3, 6}
    };

    // Initialize matrix E_A with arbitrary values.
    double E_A_data[ROWS][COLS] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    // [0][0] [0][1] [0][2] [0][3] [1][0] [1][1] [1][2] [1][3] [2][0] ...
    //   ^
    //   &E_A_data[0][0] points here, and Matrix strides forward from here

    Matrix E_A = {&E_A_data[0][0], ROWS, COLS};

    // Determine the number of columns of [A|I].
    // I is m x m, so the total length of [A|I] is the number of columns in A plus the number of rows in A.
    int n = COLS + ROWS;
   
    // Allocate a 2D array with ROWS rows and n columns dynamically.
    std::vector<double> A_aug_I_data(ROWS * n);
    Matrix A_aug_I = {A_aug_I_data.data(), ROWS, n};

    // Populate the matrix A in [A|I]
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            A_aug_I[i][j] = matrix_A[i][j];
        }
    }

    // Populate the matrix I in [A|I]
    for (int i = 0; i < ROWS; i++) {
        for (int j = COLS; j < n; j++) {
            // Fill diagonal entries with 1, and other entries with 0.
            if ((i + COLS) == j) {
                A_aug_I[i][j] = 1.0;
            } else {
                A_aug_I[i][j] = 0.0;
            }
        }
    }

    // Find P such that PA = E_A.
    // [A|I] -> Gauss-Jordan -> [E_A|P]
    gauss_jordan(n, A_aug_I, E_A);
    
    // Find the max dimension (rows or columns) to determine how many total rows we need, which is ROWS + max_dim.
    // Example of matrix [E_A over I_4]:
    // 1 2 3 0
    // 0 0 0 1
    // 0 0 0 0
    // -------
    // 1 0 0 0
    // 0 1 0 0 
    // 0 0 1 0
    // 0 0 0 1
    int max_dim = (ROWS > COLS) ? ROWS : COLS;
    int total_rows = (ROWS + max_dim);
    std::vector<double> E_A_aug_I_data(total_rows * COLS);
    Matrix E_A_aug_I = {E_A_aug_I_data.data(), total_rows, COLS};

    std::vector<double> Q_data(COLS * COLS);
    Matrix Q = {Q_data.data(), COLS, COLS};

    // Populate the matrix E_A in [E_A over I_4].
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            E_A_aug_I[i][j] = E_A[i][j];
        }
    }

    // Populate the matrix I in [E_A over I_4].
    for (int i = ROWS; i < total_rows; i++) {
        for (int j = 0; j < COLS; j++) {
            // Fill diagonal entries with 1, and other entries with 0.
            if ((i - ROWS) == j) {
                E_A_aug_I[i][j] = 1.0;
            } else {
                E_A_aug_I[i][j] = 0.0;
            }
        }
    }

    // Find Q such that E_A * Q = N_r, by column reducing E_A.
    column_reduction(total_rows, E_A_aug_I, Q);
    

    // Print matrix Q.
    std::string matrix_name = "Matrix Q";
    print_matrix(matrix_name, Q);

    return 0;
}