! From Numerical Linear ALgebra by Trefethen and Bau
! Serial: gfortran matrix_matrix_mult.f90 -o matrix_matrix_mult
! OpenMP: gfortran -fopenmp matrix_matrix_mult.f90 -o matrix_matrix_mult
! OpenMP optimized: gfortran -fopenmp -O2 matrix_matrix_mult.f90 -o matrix_matrix_mult

! Run with 2 cores: OMP_NUM_THREADS=2 ./matrix_matrix_mult

! If using allocatable arrays, a clean strategy is to include the subroutine (that reads them) in a module, so the compiler can "see" it.
MODULE matrix_computations
    IMPLICIT NONE
CONTAINS
    SUBROUTINE matrix_times_matrix_serial(m, n, p, A, B, C)
        IMPLICIT NONE

        INTEGER, INTENT(IN)  :: m, n, p
        REAL,    INTENT(IN)  :: A(:,:)
        REAL,    INTENT(IN)  :: B(:,:)
        REAL,    INTENT(OUT) :: C(:,:)
        INTEGER :: i, j, k

        REAL :: dot_product

        DO i = 1, m
            DO j = 1, p
                dot_product = 0.0
                DO k = 1, n
                    dot_product = dot_product + A(i,k) * B(k,j)
                END DO
                C(i,j) = dot_product
            END DO
        END DO
    END SUBROUTINE matrix_times_matrix_serial

    SUBROUTINE matrix_times_matrix(m, n, p, A, B, C)
        use omp_lib
        IMPLICIT NONE

        INTEGER, INTENT(IN)  :: m, n, p
        REAL,    INTENT(IN)  :: A(:,:)
        REAL,    INTENT(IN)  :: B(:,:)
        REAL,    INTENT(OUT) :: C(:,:)
        INTEGER :: i, j, k

        REAL :: dot_product

        !$omp parallel do private(dot_product, j, k)
        DO i = 1, m
            DO j = 1, p
                dot_product = 0.0
                DO k = 1, n
                    dot_product = dot_product + A(i,k) * B(k,j)
                END DO
                C(i,j) = dot_product
            END DO
        END DO
        !$omp end parallel do
    END SUBROUTINE matrix_times_matrix
END MODULE


PROGRAM main
    USE matrix_computations
    IMPLICIT NONE

    INTEGER :: m, n, p
    REAL, ALLOCATABLE :: A(:,:), B(:,:), C(:,:)
    !INTEGER :: i, j
    !REAL :: count = 1
    INTEGER :: start_count, end_count, count_rate
    REAL    :: serial_time, parallel_time
    m = 2000
    n = 2000
    p = 2000
    ! --------------------------------------------------

    ! Allocate the matrices
    ALLOCATE(A(m,n), B(n,p), C(m,p))
    CALL RANDOM_SEED()

    ! Initialize A and B with random numbers in [0.0, 1.0)
    CALL RANDOM_NUMBER(A)
    CALL RANDOM_NUMBER(B)

    ! Initialize C to 0.0 for all values to avoid garbage values
    C = 0.0
    
    ! Parallel
    CALL SYSTEM_CLOCK(start_count, count_rate)
    CALL matrix_times_matrix(m, n, p, A, B, C)
    CALL SYSTEM_CLOCK(end_count)
    parallel_time = REAL(end_count - start_count) / REAL(count_rate)
    PRINT *, "Parallel time (seconds):", parallel_time

    ! Serial
    CALL SYSTEM_CLOCK(start_count, count_rate)
    CALL matrix_times_matrix_serial(m, n, p, A, B, C)
    CALL SYSTEM_CLOCK(end_count)
    serial_time = REAL(end_count - start_count) / REAL(count_rate)
    PRINT *, "Serial time (seconds):", serial_time


    PRINT *, "Speedup:", serial_time / parallel_time

    DEALLOCATE(A, B, C)

END PROGRAM main