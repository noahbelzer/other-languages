# Exercises: Trefethen pg. 9
# Elementary matrices, Properties: Meyer pg. 132
#=
Properties of Elementary Matrices
• When used as a left-hand multiplier, an elementary matrix of Type
I, II, or III executes the corresponding row operation.
• When used as a right-hand multiplier, an elementary matrix of Type
I, II, or III executes the corresponding column operation.
=#

function question(B)
       # Think about elementary matrices. Do the same thing to I that you do to B.
       op1 = [2 0 0 0;
              0 1 0 0;
              0 0 1 0;
              0 0 0 1]

       op2 = [1 0 0   0;
              0 1 0   0;
              0 0 1/2 0;
              0 0 0   1]

       op3 = [1 0 1 0;
              0 1 0 0;
              0 0 1 0;
              0 0 0 1]

       op4 = [0 0 0 1;
              0 1 0 0;
              0 0 1 0;
              1 0 0 0]
       
       op5 = [1 -1 0 0;
              0  1 0 0;
              0 -1 1 0;
              0 -1 0 1]
       
       op6 = [1 0 0 0;
              0 1 0 0;
              0 0 1 1;
              0 0 0 0]

       # Dimensions 4x4 * 4x3 produces a 4x3 
       # Just delete column 1 from the identity matrix I
       op7 = [0 0 0;
              1 0 0;
              0 1 0;
              0 0 1]


       # Part a)
       # Work your way out from the inside (multpily op2 next to B, then the next row operation op3 to the left of that)
       #=
       standard matrix multiplication order applies, so you work inside-out (right to left for left-hand multipliers).
       For example, if you're applying a sequence of row operations E₁, E₂, E₃ to A:
       E₃ E₂ E₁ A

       You compute it as E₃(E₂(E₁A)). So E₁ acts on A first, then E₂ acts on the result, then E₃. Each left-multiplied elementary matrix peels off from the inside out.
       The same logic applies to right-hand multipliers for column operations, just mirrored — you'd work left to right in that case.
       =#
       # Print each step (of the 7) at a time:
       #display(B * op1)
       #display(op2 * B * op1)
       #display(op3 * op2 * B * op1)
       #display(op3 * op2 * B * op1 * op4)
       #display(op5 * op3 * op2 * B * op1 * op4) 
       #display(op5 * op3 * op2 * B * op1 * op4 * op6) 
       println("Product of 8 matrices:")
       display(op5 * op3 * op2 * B * op1 * op4 * op6 * op7) 

       # Part b)
       A = op5 * op3 * op2
       C = op1 * op4 * op6 * op7
       println("Product ABC:")
       display(A * B * C)
end


function main()
       a = 1
       b = 2
       c = 3
       d = 4
       e = 5
       f = 6
       g = 7
       h = 8
       i = 9
       j = 10
       k = 11
       l = 12
       m = 13
       n = 14
       o = 15
       p = 16

       B = [a b c d;
            e f g h;
            i j k l;
             m n o p]

       # I applied all the operations in the provided order (from the 7 steps) and provided the algebraic result:
       Result = [(b+j/2)-f (c+k/2)-g (c+k/2)-g    ;
                     f         g          g       ;
                     j/2-f     k/2-g      k/2-g   ;
                     n-f       o-g        o-g     ]

       
       println("Matrix B:")
       display(B)

       question(B)

       # Parts a and b should match the algebraic result. It does as of 5-25-2026.
       println()
       println("Matrix Result:")
       display(Result)
end

main()

