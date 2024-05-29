#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class StopWatch
{
public:
    StopWatch();

    double Elapsed(); // in mills

private:
    std::chrono::steady_clock::time_point start_point_;

};

#endif // STOPWATCH_H
