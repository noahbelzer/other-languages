! Ran: sudo apt install gfortran liblapack-dev libblas-dev

! gfortran -O2 solve.f90 -llapack -lblas -o solve
! ./solve

PROGRAM lapack_hello
  IMPLICIT NONE
  INTEGER, PARAMETER :: dp = KIND(1.0D0)
  INTEGER, PARAMETER :: n = 3, nrhs = 1
  REAL(dp) :: A(n,n), b(n)
  INTEGER  :: ipiv(n), info, i

  ! Column-major: A(:,1) is the first COLUMN, not the first row.
  A(:,1) = [ 2.0_dp,  1.0_dp,  1.0_dp ]
  A(:,2) = [ 1.0_dp,  3.0_dp,  2.0_dp ]
  A(:,3) = [ 1.0_dp,  2.0_dp,  4.0_dp ]
  b      = [ 4.0_dp, 5.0_dp, 6.0_dp ]

  CALL DGESV(n, nrhs, A, n, ipiv, b, n, info)

  IF (info /= 0) THEN
     PRINT *, 'DGESV failed, INFO =', info
     STOP 1
  END IF

  PRINT *, 'Solution x ='
  DO i = 1, n
     PRINT '(F12.6)', b(i)
  END DO
END PROGRAM lapack_hello