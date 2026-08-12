# Previously ran:
# sudo apt install python3-numpy
# sudo apt install libopenmpi-dev openmpi-bin
# sudo pip install mpi4py omp4py --break-system-packages
# sudo apt update && sudo apt install python3-psutil

# I installed this globally to avoid problems with a virtual environment pushing to GitHub.

# Numerical Linear Algebra by Trefethen and Bau: pages 3-4

# OLD OMP ONLY COMMAND -----> Run OMP: python3 matrix_vector_mult.py
# Explicitly modify OMP: OMP_NUM_THREADS=4 python3 matrix_vector_mult.py

# -------------------------------------------------------------------------------

# CURRENT COMMAND for Run hybird MPI + OMP:
# OMP_NUM_THREADS=2 mpiexec -n 2 --use-hwthread-cpus python3 matrix_vector_mult.py

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
def familar_definition(starting, ending, A, x):

    b_m = []
    for i in range(ending - starting + 1):
        b_m.append(0)

    summation = 0

    with omp("parallel for"):
        for i in range(ending - starting + 1):
            summation = 0

            with omp("for reduction(+:summation)"):
                for j in range(A.shape[1]):
                    summation += (A[i + starting, j] * x[j])

            b_m[i] = summation

    return b_m


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

    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    print(f"Hello from process {rank} of {size}")

    if rank == 0:
        monitoring = True
        t = threading.Thread(target=log_cpu)
        t.start()

    if rank == 0:
        A_mxn = np.random.random((12000, 12000))
        x_n = np.random.random(12000)
        chunks = [A_mxn[0:6000], A_mxn[6000:12000]]  # one chunk per rank
    else:
        chunks = None
        x_n = None

    local_A = comm.scatter(chunks, root=0)
    x_n = comm.bcast(x_n, root=0)

    start = time.perf_counter()

    # both ranks do work now
    b = familar_definition(0, 5999, local_A, x_n)

    all_results = comm.gather(b, root=0)

    if rank == 0:
        combined = np.concatenate(all_results)
        elapsed = time.perf_counter() - start
        print(f"Parallel Elapsed: {elapsed:.6f}s")

        monitoring = False
        t.join()

        # NumPy comparison
        A = np.random.random((12000, 12000))
        x = np.random.random(12000)
        start = time.perf_counter()
        b = A @ x
        elapsed = time.perf_counter() - start
        print(f"NumPy Elapsed: {elapsed:.6f}s")
    
    #matrix_times_a_vector(m, n, A_mxn, x_n)
