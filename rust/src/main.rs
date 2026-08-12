// Run: cargo run --release
mod matrix;
mod io;
mod mat_vec;
use io::{generate_matrix_file, generate_vector_file, read_matrix, read_vector};
use std::time::Instant;
use faer::{Mat, MatRef};
use sysinfo::{System, SystemExt, CpuExt};

fn cpu_usage(sys: &mut System) -> f32 {
    sys.refresh_cpu();
    let cpus = sys.cpus();
    let total: f32 = cpus.iter().map(|c| c.cpu_usage()).sum();
    total / cpus.len() as f32
}

fn time_it<F, R>(label: &str, trials: usize, sys: &mut System, mut f: F)
where
    F: FnMut() -> R,
{
    let total: u128 = (0..trials)
        .map(|_| {
            let start = Instant::now();
            let _r = f();
            start.elapsed().as_micros()
        })
        .sum();
    let usage = cpu_usage(sys);
    println!(
        "{}: {:.3}ms  (avg CPU usage: {:.1}%)",
        label,
        (total / trials as u128) as f64 / 1000.0,
        usage
    );
}

fn bench_mat_vec(sys: &mut System) {
    let size = 8000;
    let matrix_file = format!("matrix_{}.bin", size);
    let vector_file = format!("vector_{}.bin", size);

    // Uncomment when needed:
    generate_matrix_file(&matrix_file, size, size);
    generate_vector_file(&vector_file, size);

    let matrix_a = read_matrix(&matrix_file);
    let vec_x    = read_vector(&vector_file);

    let mut transposed = vec![0.0f64; size * size];
    for i in 0..size {
        for j in 0..size {
            transposed[j * size + i] = matrix_a.get(i, j);
        }
    }

    println!("--- mat-vec benchmark (avg of 10 trials) ---");
    let trials = 10;

    time_it("eq. 1.1 serial  ", trials, sys, || mat_vec::inner_product(&matrix_a, &vec_x));
    time_it("eq. 1.1 parallel", trials, sys, || mat_vec::parallel_inner_product(&matrix_a, &vec_x));
    time_it("eq. 1.2 serial  ", trials, sys, || mat_vec::linear_combo(&matrix_a, &vec_x));
    time_it("eq. 1.2 parallel", trials, sys, || mat_vec::parallel_linear_combo(&matrix_a, &vec_x));
    time_it("faer            ", trials, sys, || {
        let faer_matrix = unsafe {
            MatRef::<f64>::from_raw_parts(transposed.as_ptr(), size, size, 1, size as isize)
        };
        let faer_vec = Mat::<f64>::from_fn(size, 1, |i, _| vec_x[i]);
        faer_matrix * &faer_vec
    });
}

fn main() {
    let mut sys = System::new_all();
    sys.refresh_all();

    bench_mat_vec(&mut sys);

    // future benchmarks go here:
    // bench_mat_mat(&mut sys);
    // bench_lu(&mut sys);
}
