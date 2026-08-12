! Gaussian Elimination with Partial Pivoting
! Serial: gfortran gaussian_elim.f90 -o gaussian_elim
! OpenMP: gfortran -fopenmp gaussian_elim.f90 -o gaussian_elim
! OpenMP optimized: gfortran -fopenmp -O2 gaussian_elim.f90 -o gaussian_elim

! Run with 2 cores: OMP_NUM_THREADS=2 ./gaussian_elim

! If using allocatable arrays, a clean strategy is to include the subroutine (that reads them) in a module, so the compiler can "see" it.

MODULE matrix_computations
    IMPLICIT NONE
CONTAINS
    SUBROUTINE gaussian_elim_serial(m, n, A)
        ! Assumptions: the first column of the A is not all zeros. (There is at least 1 nonzero value in the first column.)
        IMPLICIT NONE

        INTEGER, INTENT(IN)    :: m, n
        REAL,    INTENT(INOUT) :: A(:,:)
        INTEGER                :: i, j

        REAL    :: pivot, largest_num, temp_sum
        INTEGER :: piv_row_index, piv_col_index
        ! One row (1D array) contains n elements
        REAL    :: row_to_swap(n)
        REAL(KIND=8) :: results(m)
        INTEGER :: swap_row
    
        piv_row_index = 1
        piv_col_index = 1
        pivot = A(piv_row_index,piv_col_index)
        
        DO WHILE (piv_row_index <= m)
            swap_row = 0
            WRITE(*,'(A,F8.4,A,I0,A,I0,A)') "Pivot: ", pivot, " at A(", piv_row_index, ",", piv_col_index, ")"
            PRINT *

            ! If the entire column is zeros, move the pivot one index to the right
            IF (ALL(A(:, piv_col_index) == 0.0)) THEN
                ! whole column is zero
                piv_col_index = piv_col_index + 1
                pivot = A(piv_row_index, piv_col_index)
            END IF

            WRITE(*,'(A,F8.4,A,I0,A,I0,A)') "Pivot: ", pivot, " at A(", piv_row_index, ",", piv_col_index, ")"
            PRINT *
            DO i = 1, m
                PRINT *, (A(i,j), j=1,n)
            END DO
            PRINT *
        
            largest_num = pivot

            ! Partial Pivoting
            ! Check all numbers below the current pivot for a larger num.
            IF (piv_row_index < m) THEN
                DO i = piv_row_index + 1, m
                    IF (ABS(A(i,piv_col_index)) > largest_num) THEN
                        WRITE(*,'(A,F8.4,A,I0,A,I0,A)') &
                            "Larger num is: ", ABS(A(i,piv_col_index)), &
                            " found at A(", i, ",", piv_col_index, ")"
                        PRINT *

                        largest_num = ABS(A(i,piv_col_index))
                        swap_row = i
                    END IF
                END DO
              END IF

            ! If a larger num exists, then interchange rows.
            IF (swap_row /= 0.0) THEN
                row_to_swap = A(swap_row,:)
                A(swap_row,:) = A(piv_row_index,:)

                A(piv_row_index,:) = row_to_swap
                pivot = A(piv_row_index,piv_col_index)
            END IF

            WRITE(*,'(A,F8.4,A,I0,A,I0,A)') "Pivot: ", pivot, " at A(", piv_row_index, ",", piv_col_index, ")"
            PRINT *
            DO i = 1, m
                PRINT *, (A(i,j), j=1,n)
            END DO
            PRINT *
            
            IF (pivot /= 1.0) THEN
                A(piv_row_index,:) = A(piv_row_index,:) / pivot
            END IF
            
            PRINT *, "After scaling pivot to 1"
            DO i = 1, m
               PRINT *, (A(i,j), j=1,n)
            END DO
            PRINT *
        
            DO i = piv_row_index + 1, m
                IF (A(i,piv_col_index) /= 0.0) THEN
                    A(i,:) = A(i,:) - A(i,piv_col_index) * A(piv_row_index,:)
                END IF
            END DO
        
            PRINT *, "After zeroing out entries below the pivot"
            DO i = 1, m
                PRINT *, (A(i,j), j=1,n)
            END DO
            PRINT *
        
            IF (piv_row_index <= m) THEN
                piv_row_index = piv_row_index + 1
                piv_col_index = piv_col_index + 1
                pivot = A(piv_row_index,piv_col_index)
            END IF
        END DO
        
        WHERE (ABS(A) < 1.0e-10) A = 0.0
        
        PRINT *, "Final Matrix:"
        DO i = 1, m
            PRINT *, (A(i,j), j=1,n)
        END DO
        PRINT *
 
        ! Back-substitution
        ! Solve for the last x value first
        IF (A(m,m) == 1) THEN
            results(m) = A(m,n)
            WRITE(*,'(A,F10.6)') "Last x = ", results(m)
            PRINT *
        END IF

        DO i = m-1, 1, -1
            temp_sum = 0
            WRITE(*,'(A,I0)') "i = ", i
            DO j = n-1, i+1, -1
                WRITE(*,'(A,I0)') "j = ", j
                IF (j == n-1) THEN
                    temp_sum = A(i,n)
                END IF
                temp_sum = temp_sum - A(i,j) * results(j)
            END DO
            results(i) = temp_sum
            WRITE(*,'(A,F10.6)') "results(i) ", results(i)
        END DO

        ! Print the result
        PRINT *, "RESULT HERE"
        DO i = 1, m
            WRITE(*,'(A,I0,A,F10.4)') "x_", i, " = ", results(i)
        END DO
    END SUBROUTINE gaussian_elim_serial

    SUBROUTINE gaussian_elim(m, n, A)
        use omp_lib
        IMPLICIT NONE

        INTEGER, INTENT(IN)  :: m, n
        REAL,    INTENT(INOUT)  :: A(:,:)
        INTEGER :: i, j, k

        REAL :: dot_product
    END SUBROUTINE gaussian_elim
