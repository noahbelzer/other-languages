function mat_mat_mult(m, n, A, B)
	# Familiar definition - Trefethen pg. 5
	C = zeros(m, n)

	for i in 1:m
		for j in 1:n
			for k in 1:n
				C[i,j] += A[i,k] * B[k,j]
			end
		end
	end

	display(C)
end


function linear_combo(m, n, A, C)
	# Linear combination definition - Trefethen pg. 6
	B = zeros(m, n)

	for j in 1:n
		for k in 1:n
			B[:,j] .+= C[k,j] * A[:,k]
		end
	end

	display(B)
end


function main()
	m = 3
	n = 3

	A = [1 2  3;
		 0 7 10;
		 2 9  7]

	B = [4 5 6;
		 1 2 2;
		 5 3 8]

	mat_mat_mult(m, n, A, B)
	linear_combo(m, n, A, B)
	println("True Answer:")
	display(A * B)
end

main()