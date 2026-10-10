//Baseline that computes the multiplication on a single thread 

#include "mat_operations.h" //Including the header file for matrix operations

//Function for single thread multiplication
void MatrixMultiplySingle(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size)
{
    C.assign(size * size, 0.0); //Setting the size of the result matrix

    for (std::size_t row = 0; row < size; ++row) //Each row of A
    {
        for (std::size_t col = 0; col < size; ++col) //Each column of B
        {
            double sum = 0.0;
            for (std::size_t k = 0; k < size; ++k) //Walk along row of A and down column of B
            {
                sum += A[row * size + k] * B[k * size + col];
            }
            C[row * size + col] = sum;
        }
    }
}