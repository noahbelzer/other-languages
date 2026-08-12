! From Numerical Linear ALgebra by Trefethen and Bau
! gfortran matrix_vector_mult.f90 -o matrix_vector_mult

SUBROUTINE matrix_vector_mult(m, n, A, x, b)
    ! pg. 4, eq. 1.2
    IMPLICIT NONE
    INTEGER, INTENT(IN) :: m
    INTEGER, INTENT(IN) :: n
    REAL, INTENT(IN) :: A(5,5)
    REAL, INTENT(IN) :: x(5)
    REAL, INTENT(OUT) :: b(5)
    REAL :: sum = 0
    INTEGER :: i, j
    
    DO i = 1, m
      sum = 0
      DO j = 1, n
        sum = sum + A(i,j) * x(j)
      END DO
      
      b(i) = sum
    END DO

END SUBROUTINE matrix_vector_mult


SUBROUTINE familiar_definition(m, n, A, x, b)
    ! pg. 3, eq. 1.1
    IMPLICIT NONE

    INTEGER, INTENT(IN)  :: m, n
    REAL,    INTENT(IN)  :: A(m,n)
    REAL,    INTENT(IN)  :: x(n)
    REAL,    INTENT(OUT) :: b(m)

    INTEGER :: i, j
    REAL :: s

    s = 0.0
    
    DO i = 1, m
      s = 0
      DO j = 1, n
        s = s + A(i,j) * x(j)
      END DO
      
      b(i) = s
    END DO

END SUBROUTINE familiar_definition


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
    
    ! CALL matrix_vector_mult(m, n, A, x, b)
    CALL familiar_definition(m, n, A, x, b)
    
    PRINT *
    
    PRINT *, b

END PROGRAM main