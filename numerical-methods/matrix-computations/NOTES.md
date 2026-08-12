# Matrix-Vector Multiplication: Notes & Benchmarks

Implementation and benchmarking of matrix-vector multiplication algorithms from:
> *Numerical Linear Algebra* — Trefethen & Bau

---

## Algorithms Implemented

### Familiar Definition (pg. 3, eq. 1.1)
Computes each element of the result vector as a dot product of a matrix row with the input vector:

```
b[i] = sum over j of A[i][j] * x[j]
```

### Linear Combination Definition (pg. 4, eq. 1.2)
Computes the result vector as a linear combination of matrix columns scaled by the input vector:

```
b = sum over j of x[j] * A[:,j]
```

---

## Implementations

Each algorithm was implemented in four ways:

- **Serial** — direct translation of the algorithm with no parallelism
- **Parallel** — OpenMP parallelization via `#pragma omp parallel for`
- **Eigen + OpenBLAS** — reference benchmark using Eigen's matrix-vector multiply backed by OpenBLAS

---

## Benchmark Results

**Hardware:** 2 physical cores, 4 threads (hyperthreading)
**Matrix size:** m = n = 1000
**Data type:** `double`
**Compiler flags:** `-std=c++17 -O3 -march=native -funroll-loops -fopenmp`

| Implementation          | Time (seconds) | Speedup vs Serial |
|-------------------------|----------------|-------------------|
| Serial familiar         | 0.000769       | 1.00x (baseline)  |
| Parallel familiar       | 0.000242       | 3.18x             |
| Serial linear combo     | 0.002925       | 1.00x (baseline)  |
| Parallel linear combo   | 0.001489       | 1.96x             |
| Eigen + OpenBLAS        | 0.000372       | —                 |

---

## Key Observations

**Parallel familiar definition outperforms Eigen + OpenBLAS** at this matrix size. This is
due to the row-major memory layout of `std::vector<std::vector<double>>` being well-suited
to the familiar definition's row-wise access pattern, resulting in good cache utilization.

**Linear combination is slower** than the familiar definition in both serial and parallel
cases. The linear combination accesses matrix columns, which are non-contiguous in
row-major storage, causing more cache misses. The parallel version also has additional
overhead from spawning threads inside the outer loop (once per column, 1000 times total).

**Speedups are realistic for the hardware.** With only 2 physical cores and hyperthreading,
the theoretical ceiling is around 1.5–2x from hyperthreading alone. The familiar definition
exceeds this at 3.18x due to favorable cache behavior.

---

## OpenBLAS / OpenMP Thread Conflict

When linking against OpenBLAS with `-lopenblas`, OpenBLAS spawns its own internal threads
which conflict with OpenMP threads, causing severe slowdowns. The fix is to restrict
OpenBLAS to a single thread at runtime:

```bash
OPENBLAS_NUM_THREADS=1 ./mat_vec_multiplication
```

This lets OpenMP manage all parallelism while preventing OpenBLAS from spawning competing
threads. Setting this inline (rather than via `export`) avoids polluting the shell
environment.

---

## Compilation

```bash
g++ -DEIGEN_USE_BLAS -std=c++17 -O3 -march=native -funroll-loops -Wall -Wextra \
    -fopenmp -o mat_vec_multiplication mat_vec_multiplication.cpp mat_utils.cpp -lopenblas
```

## Run

```bash
OPENBLAS_NUM_THREADS=1 ./mat_vec_multiplication
```

Familiar Definition
Threads:       4
Serial time:   0.000769246 seconds
Parallel time: 0.000242059 seconds
Speedup:       3.17793x
Linear Combination Definition
Threads:       4
Serial time:   0.00292452 seconds
Parallel time: 0.00148945 seconds
Speedup:       1.96349x
Eigen
Parallel time: 0.000372143 seconds
