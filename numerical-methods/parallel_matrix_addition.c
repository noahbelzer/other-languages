// Compile and run the script:
// mpicc -o parallel_matrix_addition parallel_matrix_addition.c
// mpirun -np 4 ./parallel_matrix_addition

#include <stdio.h>
#include <mpi.h>

#define ROWS 4
#define COLUMNS 4

int matrix_addition(int A[ROWS][COLUMNS], int B[ROWS][COLUMNS], int i) {
	int local_sum = 0;

	// Each process does a separate row, so rather than an outer loop, we just have i = rank
	
	for (int j = 0; j < COLUMNS; j++) {
		local_sum += A[i][j] + B[i][j];
	}

	return local_sum;
}

int main(int argc, char* argv[]) {
    int my_rank, p;
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

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

	int local_sum = matrix_addition(matrix_A, matrix_B, my_rank);
	int total_sum = 0;

    // Combine all local sums into total_sum on rank 0
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("The sum of all elements in (A + B) is %d\n", total_sum);
    }

    MPI_Finalize();

	return 0;
}
