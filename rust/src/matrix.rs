pub struct Matrix {
    pub rows: usize,
    pub cols: usize,
    pub data: Vec<f64>,
}

impl Matrix {
    pub fn get(&self, i: usize, j: usize) -> f64 {
        self.data[i * self.cols + j]  // row-major: natural for mathematical notation
    }
}
