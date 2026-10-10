//Calculations of speedup and Amdahl's law from measured times

#include "mat_operations.h"

double CalculateSpeedup(double secondsSingleThread, double secondsMultiThread)
{
    if (secondsMultiThread <= 0.0)
    {
        return 0.0; // protects against dividing by zero
    }
    return secondsSingleThread / secondsMultiThread;
}

//Amdahl: speedup = 1 / ((1 - P) + P / N), solved for P gives P = (1 - 1 / speedup) / (1 - 1 / N)
double CalculateAmdahlParallelFraction(double speedup, std::size_t threads)
{
    if (threads < 2 || speedup <= 0.0)
    {
        return 0.0; // with one thread nothing can be measured
    }
    const double n = static_cast<double>(threads);
    return (1.0 - 1.0 / speedup) / (1.0 - 1.0 / n);
}
