// Main application implementation together

#include "mat_operations.h"
#include <iostream>
#include <chrono>
#include <iomanip>

//Function for single thread multiplication
void SingleThreadedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplySingle(A, B, C, size);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeSingle = endtime - starttime;

    //Sum of all elements of C: a quick fingerprint that every later version must reproduce
    double checksum = 0.0;
    for (double value : C)
    {
        checksum += value;
    }

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "\n";
    std::cout << "---------------------Single-threaded---------------------" << "\n";
    std::cout << "Single-threaded time: " << DurationTimeSingle.count() << " s\n";
    std::cout << "Checksum of C: " << checksum << "\n";
    std::cout << "\n";
}

//Main function
int main()
{
    //Asking user for input on the size of the matrix
    std::size_t size = 0;
    std::cout << "Enter matrix size (e.g. 1000, 1500 or 2000): ";
    std::cin >> size;

    //Calling the function to create the matrices
    MatrixNa A, B, C;
    MatrixFill(A, size, 1);   // seed 1 for A
    MatrixFill(B, size, 2);   // seed 2 for B

    //Calling single thread calculation function
    SingleThreadedMatrixMultiplication(A, B, C, size);
    

}