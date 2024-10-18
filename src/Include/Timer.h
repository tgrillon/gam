#pragma once

#include "pch.h"

using namespace std::chrono_literals;

class Timer
{
public:
    void start();
    void stop();
    int us() const;
    int ms() const;
    void us(const std::string &label) const;
    void ms(const std::string &label) const;

private:
    std::chrono::high_resolution_clock::time_point m_start{};
    double m_duration{0.0};
};
