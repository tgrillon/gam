
#include "Timer.h"

#include "PCH/PCH.h"

#include "Utils.h"

namespace utils
{
void Timer::start()
{
	m_start = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
	auto stop = std::chrono::high_resolution_clock::now();
	m_duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - m_start).count();
}

int64_t Timer::us() const
{
	return m_duration % 1000;
}

int64_t Timer::ms() const
{
	return m_duration / 1000;
}

void Timer::us(const std::string& label) const
{
	utils::info("[Timer] ", label, " ", us());
}

void Timer::ms(const std::string& label) const
{
	utils::info("[Timer] ", label, " ", ms());
}
} // namespace utils
