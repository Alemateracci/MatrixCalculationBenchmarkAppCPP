//File used to create matrices with random values

#include "mat_operations.h" //Including the header file for matrix operations
#include <iostream> //Including the input-output stream library for console input and output
#include <random> //Including the random library for generating random numbers

//Function to fill a matrix with random values
void MatrixFill(MatrixNa& mat, std::size_t size, unsigned int seed)
{
    mat.resize(size * size); //Sets the matrix size

    std::mt19937 generator(seed); //Using the Mersenne Twister random number generator with a seed for reproducibility
    std::uniform_real_distribution<double> distribution(0.0, 1.0); //Uniform distribution of random numbers between 0.0 and 1.0

    //Filling the matrix with random values
    for (std::size_t k = 0; k < size * size; ++k) 
    {
        mat[k] = distribution(generator);
    }
}