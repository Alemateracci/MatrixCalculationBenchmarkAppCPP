//Computes the multiplication on multiple threads 

#include "mat_operations.h"
#include <thread>
#include <vector>


//Calculates rows [rowStart, rowEnd) of C, only used inside this file
static void MultiplyMatrixRows(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t rowStart, std::size_t rowEnd)
{
    for (std::size_t row = rowStart; row < rowEnd; ++row) // each row of A given to this thread
    {
        for (std::size_t col = 0; col < size; ++col) // each column of B
        {
            double sum = 0.0;
            for (std::size_t k = 0; k < size; ++k) // walk along row of A and down column of B
            {
                sum += A[row * size + k] * B[k * size + col];
            }
            C[row * size + col] = sum;
        }
    }
}

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

    std::vector<std::thread> workerthreads;
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
        
        workerthreads.emplace_back([&A, &B, &C, size, start, end]()
        {
            MultiplyMatrixRows(A, B, C, size, start, end);
        });

        start = end;
    }

    for (auto& thread : workerthreads)
    {
        thread.join(); // wait for all threads to finish
    }
}