//Computes the multiplication on multiple threads 

#include "mat_operations.h" //Including the header file for matrix operations
#include <thread> //Including the thread library for multi-threading
#include <vector> //Including the vector library for std::vector


//Function for calculating rows [rowStart, rowEnd) of C, only used inside this file
static void MultiplyMatrixRows(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t rowStart, std::size_t rowEnd)
{
    for (std::size_t row = rowStart; row < rowEnd; ++row) //Each column of A
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

//Function for multi thread multiplication
void MatrixMultiplyMulti(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
{
    C.assign(size * size, 0.0); //Setting the size of the result matrix

    //Checking and sanitizing the number of threads input 
    if (threads == 0)
    {
        threads = 1;
    }
    else if (threads > size)
    {
        threads = size;
    }

    std::vector<std::thread> workerthreads; //Vector to hold the worker threads
    const std::size_t rowsperthread = size / threads; // rows every thread gets
    const std::size_t extra = size % threads; // if nr of rows dont divide equally the the extra rows needs to be assigned to some thread
    std::size_t start = 0;

    for (std::size_t t = 0; t < threads; ++t)
    {
        std::size_t end = start + rowsperthread;
        if (t < extra)
        {
            end = end + 1; // the first 'extra' threads go one row further
        }
        
        //Starting a new thread to compute the rows [start, end) of C
        workerthreads.emplace_back([&A, &B, &C, size, start, end]()
        {
            MultiplyMatrixRows(A, B, C, size, start, end);
        });

        start = end;
    }

    for (auto& thread : workerthreads)
    {
        thread.join(); //Wait for all threads to finish
    }
}