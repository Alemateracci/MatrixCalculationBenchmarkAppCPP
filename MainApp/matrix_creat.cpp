//File used to create matrices with random values

#include "mat_operations.h"

#include <iostream> 
#include <random> 

void MatrixFill(MatrixNa& mat, std::size_t size, unsigned int seed)
{
    mat.resize(size * size); //Sets the matrix size

    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (std::size_t k = 0; k < size * size; ++k) 
    {
        mat[k] = distribution(generator);
    }
}