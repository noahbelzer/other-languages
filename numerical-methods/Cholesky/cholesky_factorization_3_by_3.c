/*
Compile:
gcc -Wall -Wextra -Wpedantic -std=c17 -O2 -g cholesky_factorization_3_by_3.c -o cholesky_factorization_3_by_3 -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int row;
    int col;
    double val;
} Entry;


void print_matrix(const char *name, int rows, int cols, Entry *matrix) {
    printf("%s =\n", name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.2f ", matrix[i + j * rows].val);
        }
        printf("\n");
    }
    printf("\n");

    printf("Details:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Row: %d, Col: %d, Val: %8.2f \n", matrix[i + j * rows].row, matrix[i + j * rows].col, matrix[i + j * rows].val);
        }
        printf("\n");
    }
}


void fill_matrix(int rows, int cols, Entry *matrix) {

    // --------- LL^T = A ---------
    // L: Lower Triangular
    Entry *L = malloc(rows * cols * sizeof(Entry));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Entry l;
            l.row = i;
            l.col = j;

            if (i >= j) {
                // Initialize the l values in the lower triangular part of the matrix as NAN
                l.val = NAN;
            } else {
                l.val = 0;
            }
                L[i + j * rows] = l;
        }
    }

    char *name = "L";
    print_matrix(name, rows, cols, L);

    // L^T: Upper Triangular
    Entry *L_Transpose = malloc(rows * cols * sizeof(Entry));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            L_Transpose[i + j * rows] = L[j + i * rows];
        }
    }

    char *new_name = "L^T";
    print_matrix(new_name, rows, cols, L_Transpose);

}

/*
Older C++ code:
vector<vector<int>> Multiplication(const vector<vector<int>>& matrix_A, const vector<vector<int>>& matrix_B) {
    int dot_product = 0;

    // matrix_C has the same number of rows as matrix_A
    vector<vector<int>> matrix_C(matrix_A.size());
    
    // Loop over the number of rows in A
    for (int i = 0; i < matrix_A.size(); ++i) {

        // Loop over the number of cols in B
        for (int k = 0; k < matrix_B[0].size(); ++k) {
            dot_product = 0;

            // Loop over the number of rows in B. Remember, the number of cols in A must match the number of rows in B.
            for (int j = 0; j < matrix_B.size(); ++j) {
                dot_product += matrix_A[i][j] * matrix_B[j][k]; 
            }

            // C has the same number of rows as A, so we can use i, which counts the number of rows in A
            cout << "Dot Product for C row " << i << ": " << dot_product << endl;
            matrix_C[i].push_back(dot_product);
            PrintMatrix(matrix_C);
        }
    }

    return matrix_C;
}


find_L
*/

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("Arg %d: %s ", i, argv[i]);
    }
    printf("\n\n");

    // Since your .bin file was written column-major (with order='F' in Python), you must always use i + j * rows when reading it in C.
    FILE *file = fopen("matrix.bin", "rb");

    int rows;
    int cols;

    fread(&rows, sizeof(int), 1, file);
    fread(&cols, sizeof(int), 1, file);

    // read raw doubles into temp buffer
    double *temp = malloc(rows * cols * sizeof(double));
    fread(temp, sizeof(double), rows * cols, file);

    Entry *matrix_A = malloc(rows * cols * sizeof(Entry));

    fread(matrix_A, sizeof(Entry), rows * cols, file);

    // Use indexing for column-major
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix_A[i + j * rows].row = i;
            matrix_A[i + j * rows].col = j;
            matrix_A[i + j * rows].val = temp[i + j * rows];
        }
    }

    char* A_name = "A";
    print_matrix(A_name, rows, cols, matrix_A);

    fill_matrix(rows, cols, matrix_A);

    fclose(file);
    free(matrix_A);
    return 0;
}