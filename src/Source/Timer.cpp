#include "Timer.h"

#include "Utils.h"

void Timer::start()
{
    m_start = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    auto stop = std::chrono::high_resolution_clock::now();
    m_duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(stop - m_start).count());
}

int Timer::us() const
{
    return static_cast<int>(m_duration) % 1000;
}

int Timer::ms() const
{
    return m_duration / 1000;
}

void Timer::us(const std::string &label) const
{
    utils::info("[Timer] ", label, " ", us());
}

void Timer::ms(const std::string &label) const
{
    utils::info("[Timer] ", label, " ", ms());
}
