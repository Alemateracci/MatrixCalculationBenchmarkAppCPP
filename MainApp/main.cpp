// Main application implementation together

#include "mat_operations.h"

#include <iostream>


int main()
{
    //Asking user for input on the size of the matrix
    std::size_t size = 0;
    std::cout << "Enter matrix size (e.g. 1000, 1500 or 2000): ";
    std::cin >> size;

    //Calling the function to create the matrices
    MatrixNa A, B;
    MatrixFill(A, size, 1);   // seed 1 for A
    MatrixFill(B, size, 2);   // seed 2 for B
}