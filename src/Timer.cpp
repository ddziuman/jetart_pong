#include "Timer.h"
#include <thread>
#include <chrono>

float Timer::deltaTime() const {
    return m_deltaTime.count();
}

void Timer::beginFrame() {
    TimePoint newFrameStart {Clock::now()};
    if (m_firstFrame) {
        m_deltaTime = std::chrono::duration_cast<Seconds>(newFrameStart - newFrameStart);
        m_firstFrame = false;
    } else {
        m_deltaTime = std::chrono::duration_cast<Seconds>(newFrameStart - m_previousFrameStart);
    }
    m_previousFrameStart = newFrameStart;
}