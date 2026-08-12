function mat_vec_mult_linear_combo!(n, b, A, x)
    for j in 1:n
        b .+= x[j] * A[:,j]
    end
end

m = 3
n = 3

A = [1 2 3;
     4 5 6;
     7 8 9]

x = [10, 20, 30]

b = zeros(m)

mat_vec_mult_linear_combo!(n, b, A, x)
println("Result b = ", b)

# $ julia mat_vec_mult_linear_combo.jl
# Result b = [140.0, 320.0, 500.0]