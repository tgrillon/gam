#pragma once

using namespace std::chrono_literals;

namespace utils
{
class Timer
{
public:
	void start();
	void stop();
	int64_t us() const;
	int64_t ms() const;
	void us(const std::string& label) const;
	void ms(const std::string& label) const;

private:
	std::chrono::high_resolution_clock::time_point m_start{};
	int64_t m_duration{ 0 };
};

} // namespace utils
