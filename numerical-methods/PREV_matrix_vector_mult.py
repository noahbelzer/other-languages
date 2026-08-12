# Previously ran:
# sudo apt install python3-numpy
# sudo apt install libopenmpi-dev openmpi-bin
# sudo pip install mpi4py omp4py --break-system-packages
# sudo apt update && sudo apt install python3-psutil

# I installed this globally to avoid problems with a virtual environment pushing to GitHub.

# Numerical Linear Algebra by Trefethen and Bau: pages 3-4
# Run hybird MPI + OMP: OMP_NUM_THREADS=4 mpiexec -n 4 --oversubscribe python3 matrix_vector_mult.py

# DEFAULT COMMAND -----> Run OMP: python3 matrix_vector_mult.py
# Explicitly modify OMP: OMP_NUM_THREADS=4 python3 matrix_vector_mult.py

import time
import random
import psutil
import threading
import numpy as np
from mpi4py import MPI
from omp4py import *

def log_cpu():
    while monitoring:
        print(f"CPU: {psutil.cpu_percent(interval=0.5, percpu=True)}")


@omp
def familar_definition(m, n, A, x):
    # pg. 3, eq. 1.1
    # Answer: [55, 130, 205, 280, 355]

    b_m = []
    for i in range(m):
        b_m.append(0)

    summation = 0

    with omp("parallel for"):
        for i in range(m):
            summation = 0

            with omp("for reduction(+:summation)"):
                for j in range(n):
                    summation += (A[i, j] * x[j])

            b_m[i] = summation

    #print(b_m)


def serial_familar_definition(m, n, A, x):
    # pg. 3, eq. 1.1
    # Answer: [55, 130, 205, 280, 355]

    b_m = []
    for i in range(m):
        b_m.append(0)

    sum = 0

    for i in range(m):
        sum = 0
        for j in range(n):
            sum += (A[i, j] * x[j])
        b_m[i] = sum

    #print(b_m)


def matrix_times_a_vector(m, n, A, x):
    # pg. 4, eq. 1.2
    # Answer: [55, 130, 205, 280, 355]

    b_m = []
    for i in range(m):
        b_m.append(0)

    
    summand = []
    x_j_times_a_j = []

    sum = 0

    # Loop in book
    for j in range(n):

        # Our loop to multiply x_j by each elem in the column A_j
        for k in range(m):
            summand.append(x[j] * A[k, j])

        # Add the whole column to this x_j_times_a_j
        x_j_times_a_j.append(summand)
        summand = []

    new_sum = 0

    # x_j_times_a_j is n x m because it is stored as the transpose due to appending 1S arrays
    for j in range(m):
        for i in range(n):
            new_sum += x_j_times_a_j[i, j]

        b_m[j] = new_sum
        new_sum = 0

    print(b_m)
       


if __name__ == '__main__':

    m = 5
    n = 5

    A_mxn = [
        [1,   2,  3,  4,  5],
        [6,   7,  8,  9, 10],
        [11, 12, 13, 14, 15],
        [16, 17, 18, 19, 20],
        [21, 22, 23, 24, 25]
    ]

    x_n = [1, 2, 3, 4, 5]




    m = 10000
    n = 10000
    A_mxn = np.random.random((10000, 10000))
    x_n = np.random.random(10000)

    monitoring = True
    t = threading.Thread(target=log_cpu)
    t.start()

    '''
    start = time.perf_counter()
    serial_familar_definition(m, n, A_mxn, x_n)
    elapsed = time.perf_counter() - start
    print(f"Serial Elapsed: {elapsed:.6f}s")
    '''

    start = time.perf_counter()
    familar_definition(m, n, A_mxn, x_n)
    elapsed = time.perf_counter() - start
    print(f"Parallel Elapsed: {elapsed:.6f}s")
    
    # Results:
    # Serial Elapsed: 18.414874s
    # Parallel Elapsed: 7.382327s
    # So, a solid ~2.5x speedup.

    monitoring = False
    t.join()
    
    #matrix_times_a_vector(m, n, A_mxn, x_n)