END MODULE


PROGRAM main
    USE matrix_computations
    IMPLICIT NONE

    INTEGER :: m, n, i, j
    REAL, ALLOCATABLE :: A(:,:)

    INTEGER :: start_count, end_count, count_rate
    REAL    :: serial_time!, parallel_time
    m = 5
    n = 6
    ! --------------------------------------------------
    ! Seems to work properly with 3 x 4 problems and 5 x 6 problems
    ! Use this to verify: https://onlinemschool.com/math/assistance/equation/gaus/

    ! Allocate the matrices
    ALLOCATE(A(m,n))
    !CALL RANDOM_SEED()

    ! Initialize A with random numbers in [0.0, 1.0)
    !CALL RANDOM_NUMBER(A)
    
    A(1,1) = 9.0
    A(2,1) = 8.0
    A(3,1) = 1.0
    A(4,1) = 1.0
    A(5,1) = 3.0

    A(1,2) = 6.0
    A(2,2) = 6.0
    A(3,2) = 2.0
    A(4,2) = 1.0
    A(5,2) = 2.0


    A(1,3) = 1.0
    A(2,3) = 2.0
    A(3,3) = 3.0
    A(4,3) = 4.0
    A(5,3) = 5.0

    A(1,4) = 2.0
    A(2,4) = 2.0
    A(3,4) = 2.0
    A(4,4) = 3.0
    A(5,4) = 3.0

    A(1,5) = 0.0
    A(2,5) = 2.0
    A(3,5) = 1.0
    A(4,5) = 2.0
    A(5,5) = 1.0

    A(1,6) = 3.0
    A(2,6) = 9.0
    A(3,6) = 1.0
    A(4,6) = 2.0
    A(5,6) = 5.0

    DO i = 1, m
        PRINT *, (A(i,j), j=1,n)
    END DO
    PRINT *

    ! Parallel
    !CALL SYSTEM_CLOCK(start_count, count_rate)
    !CALL matrix_times_matrix(m, n, p, A, B, C)
    !CALL SYSTEM_CLOCK(end_count)
    !parallel_time = REAL(end_count - start_count) / REAL(count_rate)
    !PRINT *, "Parallel time (seconds):", parallel_time

    ! Serial
    CALL SYSTEM_CLOCK(start_count, count_rate)
    CALL gaussian_elim_serial(m, n, A)
    CALL SYSTEM_CLOCK(end_count)
    serial_time = REAL(end_count - start_count) / REAL(count_rate)
    PRINT *, "Serial time (seconds):", serial_time


    !PRINT *, "Speedup:", serial_time / parallel_time

    DEALLOCATE(A)

END PROGRAM main