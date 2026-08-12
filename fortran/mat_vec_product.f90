! Previously ran: sudo apt install libopenblas-dev
!
! Serial: gfortran mat_vec_product.f90 -o mat_vec_product
! OpenMP with BLAS: gfortran -fopenmp mat_vec_product.f90 -o mat_vec_product -lopenblas

PROGRAM main
    IMPLICIT NONE
    INTEGER :: m, n
    REAL, ALLOCATABLE :: b(:)
    REAL, ALLOCATABLE :: A(:,:)
    REAL, ALLOCATABLE :: x(:)
    INTEGER :: i

    m = 10
    n = 10

    ALLOCATE(b(m))
    ALLOCATE(A(m,n))
    ALLOCATE(x(n))

    ! Load the data
    x(1) = 1
    x(2) = 2
    x(3) = 3
    x(4) = 4
    x(5) = 5
    x(6) = 6
    x(7) = 7
    x(8) = 8
    x(9) = 9
    x(10) = 10

    CALL read_matrix('mat_vec_product_data.txt', m, n, A)

    ! Call the algorithms
    CALL familiar_mat_vec_mult(m, n, b, A, x)

    DO i = 1, m
        WRITE(*, '(A, I0, A, F8.4)') "familiar: b(", i, ") =", b(i)
    END DO

    CALL linear_combo_mat_vec_mult(m, n, b, A, x)

    DO i = 1, m
        WRITE(*, '(A, I0, A, F8.4)') "linear combo: b(", i, ") =", b(i)
    END DO

    ! BLAS:
    ! The corresponding BLAS routine is SGEMV (Single-precision GEneral Matrix-Vector multiply).
    ! It computes: b = αAx + βb.
    ! Setting alpha=1.0 and beta=0.0 reduces this to just b = Ax
    ! Call signature: CALL SGEMV(trans, m, n, alpha, A, lda, x, incx, beta, b, incy)
    !
    ! Argument: Value: Meaning:
    ! trans     'N'    No transpose — compute Ax, not A^T x
    ! m          m     Number of rows of A
    ! n          n     Number of columns of A
    ! alpha      1.0   Scalar multiplier on Ax
    ! A          A     The matrix
    ! lda        m     Leading dimension of A (number of rows for column-major)
    ! x          x     The input vector
    ! incx       1     Stride through x. 1 means contiguous
    ! beta       0.0   Scalar multiplier on the input b. 0 overwrites it
    ! b          b     The output vector
    ! incy       1     Stride through b

    CALL SGEMV('N', m, n, 1.0, A, m, x, 1, 0.0, b, 1)

    DO i = 1, m
        WRITE(*, '(A, I0, A, F8.4)') "BLAS SGEMV: b(", i, ") =", b(i)
    END DO

    DEALLOCATE(b)
    DEALLOCATE(A)
    DEALLOCATE(x)

END PROGRAM main

SUBROUTINE linear_combo_mat_vec_mult(m, n, b, A, x)
    ! Numerical Linear Algebra by Trefethen and Bau - pg. 4, eq. 1.2
    USE omp_lib
    IMPLICIT NONE
    INTEGER, INTENT(IN) :: m, n
    REAL, INTENT(IN)  :: A(m,n), x(n)
    REAL, INTENT(OUT) :: b(m)
    INTEGER :: j

    !$omp parallel
    !$omp single
    WRITE(*, '(A, I0, A)') "Running with ", omp_get_num_threads(), " threads"
    !$omp end single
    !$omp end parallel

    b = 0.0

    ! Each thread gets a private zero-initialized copy of b, accumulates into it, and the results are summed together at the end. 
    !$omp parallel do reduction(+:b)
    DO j = 1, n
        b = b + x(j) * A(:,j)
    END DO
    !$omp end parallel do
END SUBROUTINE linear_combo_mat_vec_mult


SUBROUTINE familiar_mat_vec_mult(m, n, b, A, x)
    ! Numerical Linear Algebra by Trefethen and Bau - pg. 3, eq. 1.1
    USE omp_lib
    IMPLICIT NONE
    INTEGER, INTENT(IN) :: m, n
    REAL, INTENT(IN)  :: A(m,n), x(n)
    REAL, INTENT(OUT) :: b(m)
    REAL :: sum
    INTEGER :: i, j

    !$omp parallel
    !$omp single
    WRITE(*, '(A, I0, A)') "Running with ", omp_get_num_threads(), " threads"
    !$omp end single
    !$omp end parallel

    !$omp parallel do private(sum)
    DO i = 1, m
        sum = 0

        DO j = 1, n
            sum = sum + A(i,j) * x(j)
        END DO

        b(i) = sum
    END DO
    !$omp end parallel do
END SUBROUTINE familiar_mat_vec_mult


SUBROUTINE read_matrix(filename, m, n, A)
    IMPLICIT NONE
    CHARACTER(LEN=*), INTENT(IN) :: filename
    INTEGER, INTENT(IN) :: m, n
    REAL, INTENT(OUT) :: A(m,n)
    INTEGER :: i, unit, ierr

    OPEN(newunit=unit, file=filename, status='old', action='read', iostat=ierr)
    IF (ierr /= 0) STOP "Error opening matrix file"

    DO i = 1, m
        READ(unit, *) A(i,:)
    END DO

    CLOSE(unit)

    ! Solution:
    ! b(1) =164.0000
    ! b(2) =197.0000
    ! b(3) =315.0000
    ! b(4) =242.0000
    ! b(5) =225.0000
    ! b(6) =210.0000
    ! b(7) =104.0000
    ! b(8) =334.0000
    ! b(9) =320.0000
    ! b(10) =261.0000
END SUBROUTINE read_matrix