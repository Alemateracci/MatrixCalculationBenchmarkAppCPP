//Computes the multiplication on multiple threads with cache blocking (tiling)

#include "mat_operations.h" //Including the header file for matrix operations
#include <algorithm> //Including the algorithm library for std::min
#include <thread> //Including the thread library for multi-threading
#include <vector> //Including the vector library for std::vector

static const std::size_t blockSize = 64; //Size of one tile (blockSize x blockSize numbers), 64 x 64 doubles = 32 KB

//Function for calculating rows [rowStart, rowEnd) of C tile by tile, only used inside this file
static void MultiplyMatrixRowsBlocked(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t rowStart, std::size_t rowEnd)
{
    for (std::size_t rowBlock = rowStart; rowBlock < rowEnd; rowBlock += blockSize) //Selected block of rows of C to compute
    {
        const std::size_t rowLimit = std::min(rowBlock + blockSize, rowEnd); //Limit of the current block of rows to compute

        for (std::size_t kBlock = 0; kBlock < size; kBlock += blockSize) //Selected block of columns of A and rows of B to compute
        {
            const std::size_t kLimit = std::min(kBlock + blockSize, size); //Limit of the current block of columns of A and rows of B to compute

            for (std::size_t colBlock = 0; colBlock < size; colBlock += blockSize) //Selected block of columns of C to compute
            {
                const std::size_t colLimit = std::min(colBlock + blockSize, size); //Limit of the current block of columns of C to compute

                //Same loops as the reordered version, but only inside the current selected block
                for (std::size_t row = rowBlock; row < rowLimit; ++row)
                {
                    for (std::size_t k = kBlock; k < kLimit; ++k)
                    {
                        const double a = A[row * size + k]; // loaded once and reused along the tile row of B
                        for (std::size_t col = colBlock; col < colLimit; ++col)
                        {
                            C[row * size + col] += a * B[k * size + col];
                        }
                    }
                }
            }
        }
    }
}

//Function for multi thread multiplication with memory-optimised (reordered) loops and cache blocking
void MatrixMultiplyMultiReorderedBlocked(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
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
    const std::size_t rowsperthread = size / threads; //Rows that every thread gets
    const std::size_t extra = size % threads; //If nr of rows dont divide equally the extra rows needs to be assigned to some thread
    std::size_t start = 0; //Starting row for the current thread

    //Creating the worker threads and assigning them their rows to compute
    for (std::size_t t = 0; t < threads; ++t)
    {
        std::size_t end = start + rowsperthread; //Ending row for the current thread
        if (t < extra)
        {
            end = end + 1; // the first 'extra' threads go one row further
        }

        //Creating a thread and assigning it the function to compute its rows
        workerthreads.emplace_back([&A, &B, &C, size, start, end]()
        {
            MultiplyMatrixRowsBlocked(A, B, C, size, start, end);
        });

        start = end;
    }

    //Waiting for all threads to finish their work
    for (auto& thread : workerthreads)
    {
        thread.join(); //Wait for all threads to finish and join together
    }
}