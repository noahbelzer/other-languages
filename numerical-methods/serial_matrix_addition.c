// Compile command: gcc -Wall serial_matrix_addition.c -o serial_matrix_addition
// Run: ./serial_matrix_addition

#include <stdio.h>

#define ROWS 4
#define COLUMNS 4

int matrix_addition(int A[ROWS][COLUMNS], int B[ROWS][COLUMNS]) {
	int cumulative_sum = 0;

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < ROWS; j++) {
			cumulative_sum += A[i][j] + B[i][j];
		}
	}

	return cumulative_sum;
}

int main() {
	int matrix_A[ROWS][COLUMNS] =
	{{1, 0, 2, 1},
	 {6, 1, 4, 0},
	 {3, 5, 9, 2},
	 {0, 8, 7, 4}};

	int matrix_B[ROWS][COLUMNS] =
	{{1, 7, 3, 0},
	 {0, 2, 6, 5},
	 {0, 5, 5, 1},
	 {2, 4, 0, 9}};

	int result = matrix_addition(matrix_A, matrix_B);

	printf("The result of matrix A added to matrix B is %d\n", result);

	return 0;
}
