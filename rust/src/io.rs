use std::fs::File;
use std::io::{Read, Write};
use crate::matrix::Matrix;

pub fn generate_matrix_file(filename: &str, rows: usize, cols: usize) {
    let mut file = File::create(filename).expect("Could not create file");

    file.write_all(&(rows as u64).to_le_bytes()).expect("Could not write header");
    file.write_all(&(cols as u64).to_le_bytes()).expect("Could not write header");

    for i in 0..rows {
        for j in 0..cols {
            let val = (i * cols + j) as f64;
            file.write_all(&val.to_le_bytes()).expect("Could not write value");
        }
    }

    println!("Generated {}x{} matrix in {}", rows, cols, filename);
}

pub fn generate_vector_file(filename: &str, size: usize) {
    let mut file = File::create(filename).expect("Could not create file");

    file.write_all(&(size as u64).to_le_bytes()).expect("Could not write header");

    for i in 0..size {
        let val = i as f64;
        file.write_all(&val.to_le_bytes()).expect("Could not write value");
    }

    println!("Generated {}x1 vector in {}", size, filename);
}

pub fn read_matrix(filename: &str) -> Matrix {
    let mut file = File::open(filename).expect("Could not open file");
    let mut buf = [0u8; 8];

    file.read_exact(&mut buf).expect("Could not read rows");
    let rows = u64::from_le_bytes(buf) as usize;

    file.read_exact(&mut buf).expect("Could not read cols");
    let cols = u64::from_le_bytes(buf) as usize;

    let mut data = vec![0.0f64; rows * cols];
    for val in data.iter_mut() {
        file.read_exact(&mut buf).expect("Could not read value");
        *val = f64::from_le_bytes(buf);
    }

    Matrix { rows, cols, data }
}

pub fn read_vector(filename: &str) -> Vec<f64> {
    let mut file = File::open(filename).expect("Could not open file");
    let mut buf = [0u8; 8];

    file.read_exact(&mut buf).expect("Could not read size");
    let size = u64::from_le_bytes(buf) as usize;

    let mut data = vec![0.0f64; size];
    for val in data.iter_mut() {
        file.read_exact(&mut buf).expect("Could not read value");
        *val = f64::from_le_bytes(buf);
    }

    data
}
