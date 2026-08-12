# Steps to install and use Rust on Windows (through Ubuntu)
1. Go to https://rustup.rs/
2. Download and run rustup‑init.exe
3. Microsoft C++ Build Tools may be required (if not already installed).
4. Open Windows PowerShell, and using the arrow drop-down at the top, select Ubuntu.
5. In this terminal window, navigate to the desired repository (numerical-linear-algebra-rust, in this case)
6. Run: sudo snap install rustup --classic
7. Run: rustup default stable
8. To set up Cargo inside this root directory, run: cargo init .
9. Create or move into the directory containing the specific .rs files you want to make (src/, in this case).
10. Create files, such as main.rs
11. To run the code: cargo run
