# Previously done: pip install mpi4py
# Use the virtual environment: source deep_learning_1/bin/activate
# Run: mpiexec -n 4 --oversubscribe python3 LU_fact.py

from mpi4py import MPI

N = 3

'''
comm = MPI.COMM_WORLD      # The global communicator
rank = comm.Get_rank()     # This process's ID (0, 1, 2, ...)
size = comm.Get_size()     # Total number of processes

print(f"Hello from process {rank} of {size}")

if rank == 0:
    # Rank 0 is conventionally the "master" process
    print(f"\nMaster process here. We have {size} workers total.")

if rank == 1:
    print("Rank 1 here")
'''

def create_L(A): 
    matrix_L = [[0] * N for _ in range(N)]

    for i in range(N):
        for j in range(N):
            if (i == j):
                matrix_L[i][j] = 1
        
    for i in range(N):
        for j in range(N):
            if (j < N - 1):
                print(matrix_L[i][j], end=" ")
            else:
                print(matrix_L[i][j])


if __name__ == '__main__':

    matrix_A = [
        [2,  2,  2],
        [4,  7,  7],
        [6, 18, 22]
    ]

    create_L(matrix_A)