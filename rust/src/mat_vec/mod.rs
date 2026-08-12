use crate::matrix::Matrix;
use rayon::prelude::*;

pub fn inner_product(a: &Matrix, x: &Vec<f64>) -> Vec<f64> {
    // Numerical Linear Algebra by Trefethen and Bau - pg. 3, eq. 1.1
    (0..a.rows)
        .into_iter()
        .map(|i| (0..a.cols).map(|j| a.get(i, j) * x[j]).sum())
        .collect()
}

pub fn parallel_inner_product(a: &Matrix, x: &Vec<f64>) -> Vec<f64> {
    // Numerical Linear Algebra by Trefethen and Bau - pg. 3, eq. 1.1
    (0..a.rows)
        .into_par_iter()
        .map(|i| (0..a.cols).map(|j| a.get(i, j) * x[j]).sum())
        .collect()
}

pub fn linear_combo(a: &Matrix, x: &Vec<f64>) -> Vec<f64> {
    // Numerical Linear Algebra by Trefethen and Bau - pg. 4, eq. 1.2
    let mut b = vec![0.0f64; a.rows];
    for j in 0..a.cols {
        for i in 0..a.rows {
            b[i] += x[j] * a.get(i, j);
        }
    }
    b
}

pub fn parallel_linear_combo(a: &Matrix, x: &Vec<f64>) -> Vec<f64> {
    // Numerical Linear Algebra by Trefethen and Bau - pg. 4, eq. 1.2
    // b = sum_j x[j] * A[:,j]
    (0..a.cols)
        .into_par_iter()         // split columns across threads
        .map(|j| {
            let x_j = x[j];
            // compute one scaled column: x[j] * A[:,j]
            // produces a Vec<f64> of length rows
            (0..a.rows)
                .map(|i| x_j * a.get(i, j))
                .collect::<Vec<f64>>()
        })
        .reduce(
            // identity: a zero vector to initialize each thread's accumulator
            || vec![0.0f64; a.rows],
            // combine two partial sums by adding them elementwise
            |mut acc, col| {
                for i in 0..a.rows { acc[i] += col[i]; }
                acc  // return the updated accumulator
            }
        )
}
