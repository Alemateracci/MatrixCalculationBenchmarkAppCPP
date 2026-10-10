//Calculations of speedup and Amdahl's law from measured times

#include "mat_operations.h" //Including the header file for matrix operations

//Function for speedup of a multi-threaded run compared to the single-thread
double CalculateSpeedup(double secondsSingleThread, double secondsMultiThread)
{
    //Checking if the multi-threaded time is less than or equal to 0.0
    if (secondsMultiThread <= 0.0)
    {
        return 0.0; // protects against dividing by zero
    }
    return secondsSingleThread / secondsMultiThread;
}

//Function for parallel fraction P of the program measured from the speedup (Amdahl's law solved for P)
double CalculateAmdahlParallelFraction(double speedup, std::size_t threads)
{
    //Checking if the number of threads is less than 2 or if the speedup is less than or equal to 0.0
    if (threads < 2 || speedup <= 0.0)
    {
        return 0.0; // with one thread nothing can be measured
    }
    const double n = static_cast<double>(threads);
    return (1.0 - 1.0 / speedup) / (1.0 - 1.0 / n);
}
