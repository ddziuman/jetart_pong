#pragma once
#include <chrono>

class Timer {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using Seconds = std::chrono::duration<float>; // defaults to std::chrono::duration<float, std::ratio<1, 1>>, which means '1 tick (in double) is 1/1 of a second (1 second)'

private:
    TimePoint m_previousFrameStart {Clock::now()};
    Seconds m_deltaTime {};
    bool m_firstFrame {true};


public:
    float deltaTime() const;
    void beginFrame();
};