//Computes the multiplication on multiple threads with cache blocking (tiling)

#include "mat_operations.h"
#include <algorithm>
#include <thread>
#include <vector>


//Size of one tile (blockSize x blockSize numbers), 64 x 64 doubles = 32 KB
static const std::size_t blockSize = 64;

//Calculates rows [rowStart, rowEnd) of C tile by tile, only used inside this file
static void MultiplyMatrixRowsBlocked(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t rowStart, std::size_t rowEnd)
{
    for (std::size_t rowBlock = rowStart; rowBlock < rowEnd; rowBlock += blockSize) // tile of rows of C given to this thread
    {
        const std::size_t rowLimit = std::min(rowBlock + blockSize, rowEnd);

        for (std::size_t kBlock = 0; kBlock < size; kBlock += blockSize) // tile along k
        {
            const std::size_t kLimit = std::min(kBlock + blockSize, size);

            for (std::size_t colBlock = 0; colBlock < size; colBlock += blockSize) // tile of columns of C
            {
                const std::size_t colLimit = std::min(colBlock + blockSize, size);

                //Same loops as the reordered version, but only inside the current tile
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

    std::vector<std::thread> workerthreads;
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
            MultiplyMatrixRowsBlocked(A, B, C, size, start, end);
        });

        start = end;
    }

    for (auto& thread : workerthreads)
    {
        thread.join(); // wait for all threads to finish
    }
}