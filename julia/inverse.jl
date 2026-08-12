using LinearAlgebra

function linear_combo(m, A, Z)
    # Trefethen pg. 8
    E = zeros(m, m)

    for j in 1:m
        for i in 1:m
            # @views prevents unnecessary memory allocation when slicing (e.g. E[:,j]).
            # Without it, Julia: (1) allocates a temporary array to hold the slice,
            # (2) performs the operation into that copy, (3) writes the copy back to E,
            # then (4) discards the temporary array. @views eliminates steps 1, 3, and 4
            # by pointing directly into E's existing memory instead.
            @views E[:,j] .+= Z[i,j] * A[:,i]
        end
    end

    display(E)
end

function main()
	m = 3

	A = [2 1 1;
		 3 2 1;
		 2 1 2]

    A_inv = inv(A)
    display(A_inv)

    linear_combo(m, A, A_inv)
end

main()