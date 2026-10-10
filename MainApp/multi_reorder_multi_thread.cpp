//Computes the multiplication on multiple threads with memory-optimised (reordered) loops

#include "mat_operations.h" //Including the header file for matrix operations
#include <thread> //Including the thread library for multi-threading
#include <vector> //Including the vector library for using std::vector


//Function for calculating rows [rowStart, rowEnd) of C with the loop order row, k, col
static void MultiplyMatrixRowsReordered(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t rowStart, std::size_t rowEnd)
{
    for (std::size_t row = rowStart; row < rowEnd; ++row) //Every row in the whole A
    {
        for (std::size_t k = 0; k < size; ++k) //Each column in the whole A (or row in the whole B)
        {
            const double a = A[row * size + k]; //Loaded once and reused for the whole row of B
            for (std::size_t col = 0; col < size; ++col) //Walk along row k of B and along row of C
            {
                C[row * size + col] += a * B[k * size + col];
            }
        }
    }
}

//Function for multi thread multiplication with memory-optimised (reordered) loops
void MatrixMultiplyMultiReordered(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
{
    C.assign(size * size, 0.0); //Setting the size of the result matrix and zeroing it (the += in the loop needs a zero start)

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
    const std::size_t extra = size % threads; // if nr of rows dont divide equally the extra rows needs to be assigned to some thread
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
            MultiplyMatrixRowsReordered(A, B, C, size, start, end);
        });

        start = end;
    }

    for (auto& thread : workerthreads)
    {
        thread.join(); // wait for all threads to finish
    }
}