//File for all shared operations used on matrices and vectors ut only declares them (no body)

#pragma once

#include <cstddef>
#include <string>
#include <vector>

using MatrixNa = std::vector<double>; //Matrix 

// Fills an n x n matrix with random values between 0 and 1 using the provided seed.
void MatrixFill(MatrixNa& mat, std::size_t size, unsigned int seed); //Unassigned means only positive larger numbers

//Multiplication function of AxB = C with single thread.
void MatrixMultiplySingle(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size);

//Multiplication function of AxB = C with multiple threads.
void MatrixMultiplyMulti(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads);

//Multiplication function of AxB = C with multiple threads and cache-friendly (reordered) loops.
void MatrixMultiplyMultiReordered(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads);

//Multiplication function of AxB = C with multiple threads and cache blocking (the matrices are processed in small tiles).
void MatrixMultiplyMultiReorderedBlocked(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads);

//Speedup of a multi-threaded run compared to the single-thread run: T(single) / T(multi).
double CalculateSpeedup(double timeSingleThread, double timeMultiThread);

//Parallel fraction P of the program measured from the speedup (Amdahl's law solved for P).
double CalculateAmdahlParallelFraction(double speedup, std::size_t threads);

//Speedup that Amdahl's law predicts for a parallel fraction P on N threads.
double CalculateAmdahlSpeedup(double parallelFraction, std::size_t threads);
