//Baseline that computes the multiplication on a single thread 

#include "mat_operations.h"

void MatrixMultiplySingle(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size)
{
    C.assign(size * size, 0.0); //Setting the size of the result matrix

    for (std::size_t row = 0; row < size; ++row) // each row of A
    {
        for (std::size_t col = 0; col < size; ++col) // each column of B
        {
            double sum = 0.0;
            for (std::size_t k = 0; k < size; ++k) // walk along row of A and down column  of B
            {
                sum += A[row * size + k] * B[k * size + col];
            }
            C[row * size + col] = sum;
        }
    }
}