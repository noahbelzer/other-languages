! From Numerical Linear ALgebra by Trefethen and Bau
! Serial: gfortran matrix_vector_mult.f90 -o matrix_vector_mult
! OpenMP: gfortran -fopenmp matrix_vector_mult.f90 -o matrix_vector_mult

SUBROUTINE familiar_definition(m, n, A, x, b)
    ! pg. 3, eq. 1.1
    use omp_lib
    IMPLICIT NONE

    INTEGER, INTENT(IN)  :: m, n
    REAL,    INTENT(IN)  :: A(m,n)
    REAL,    INTENT(IN)  :: x(n)
    REAL,    INTENT(OUT) :: b(m)

    INTEGER :: i, j
    REAL :: s
    
    !$omp parallel do private(j, s)
    DO i = 1, m
      s = 0.0

      DO j = 1, n
        s = s + A(i,j) * x(j)
      END DO
      
      b(i) = s
    END DO
    !$omp end parallel do

END SUBROUTINE familiar_definition


SUBROUTINE matrix_times_vector(m, n, A, x, b)
    ! pg. 4, eq. 1.2
    IMPLICIT NONE

    INTEGER, INTENT(IN)  :: m, n
    REAL,    INTENT(IN)  :: A(m,n)
    REAL,    INTENT(IN)  :: x(n)
    REAL,    INTENT(OUT) :: b(m)

    INTEGER :: j

    b = 0.0

    DO j = 1, n
        b = b + x(j) * A(:, j)
    END DO

END SUBROUTINE matrix_times_vector


PROGRAM main
    IMPLICIT NONE
    REAL :: A(5,5)
    REAL :: x(5)
    INTEGER :: i, j
    REAL :: count = 1
    REAL :: b(5)
    INTEGER :: m = 5
    INTEGER :: n = 5

    ! Initialize A
    DO j = 1, n
        DO i = 1, m
            A(i, j) = count
            count = count + 1
        END DO
    END DO
  
    ! Initialize B
    DO j = 1, n
        x(j) = count
        count = count + 1
    END DO
    
    DO i = 1, m
        PRINT *, (A(i,j), j=1,n)
    END DO
    
    PRINT *
    
    PRINT *, x
    
    CALL matrix_times_vector(m, n, A, x, b)
    !CALL familiar_definition(m, n, A, x, b)
    
    PRINT *
    
    PRINT *, b

END PROGRAM main