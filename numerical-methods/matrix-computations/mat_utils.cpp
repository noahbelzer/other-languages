#include "mat_utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

void print_matrix(const std::vector<std::vector<double>>& matrix) {
    /*
    Print a 2D matrix
    */
    std::cout << "Matrix: " << std::endl;

    // For row in matrix
    for (const auto& row : matrix) {
        // For element in row
        for (const auto& elem : row) {
            // Fix floating point precision printing: Print very small values as zero
            double val = (abs(elem) < 1e-12) ? 0.0 : elem;
            
            // Give two spaces for positive numbers, and one for negatives, for alignment
            if (val >= 0)
                std::cout << val << "  ";  // extra space for alignment
            else
                std::cout << val << " ";
        }
	std::cout << std::endl;
    }

    std::cout << "" << std::endl;
}


void print_vector(const std::vector<double>& vec) {
    /*
    Print a 1D vector
    */
    std::cout << "Vector: " << std::endl;

    // For elem in vector
    for (const auto& elem : vec) {
        // Fix floating point precision printing: Print very small values as zero
        double val = (abs(elem) < 1e-12) ? 0.0 : elem;

        // Give two spaces for positive numbers, and one for negatives, for alignment
        if (val >= 0)
            std::cout << val << "  " << std::endl;  // extra space for alignment
        else
            std::cout << val << " " << std::endl;
        }
    std::cout << std::endl;
}


std::vector<std::vector<double>> read_matrix_csv(const std::string& filename) {
    std::vector<std::vector<double>> A;

    std::ifstream f(filename);
    if (!f.is_open())
        throw std::runtime_error("Could not open file: " + filename);

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;

        std::vector<double> row;
        std::istringstream iss(line);
        std::string token;

        while (std::getline(iss, token, ',')) {
            try {
                row.push_back(std::stod(token));
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("Invalid value: \"" + token + "\"");
            }
        }

        if (!A.empty() && row.size() != A[0].size())
            throw std::runtime_error("Inconsistent column count at row " + std::to_string(A.size()));

        A.push_back(std::move(row));
    }

    if (A.empty())
        throw std::runtime_error("File is empty or contains no valid data: " + filename);

    return A;
}
