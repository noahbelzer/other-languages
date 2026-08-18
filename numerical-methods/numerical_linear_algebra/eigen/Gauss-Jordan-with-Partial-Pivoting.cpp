/*
Primary Gauss-Jordan elimination algorithm with partial pivoting implemented.
At each step, whenever the pivot position moves, we check the rows including the current pivot row and the ones below it to determine if there is a larger value.
If there is a larger value below or above our pivot position, we swap the rows to bring that larger value into the pivot position (making this partial pivoting).

The other script, called Gauss-Jordan-only-partial-pivoting-for-case-of-zero-pivot.cpp, only does partial pivoting when the pivot position is zero and we need to perform a swap (might as well swap it with the row that has the largest value in the pivot column).
The other script is more in line with the Matrix Analysis book.

This script always does partial pivoting at each step, which may be more robust and stronger for numerical stability by reducing rounding errors.

Compile:
g++ -Wall Gauss-Jordan-with-Partial-Pivoting.cpp -o Gauss-Jordan-with-Partial-Pivoting
*/
#include "Gauss-Jordan-with-Partial-Pivoting.h"

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void Print_Matrix(const vector<vector<double>>& matrix) {
    /*
    Print a 2D vector
    */

    cout << "Matrix: " << endl;

    // For row in matrix
    for (const auto& row : matrix) {
        // For element in row
        for (const auto& elem : row) {
            // Fix floating point precision printing: Print very small values as zero (to handle -0 printing instead of 0)
            double val = (abs(elem) < 1e-12) ? 0.0 : elem;
            
            // Give 2 spaces for positive numbers, and one for negatuves, for alignment
            if (val >= 0)
                cout << val << "  ";  // extra space for alignment
            else
                cout << val << " ";
        }
        cout << endl;
    }

    cout << "" << endl;
}

