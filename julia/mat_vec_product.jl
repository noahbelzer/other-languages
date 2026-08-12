# Previously installed:
# In the terminal below, type julia (then enter). Then type import Pkg; Pkg.add("OhMyThreads"). Then type exit().

# Run: julia --threads auto mat_vec_product.jl

using LinearAlgebra
using Base.Threads
using OhMyThreads

function read_matrix(filename::String, m::Int, n::Int)
    A = zeros(m, n)
    open(filename, "r") do f
        for i in 1:m
            A[i, :] = parse.(Float64, split(readline(f)))
        end
    end
    return A
end


function familiar_mat_vec_mult!(m, n, b, A, x)
    # Numerical Linear Algebra by Trefethen and Bau - pg. 3, eq. 1.1
    @threads for i in 1:m
        summation = 0.0

        for j in 1:n
            summation += A[i,j] * x[j]
        end

        b[i] = summation
    end
end

function linear_combo_mat_vec_mult!(m, n, b, A, x)
    # Numerical Linear Algebra by Trefethen and Bau - pg. 4, eq. 1.2
    @tasks for j in 1:n
        @set reducer = (+)
        b .+= x[j] * A[:,j]
    end
end

m, n = 10, 10
A = read_matrix("mat_vec_product_data.txt", m, n)
x = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0]
b = zeros(m)



println("Threads used: ", nthreads())

familiar_mat_vec_mult!(m, n, b, A, x)
println("familiar: Result b = ", b)

fill!(b, 0.0)  # reset before next call
linear_combo_mat_vec_mult!(m, n, b, A, x)
println("linear_combo: Result b = ", b)

println("Julia's built-in BLAS: ", A * x) 

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