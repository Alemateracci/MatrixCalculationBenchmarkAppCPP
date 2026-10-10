//File for all shared operations used on matrices and vectors ut only declares them (no body)

#pragma once

#include <cstddef>
#include <string>
#include <vector>

using MatrixNa = std::vector<double>; //Matrix 

// Fills an n x n matrix with random values between 0 and 1 using the provided seed.
void MatrixFill(MatrixNa& mat, std::size_t size, unsigned int seed); //Unassigned means only positive larger numbers

//Multiplication function of AxB = C.
void MatrixMultiplySingle(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size);