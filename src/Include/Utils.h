#pragma once

namespace gam
{
using scalar_t = float;
using index_t = uint32_t;

namespace IndexHelpers
{
uint8_t Next[3]{ 1, 2, 0 };
uint8_t Current[3]{ 0, 1, 2 };
uint8_t Previous[3]{ 2, 0, 1 };
} // namespace IndexHelpers

struct HashEdgePair
{
	index_t operator()(const std::pair<index_t, index_t>& P) const { return P.first + P.second; }
};
} // namespace gam

namespace utils
{
// Thanks to https://medium.com/@batteriesnotincludeddev/indexed-for-each-in-modern-c-7df21fce72a1
auto enumerate(const auto& data)
{
	return data
		| std::views::transform(
			   [i = 0](const auto& value) mutable
			   {
				   return std::make_pair(i++, value);
			   });
}

template<typename... Args>
void print(Args&&... args)
{
	(std::cout << ... << args);
}

template<typename... Args>
void message(Args&&... args)
{
	(std::cout << ... << args);
	std::cout << '\n';
}

template<typename... Args>
void status(Args&&... args)
{
	std::cout << "-- [STATUS] ";
	(std::cout << ... << args);
	std::cout << " --\n";
}

template<typename... Args>
void info(Args&&... args)
{
	std::cout << "## [Info] ";
	(std::cout << ... << args);
	std::cout << " ##\n";
}

template<typename... Args>
void debug(Args&&... args)
{
	std::cout << "## [DEBUG] ";
	(std::cout << ... << args);
	std::cout << " ##\n";
}

template<typename... Args>
void error(Args&&... args)
{
	std::cerr << "!! [ERROR] ";
	(std::cerr << ... << args);
	std::cout << " !!" << std::endl;
}

std::vector<Point> read_point_set(const std::string& filename, float x_scale = 1.0, float y_scale = 1.0, float z_scale = 1.0);
} // namespace utils
