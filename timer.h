#ifndef STUDENT_PROCESSOR_TIMER_H
#define STUDENT_PROCESSOR_TIMER_H

#include <chrono>

class Timer {
public:
    Timer() : start_{std::chrono::steady_clock::now()} {}

    void reset() {
        start_ = std::chrono::steady_clock::now();
    }

    double elapsed() const {
        return std::chrono::duration<double>(std::chrono::steady_clock::now() - start_).count();
    }

private:
    std::chrono::steady_clock::time_point start_;
};

#endif  // STUDENT_PROCESSOR_TIMER_H
