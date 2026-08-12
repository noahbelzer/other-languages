# Previously done: pip install numba --break-system-packages

from numba import njit, prange
import numpy as np
import os

def read_matrix(filename: str, m: int, n: int) -> np.ndarray:
    A = np.zeros((m, n))
    with open(filename, 'r') as f:
        for i in range(m):
            A[i, :] = list(map(float, f.readline().split()))
    return A


@njit(parallel=True)
def familiar_mat_vec_mult(m, n, b, A, x):
    # Numerical Linear Algebra by Trefethen and Bau - pg. 3, eq. 1.1
    for i in prange(0, m):
        sum = 0

        for j in range(0, n):
            sum += A[i,j] * x[j]

        b[i] = sum


def serial_familiar_mat_vec_mult(m, n, b, A, x):
    # Numerical Linear Algebra by Trefethen and Bau - pg. 3, eq. 1.1
    for i in range(0, m):
        sum = 0

        for j in range(0, n):
            sum += A[i,j] * x[j]

        b[i] = sum


@njit(parallel=True)
def linear_combo_mat_vec_mult(m, n, b, A, x):
    # Numerical Linear Algebra by Trefethen and Bau - pg. 4, eq. 1.2
    b.fill(0.0)

    for j in prange(0, n):
        b += x[j] * A[:,j]         # Not contiguous in memory, but this is how it looked in the book


def serial_linear_combo_mat_vec_mult(m, n, b, A, x):
    # Numerical Linear Algebra by Trefethen and Bau - pg. 4, eq. 1.2
    b.fill(0.0)

    for j in prange(0, n):
        b += x[j] * A[:,j]         # Not contiguous in memory, but this is how it looked in the book


def print_results(m, b1, b2, b3, b4, b5):
    for i in range(0, m):
        print("serial_familiar: b[", i, "] =", b1[i])
    print()

    for i in range(0, m):   
        print("familiar: b[", i, "] =", b2[i])
    print()

    for i in range(0, m):    
        print("serial_linear_combo: b[", i, "] =", b3[i])
    print()

    for i in range(0, m):    
        print("linear_combo: b[", i, "] =", b4[i])
    print()

    for i in range(0, m):    
        print("NumPy BLAS: b[", i, "] =", b5[i])
    print()


if __name__ == "__main__":
    m = 10
    n = 10

    b1 = np.empty(m)
    b2 = np.empty(m)
    b3 = np.empty(m)
    b4 = np.empty(m)
    b5 = np.empty(m)

    x = np.array([1,2,3,4,5,6,7,8,9,10])

    A = read_matrix('mat_vec_product_data.txt', m, n)

    # Call the functions
    serial_familiar_mat_vec_mult(m, n, b3, A, x)
    familiar_mat_vec_mult(m, n, b4, A, x)
    serial_linear_combo_mat_vec_mult(m, n, b1, A, x)
    linear_combo_mat_vec_mult(m, n, b2, A, x)
    b5 = A @ x

    print_results(m, b1, b2, b3, b4, b5)

    # Solution:
    # b[1] =164.0000
    # b[2] =197.0000
    # b[3] =315.0000
    # b[4] =242.0000
    # b[5] =225.0000
    # b[6] =210.0000
    # b[7] =104.0000
    # b[8] =334.0000
    # b[9] =320.0000
    # b[10] =261.0000