vector<vector<double>> Gauss_Jordan(const vector<vector<double>>& passed_matrix) {
    /*
    Algorithm takes a 2D vector as input and returns a 2D vector
    */

    // Make a copy of the input, so we don't modify the original
    vector<vector<double>> matrix = passed_matrix;  

    size_t pivot_row = 0;
    size_t pivot_col = 0;

    // Initialize the pivot position
    double pivot = matrix[pivot_row][pivot_col];

    // While the rows and columns are within bounds
    while (pivot_row < matrix.size() && pivot_col < matrix[0].size()) {
        cout << "Pivot: " << pivot << " at " << pivot_row << ", " << pivot_col << endl;
        
        // ----- Start: Partial pivoting -----
        vector<double> swap_row;
        vector<double> original_row;

        // Candidates for the maximum value in the pivot col: Create a vector to hold pairs of {value in pivot col, its row index}
        vector<pair<double,int>> candidates; 

        // Check all numbers starting with the pivot position and moving down in the same col
        for (size_t i = pivot_row; i < matrix.size(); ++i) {
            // Add the pair of {value in pivot col, its row index} to the candidates vector
            candidates.push_back({matrix[i][pivot_col], i});
        }
        
        // Save the original row before any potential swap
        original_row = matrix[pivot_row];

        // Find the maximum value from the pivot col using a lambda function for the built-in max_element function from <algorithm>
        auto pointer_to_pair_containing_max = max_element(
            candidates.begin(),
            candidates.end(),
            [](const pair<double, int>& a, const pair<double, int>& b) {
                // Compare only the magnitude of the first element of each pair
                return abs(a.first) < abs(b.first);  
            }
        );

        cout << "Max value in pivot column: " << pointer_to_pair_containing_max->first << " at row " << pointer_to_pair_containing_max->second << endl;

        // Save the row index of the max value
        size_t row_index_of_max_value = pointer_to_pair_containing_max->second;

        // If the max value is in a row other than the pivot row, perform the swap
        if (row_index_of_max_value != pivot_row) {
            // Swap the pivot row and the row containing the largest value in the pivot col
            matrix[pivot_row] = matrix[row_index_of_max_value];
            matrix[row_index_of_max_value] = original_row;

            // Reassign the pivot position to the same location, but with a different value, after the swap
            pivot = matrix[pivot_row][pivot_col];
            cout << "New pivot due to swap: " << pivot << " at " << pivot_row << ", " << pivot_col << endl;
        }

        // After partial pivoting, if the new pivot position is still zero, just move the pivot location one position to the right
        // Note: If all the candidates were zero to begin with, the max_element function just kept the first occurnace anyway, so the pivot row should have stayed as the row containing the max value
        if (pivot == 0) {
            // Keep the row the same, but increment the col
            if (pivot_col + 1 >= matrix[0].size()) {
                // Exit if we are at the last column
                break;  
            } else {
                ++pivot_col;
                // Reassign the pivot position
                pivot = matrix[pivot_row][pivot_col];
                cout << "New pivot due to zero pivot: " << pivot << " at " << pivot_row << ", " << pivot_col << endl;
                
                // If the new pivot is zero again after this, then recheck the numbers below it to see if there is a non zero value
                if (pivot == 0) {
                    // Stop this iteration of the while loop and start over with a new iteration
                    continue;  
                }
            }
        }
        // ----- End: Partial pivoting -----

        // ----- Start: Make the pivot 1, if needed -----
        if (pivot != 1) {
            for (double& x : matrix[pivot_row]) {
                x /= pivot;
            }
            pivot = matrix[pivot_row][pivot_col];
            cout << "New pivot turned to a 1: " << pivot << " at " << pivot_row << ", " << pivot_col << endl;
        }
        // ----- End: Make the pivot 1, if needed -----

        // ----- Start: Perform a row operation for all rows below and above the pivot (subtract a multiple of the pivot row from the current row) -----
        // Check all numbers above and below the pivot position in the same column
        // We want to check if there is any non-zero value above or below the pivot in the same column
        bool non_zero_found = false;
        for (size_t i = 0; i < matrix.size(); ++i) {
            if (matrix[i][pivot_col] != 0) {
                non_zero_found = true;
            }
        }

        cout << "Non zero: " << non_zero_found << endl;
        
        // If the entire column was zero (meaning there are all zeros below and above the pivot), don't do the row operations and let the pivot position move down and right later
        if (non_zero_found) {
            size_t left_to_right = 0;

            for (size_t i = 0; i < matrix.size(); ++i) {
                cout << "i: " << i << endl;
                if (i != pivot_row) {
                    // The current number is the one above or below the pivot we are currently looking at
                    double current_number = matrix[i][pivot_col];
                    cout << "Current Number: " << current_number << endl;

                    if (current_number != 0) {
                        for (double& x : matrix[i]) {
                            cout << "pivot_row: " << pivot_row << ", left_to_right: " << left_to_right << endl;
                            cout << x << " = " << x << " - " <<current_number << " * " << matrix[pivot_row][left_to_right] << " | ";

                            // Perform the row operation (subtract a multiple of the pivot row from the current row)
                            x -= current_number * matrix[pivot_row][left_to_right];
                            
                            // Fix floating point numbers being different instead of 0, like 3-3 = 4.44089e-16.
                            x = (abs(x) < 1e-12) ? 0.0 : x;
                            cout << "        Updated x: " << x << endl;

                            // If there are still more cols to the right, increment left_to_right to look at the next column to the right
                            if (left_to_right + 1 < matrix[0].size()) {
                                ++left_to_right;
                            }
                        }

                        // Reset left_to_right to begin at the leftmost column again for the next row operation
                        left_to_right = 0;  
                    }
                }
            }

            cout << "Set of row operations done!" << endl;
            Print_Matrix(matrix);
            cout << "Intermediate matrix printed!" << endl;
        }

        if (pivot_row + 1 >= matrix.size() || pivot_col + 1 >= matrix[0].size()) {
            // Exit if we are at the last row or last column
            break;  
        } else {
            // Move the pivot location down one row and right one column
            ++pivot_row;
            ++pivot_col;

            // Reassign the pivot position
            pivot = matrix[pivot_row][pivot_col];
        }
        // ----- End: Perform a row operation for all rows below and above the pivot (subtract a multiple of the pivot row to the current row) -----
    }

    return matrix;
}

/*
int main() {
    
    // Initialize matrix as a vector of vectors. pg. 56
    vector<vector<double>> typed_matrix = {
        {1, 2, 1, 3, 3},
        {2, 4, 0, 4, 4},
        {1, 2, 3, 5, 5},
        {2, 4, 0, 4, 7}
    };

    vector<vector<double>> test_matrix = {
        {2, 1, 2, 10},
        {1, 2, 1, 8},
        {3, 1, -1, 2}
    };

    vector<vector<double>> test_matrix2 = {
        {1, -2, 0, -4},
        {0, -5, 1, -9},
        {4, 0, -3, -10}
    };

    vector<vector<double>> test_matrix3 = {
        {2, -4, -8, 6, 3},
        {0, 1, 3, 2, 3},
        {3, -2, 0, 0, 8}
    };

    cout << "First Matrix: " << endl;
    Print_Matrix(typed_matrix);

    vector<vector<double>> result = Gauss_Jordan(typed_matrix);

    cout << "Finished Matrix: " << endl;
    Print_Matrix(result);
    
    return 0;
}
*/
