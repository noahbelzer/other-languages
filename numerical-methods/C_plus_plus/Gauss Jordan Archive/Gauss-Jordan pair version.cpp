// g++ -Wall Gauss-Jordan.cpp -o Gauss-Jordan
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void PrintMatrix(const vector<vector<double>>& matrix) {
    /*
    Print a 2D vector
    */

    cout << "Matrix: " << endl;
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            // Fix floating point precision printing: Print very small values as zero (to handle -0 printing instead of 0)
            double val = (abs(elem) < 1e-12) ? 0.0 : elem;

            if (val >= 0)
                cout << val << "  ";  // extra space for alignment
            else
                cout << val << " ";
        }
        cout << endl;
    }

    cout << "" << endl;
}

vector<vector<double>> Algorithm(const vector<vector<double>>& passed_matrix) {
    /*
    Algorithm takes a 2D vector as input and returns a 2D vector
    */

    vector<vector<double>> matrix = passed_matrix;  // copy input

    size_t pivot_row = 0;
    size_t pivot_col = 0;
    double pivot = matrix[pivot_row][pivot_col];

    while (pivot_row < matrix.size() && pivot_col < matrix[0].size()) {
        cout << "Pivot: " << pivot << " at " << pivot_row << ", " << pivot_col << endl;
        bool non_zero_found = false;
        
        // ----- Start: Is the pivot poisition zero? If so, swap rows using partial pivoting. -----
        while ((pivot == 0) && (pivot_row + 1 < matrix.size())) {
            cout << "While run for is pivot position?" << endl;
            
            non_zero_found = false;
            vector<double> swap_row;
            vector<double> original_row;
            vector<pair<double,int>> non_zero_candidates;

            // Check all numbers below the pivot position in the same column
            for (size_t i = pivot_row + 1; i < matrix.size(); ++i) {
                if (matrix[i][pivot_col] != 0) {
                    non_zero_found = true;
                    non_zero_candidates.push_back({matrix[i][pivot_col], i});
                }
            }

            cout << "Inside while for is pivot pos, non_zero_found: " << non_zero_found << endl;
            if (non_zero_found) {
                original_row = matrix[pivot_row];

                // Find the maximum first value using a lambda function for the built-in max_element function from <algorithm>
                auto pointer_to_pair_containing_max = max_element(
                    non_zero_candidates.begin(),
                    non_zero_candidates.end(),
                    [](const pair<double, int>& a, const pair<double, int>& b) {
                        return abs(a.first) < abs(b.first);  // compare only the first element of the pair
                    }
                );

                // Get the row (second element of the pair) corresponding to the max first value
                int row_num_of_max_value = pointer_to_pair_containing_max->second;

                // Swap the pivot row and the row containing a non-zero value in the pivot col
                matrix[pivot_row] = matrix[pointer_to_pair_containing_max->second];
                matrix[row_num_of_max_value] = original_row;

                pivot = matrix[pivot_row][pivot_col];
            } else {
                // Keep the row the same, but increment the col
                if (pivot_col + 1 >= matrix[0].size()) {
                    break;  // Exit if we are at the last column
                } else {
                    ++pivot_col;
                    pivot = matrix[pivot_row][pivot_col];
                    cout << "New pivot due to zeros on or below the pos: " << pivot << " at " << pivot_row << ", " << pivot_col << endl;
                }
            }
        }
        // ----- End: Is the pivot poisition zero? If so, swap rows using partial pivoting. -----

        // ----- Start: Make the pivot 1, if needed -----
        if (pivot != 1) {
            for (double& x : matrix[pivot_row]) {
                x /= pivot;
            }
            pivot = matrix[pivot_row][pivot_col];
            cout << "New pivot turned to a 1: " << pivot << " at " << pivot_row << ", " << pivot_col << endl;
        }
        // ----- End: Make the pivot 1, if needed -----

        // ----- Start: Perform a row operation for all rows below and above the pivot (subtract a multiple of the pivot row to the current row) -----
        // Check all numbers above and below the pivot position in the same column
        non_zero_found = false;
        for (size_t i = 0; i < matrix.size(); ++i) {
            if (matrix[i][pivot_col] != 0) {
                non_zero_found = true;
            }
        }

        cout << "Non zero: " << non_zero_found << endl;
        
        if (non_zero_found) {
            size_t left_to_right = 0;

            for (size_t i = 0; i < matrix.size(); ++i) {
                cout << "i: " << i << endl;
                if (i != pivot_row) {
                    // The number above or below the pivot in the same column as it
                    double current_number = matrix[i][pivot_col];
                    cout << "Current Number: " << current_number << endl;

                    if (current_number != 0) {
                        for (double& x : matrix[i]) {
                            cout << "pivot_row: " << pivot_row << ", left_to_right: " << left_to_right << endl;
                            cout << x << " = " << x << " - " <<current_number << " * " << matrix[pivot_row][left_to_right] << " | ";
                            x -= current_number * matrix[pivot_row][left_to_right];
                            cout << "Updated x: " << x << endl;

                            if (left_to_right + 1 < matrix[0].size()) {
                                ++left_to_right;
                            }
                        }

                        left_to_right = 0;  // reset left_to_right
                    }
                }
            }

            cout << "Set of row operations done!" << endl;
            PrintMatrix(matrix);
            cout << "Intermediate matrix printed!" << endl;
        }

        if (pivot_row + 1 >= matrix.size() || pivot_col + 1 >= matrix[0].size()) {
            break;  // Exit if we are at the last row or last column
        } else {
            ++pivot_row;
            ++pivot_col;
            pivot = matrix[pivot_row][pivot_col];
        }
        // ----- End: Perform a row operation for all rows below and above the pivot (subtract a multiple of the pivot row to the current row) -----
    }

    return matrix;
}


int main() {
    // Initialize matrix as a vector of vectors. pg. 56
    vector<vector<double>> typed_matrix = {
        {1, 2, 1, 3, 3},
        {2, 4, 0, 4, 4},
        {1, 2, 3, 5, 5},
        {2, 4, 0, 4, 7}
    };

    vector<vector<double>> test_matrix = {
        {1, 2, 2, 3, 1},
        {2, 4, 4, 6, 2},
        {3, 6, 6, 9, 6},
        {1, 2, 4, 5, 3}
    };

    cout << "First Matrix: " << endl;
    PrintMatrix(typed_matrix);

    vector<vector<double>> result = Algorithm(typed_matrix);

    cout << "Finished Matrix: " << endl;
    PrintMatrix(result);

    return 0;
}
