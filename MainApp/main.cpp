// Main application implementation together

#include "mat_operations.h"
#include <iostream>
#include <chrono>
#include <iomanip>

//Structure for returning data from each experiment run
struct RunResult
{
    double time;
    double checksum;
};

//Function to calculate the checksum of a matrix
double ChecksumCheck(const MatrixNa& C)
{
    double checksum = 0.0;
    for (double value : C)
    {
        checksum += value;
    }
    return checksum;
}

//Function for single thread multiplication
RunResult SingleThreadedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplySingle(A, B, C, size);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeSingle = endtime - starttime;

    //Sum of all elements of C: a quick fingerprint that every later version must reproduce
    double checksum = ChecksumCheck(C);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "\n";
    std::cout << "---------------------Single-threaded--------------------" << "\n";
    std::cout << "Time: " << DurationTimeSingle.count() << " s\n";
    std::cout << "Checksum: " << checksum << "\n";
    std::cout << "--------------------------------------------------------" << "\n";
    std::cout << "\n";

    return RunResult(DurationTimeSingle.count(), checksum);
}

//Function for multi thread multiplication
RunResult MultiThreadedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplyMulti(A, B, C, size, threads);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeMulti = endtime - starttime;

    //Same fingerprint as the single-thread version, the two must match
    double checksum = ChecksumCheck(C);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "---------------------Multi-threaded---------------------" << "\n";
    std::cout << "Time: " << DurationTimeMulti.count() << " s\n";
    std::cout << "Checksum: " << checksum << "\n";
    std::cout << "--------------------------------------------------------" << "\n";
    std::cout << "\n";

    return RunResult(DurationTimeMulti.count(), checksum);
}

//Function for multi thread multiplication with memory-optimised (reordered) loops
RunResult MultiThreadedReorderedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplyMultiReordered(A, B, C, size, threads);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeReordered = endtime - starttime;

    //Same fingerprint as the single-thread version, they must match
    double checksum = ChecksumCheck(C);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "------------Multi-threaded, memory-optimised------------" << "\n";
    std::cout << "Time: " << DurationTimeReordered.count() << " s\n";
    std::cout << "Checksum: " << checksum << "\n";
    std::cout << "--------------------------------------------------------" << "\n";
    std::cout << "\n";

    return RunResult(DurationTimeReordered.count(), checksum);
}

//Function for multi thread multiplication with memory-optimised (reordered) loops and cache blocking
RunResult MultiThreadedReorderedBlockedMatrixMultiplication(const MatrixNa& A, const MatrixNa& B, MatrixNa& C, std::size_t size, std::size_t threads)
{
    auto starttime = std::chrono::steady_clock::now();
    MatrixMultiplyMultiReorderedBlocked(A, B, C, size, threads);
    auto endtime = std::chrono::steady_clock::now();

    std::chrono::duration<double> DurationTimeReorderedBlocked = endtime - starttime;

    //Same fingerprint as the single-thread version, they must match
    double checksum = ChecksumCheck(C);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "------------Multi-threaded and cache-blocked------------" << "\n";
    std::cout << "Time: " << DurationTimeReorderedBlocked.count() << " s\n";
    std::cout << "Checksum: " << checksum << "\n";
    std::cout << "--------------------------------------------------------" << "\n";
    std::cout << "\n";

    return RunResult(DurationTimeReorderedBlocked.count(), checksum);
}

//Prints the speedup and the laws of one multi-threaded version compared with the single-thread run
void PrintLaws(const char* name, double secondsSingleThread, double secondsMultiThread, std::size_t threads)
{
    double speedup = CalculateSpeedup(secondsSingleThread, secondsMultiThread);
    double parallelFraction = CalculateAmdahlParallelFraction(speedup, threads);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "--------" << name << " (" << threads << " threads)--------" << "\n";
    std::cout << "Single-thread time: " << secondsSingleThread << " s\n";
    std::cout << "Multi-thread time: " << secondsMultiThread << " s\n";
    std::cout << "Speedup vs single-thread: " << speedup << "x\n";
    std::cout << "Parallel fraction P (Amdahl): " << parallelFraction << "\n";
    std::cout << "----------------------------------------------------------" << "\n";
    std::cout << "\n";
}

//Main function
int main()
{
    //Asking user for input on the size of the matrix and checking if valid
    std::size_t size = 0;
    std::cout << "Enter matrix size (e.g. 1000, 1500 or 2000): ";
    std::cin >> size;
    if (size == 0)
    {
        std::cout << "Matrix size must be greater than 0. Assuming 1000\n";
        size = 1000;
    }

    //Asking user for input on the size of the matrix and checking if valid
    std::size_t threads = 0;
    std::cout << "Enter number of threads: ";
    std::cin >> threads;
    if (threads == 0)
    {
        std::cout << "Number of threads must be greater than 0. Assuming 1\n";
        threads = 1;
    }

    //Calling the function to create the matrices
    MatrixNa A, B, C;
    MatrixFill(A, size, 1);   // seed 1 for A
    MatrixFill(B, size, 2);   // seed 2 for B

    //Calling thread calculation function
    RunResult SingleThreadedData = SingleThreadedMatrixMultiplication(A, B, C, size);
    RunResult MultiThreadedData = MultiThreadedMatrixMultiplication(A, B, C, size, threads);
    RunResult MultiThreadedReorderedData = MultiThreadedReorderedMatrixMultiplication(A, B, C, size, threads);
    RunResult MultiThreadedReorderedBlockedData = MultiThreadedReorderedBlockedMatrixMultiplication(A, B, C, size, threads);

    //Printing if all results are the same
    if (SingleThreadedData.checksum == MultiThreadedData.checksum && SingleThreadedData.checksum == MultiThreadedReorderedData.checksum && SingleThreadedData.checksum == MultiThreadedReorderedBlockedData.checksum)
    {
        std::cout << "\n";
        std::cout << "CHECKSUM MATCHES. THE RESULTS ARE CONSISTENT." << "\n";
        std::cout << "\n";
    }
    else
    {
        std::cout << "\n";
        std::cout << "CHECKSUMS DO NOT MATCH. THERE IS AN INCONSISTENCY IN THE RESULTS." << "\n";
        std::cout << "\n";
    }

    //Laws: each multi-threaded version compared with the single-thread run
    std::cout << "--------------------------LAWS-----------------------------" << "\n";
    PrintLaws("Multi-threaded", SingleThreadedData.time, MultiThreadedData.time, threads);
    PrintLaws("Memory-optimised", SingleThreadedData.time, MultiThreadedReorderedData.time, threads);
    PrintLaws("Cache-blocked", SingleThreadedData.time, MultiThreadedReorderedBlockedData.time, threads);

}