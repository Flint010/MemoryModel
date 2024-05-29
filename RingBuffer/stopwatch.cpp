#include "stopwatch.h"

StopWatch::StopWatch()
{
    start_point_ = std::chrono::steady_clock::now();
}

double StopWatch::Elapsed()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_point_).count();
}
