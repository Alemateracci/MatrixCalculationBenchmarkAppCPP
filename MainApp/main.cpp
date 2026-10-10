// Main application implementation together

#include "mat_operations.h"
#include <iostream>
#include <chrono>
#include <iomanip>

//Function to calculate the checksum of a matrix
double ChecksumCheck(const MatrixNa& C)
{
    double checksum = 0.0;
    for (double value : C)
    {
        checksum += value;
    }
    return checksum;
}

//Function for single thread multiplication
double SingleThreadedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplySingle(A, B, C, size);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeSingle = endtime - starttime;

    //Sum of all elements of C: a quick fingerprint that every later version must reproduce
    double checksum = ChecksumCheck(C);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "\n";
    std::cout << "---------------------Single-threaded--------------------" << "\n";
    std::cout << "Single-threaded time: " << DurationTimeSingle.count() << " s\n";
    std::cout << "Checksum of C: " << checksum << "\n";
    std::cout << "--------------------------------------------------------" << "\n";
    std::cout << "\n";

    return checksum;
}

//Function for multi thread multiplication
double MultiThreadedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplyMulti(A, B, C, size, threads);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeMulti = endtime - starttime;

    //Same fingerprint as the single-thread version, the two must match
    double checksum = ChecksumCheck(C);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "---------------------Multi-threaded--------------------" << "\n";
    std::cout << "Multi-threaded time: " << DurationTimeMulti.count() << " s\n";
    std::cout << "Checksum of C: " << checksum << "\n";
    std::cout << "--------------------------------------------------------" << "\n";
    std::cout << "\n";

    return checksum;
}

//Function for multi thread multiplication with memory-optimised (reordered) loops
double MultiThreadedReorderedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplyMultiReordered(A, B, C, size, threads);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeReordered = endtime - starttime;

    //Same fingerprint as the single-thread version, they must match
    double checksum = ChecksumCheck(C);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "------------Multi-threaded, memory-optimised------------" << "\n";
    std::cout << "Multi-threaded memory-optimised time: " << DurationTimeReordered.count() << " s\n";
    std::cout << "Checksum of C: " << checksum << "\n";
    std::cout << "--------------------------------------------------------" << "\n";
    std::cout << "\n";

    return checksum;
}

//Function for multi thread multiplication with memory-optimised (reordered) loops and cache blocking
double MultiThreadedReorderedBlockedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplyMultiReorderedBlocked(A, B, C, size, threads);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeReorderedBlocked = endtime - starttime;

    //Same fingerprint as the single-thread version, they must match
    double checksum = ChecksumCheck(C);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "------------Multi-threaded and cache-blocked------------" << "\n";
    std::cout << "Multi-threaded memory-optimised and cache-blocked time: " << DurationTimeReorderedBlocked.count() << " s\n";
    std::cout << "Checksum of C: " << checksum << "\n";
    std::cout << "--------------------------------------------------------" << "\n";
    std::cout << "\n";

    return checksum;
}

//Main function
int main()
{
    //Asking user for input on the size of the matrix and checking if valid
    std::size_t size = 0;
    std::cout << "Enter matrix size (e.g. 1000, 1500 or 2000): ";
    std::cin >> size;
    if (size == 0)
    {
        std::cout << "Matrix size must be greater than 0. Assuming 1000\n";
        size = 1000;
    }

    //Asking user for input on the size of the matrix and checking if valid
    std::size_t threads = 0;
    std::cout << "Enter number of threads: ";
    std::cin >> threads;
    if (threads == 0)
    {
        std::cout << "Number of threads must be greater than 0. Assuming 1\n";
        threads = 1;
    }

    //Calling the function to create the matrices
    MatrixNa A, B, C;
    MatrixFill(A, size, 1);   // seed 1 for A
    MatrixFill(B, size, 2);   // seed 2 for B

    //Calling thread calculation function
    double SingleThreadedChecksum = SingleThreadedMatrixMultiplication(A, B, C, size);
    double MultiThreadedChecksum = MultiThreadedMatrixMultiplication(A, B, C, size, threads);
    double MultiThreadedReorderedChecksum = MultiThreadedReorderedMatrixMultiplication(A, B, C, size, threads);
    double MultiThreadedReorderedBlockedChecksum = MultiThreadedReorderedBlockedMatrixMultiplication(A, B, C, size, threads);

    if (SingleThreadedChecksum == MultiThreadedChecksum && SingleThreadedChecksum == MultiThreadedReorderedChecksum && SingleThreadedChecksum == MultiThreadedReorderedBlockedChecksum)
    {
        std::cout << "Checksums match! The results are consistent." << "\n";
        std::cout << "\n";
    }
    else
    {
        std::cout << "Checksums do not match! There is an inconsistency in the results." << "\n";
        std::cout << "\n";
    }
}