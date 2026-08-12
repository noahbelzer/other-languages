# Design Notes

## Matrix Struct and Memory Layout

The `Matrix` struct is a flat `Vec<f64>` with explicit `rows` and `cols` fields:

```rust
pub struct Matrix {
    pub rows: usize,
    pub cols: usize,
    pub data: Vec<f64>,
}
```

This is the standard representation. A `Vec<Vec<f64>>` set-up is not ideal because nested vectors scatter rows across the heap with no guaranteed contiguity, which can hinder cache performance. A flat buffer with explicit index arithmetic is what BLAS and LAPACK use internally.

### Row-Major Storage

The `Matrix` struct defaults to **row-major order**, meaning element `(i, j)` lives at `data[i * cols + j]`. Row `i` occupies the contiguous slice `data[i * cols .. (i+1) * cols]`.

```rust
pub fn get(&self, i: usize, j: usize) -> f64 {
    self.data[i * self.cols + j]
}
```

This choice is deliberate. Textbooks like *Numerical Linear Algebra* by Trefethen and Bau write algorithms in standard mathematical notation indexed by rows and columns. With row-major storage, translating eq. 1.1 directly from the book produces cache-friendly code:

```rust
// b[i] = sum_j A[i,j] * x[j]  — inner loop is contiguous in memory
for i in 0..a.rows {
    for j in 0..a.cols {
        result[i] += a.get(i, j) * x[j];
    }
}
```

The inner `j` loop walks `data[i * cols], data[i * cols + 1], ...` — sequential memory addresses, fully cache-friendly.

---

## Why Use faer for Performance

`faer` is a pure-Rust NLA library that implements cache-oblivious blocked algorithms with SIMD kernels.

### Transposing for faer

faer's `Mat` type is **column-major** internally. Passing row-major data directly produces incorrect results (the matrix is read transposed) or requires element-by-element conversion via `Mat::from_fn`, which is extremely slow — benchmarks showed 68 seconds for a 16000×16000 matrix versus milliseconds for the multiply itself.

The correct approach is a one-time transpose **outside the performance timer**:

```rust
// Transpose to column-major once, before benchmarking
let mut transposed = vec![0.0f64; size * size];
for i in 0..size {
    for j in 0..size {
        transposed[j * size + i] = matrix_a.get(i, j);
    }
}

// faer gets a zero-copy view into the transposed buffer
let faer_matrix = unsafe {
    MatRef::<f64>::from_raw_parts(
        transposed.as_ptr(),
        size, size,
        1,             // row stride = 1 (column-major)
        size as isize, // col stride = size
    )
};
```

`MatRef::from_raw_parts` creates a zero-copy view — no data is moved during the timed section. The `unsafe` block is required because Rust cannot verify pointer validity and stride correctness at compile time.

This framing is honest: the transpose cost is real and would be incurred in any system where data originates in row-major format. Excluding it from the timer isolates the algorithmic performance of the multiply itself.

---

## Project Structure

```
src/
  main.rs         — benchmark runner; add new bench_*() functions here
  matrix.rs       — Matrix struct and row-major indexing
  io.rs           — binary file generation and reading
  mat_vec/
    mod.rs        — mat-vec implementations (eq. 1.1, eq. 1.2, serial, parallel)
```

New topics (mat-mat multiplication, LU factorization, QR, etc.) each get their own subdirectory under `src/`, following the same pattern: direct textbook translation alongside a faer reference benchmark.

### Binary File Format

Matrix and vector data is stored in a simple binary format:

- **Header**: `rows` and `cols` as little-endian `u64` (matrices); `size` as little-endian `u64` (vectors)
- **Data**: row-major `f64` values in little-endian byte order

This format is orders of magnitude faster to read and write than text for large matrices, and the size is exactly `8 * rows * cols` bytes versus roughly `10-12 * rows * cols` bytes for formatted text. Regenerate binary files whenever the `Matrix` layout or generation logic changes.

---

## Language Stack

- **Rust** — primary implementation language; ownership model catches data races at compile time, Rayon provides ergonomic parallelism, and the performance ceiling matches C++
- **Python** — secondary; useful for quick verification and plotting since NumPy's `.npy` format is compatible with the flat `f64` buffer layout used here
- **faer** — pure-Rust NLA library used as the performance reference; provides LU, QR, SVD, and other factorizations that will serve as benchmarks for hand-written implementations in later modules
