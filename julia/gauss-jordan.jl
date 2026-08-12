# Gauss-Jordan Algorithm with Partial Pivoting

function update_pivot(row_pos, col_pos, A)
    # We only need to check below the pivot position becuase above will be zero
    row_pos += 1
    col_pos += 1
    original_pivot = A[row_pos,col_pos]
    original_row_pos = row_pos

    pivot, row_pos = check_for_larger_num(row_pos, col_pos, original_pivot, A)

    if (pivot != original_pivot)
        row_to_swap = A[original_row_pos,:]

        A[original_row_pos,:] = A[row_pos,:]
        A[row_pos,:] = row_to_swap
    end

    return row_pos, col_pos, pivot
end


function check_for_larger_num(row_pos, col_pos, pivot, A)
    # Initialize larger_num as pivot 
    larger_num = pivot
    row_num = row_pos

    for i in row_pos+1:m
        if (A[i,col_pos] > larger_num)
            larger_num = A[i,col_pos]
            row_num = i
        end
    end

    if (larger_num > pivot)
        pivot = larger_num
        row_pos = row_num
    end

    return pivot, row_pos
end


function gauss_jordan(m, n, A)
    row_pos = 0
    col_pos = 0
    pivot, row_pos = update_pivot(row_pos, col_pos, A)
end


function main()
    # The matrix is Float64 because of the .0s
    matrix = [2.0 1.0  2.0 10.0;
              1.0 2.0  1.0  8.0;
              3.0 1.0 -1.0  2.0]
    
    m, n = size(matrix)

    gauss_jordan(m, n, matrix)

end

main()