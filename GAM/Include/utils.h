#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <utility>
#include <cmath>
#include <ranges>
#include <iomanip>

namespace GAM
{
using ScalarType= double;
using IndexType= size_t;

struct HashEdgePair
{
  IndexType operator()(const std::pair<IndexType, IndexType>& P) const 
  {
    return P.first + P.second; 
  }
};

struct Vector;
struct Vertex;
}

namespace utils
{
// Thanks to https://medium.com/@batteriesnotincludeddev/indexed-for-each-in-modern-c-7df21fce72a1
auto enumerate(const auto& data) {
  return data | std::views::transform([i = 0](const auto& value) mutable {
    return std::make_pair(i++, value);
  });
}

template<typename ...Args>
void log(Args && ...args)
{
  (std::cout << ... << args);
}

template<typename ...Args>
void logln(Args && ...args)
{
  (std::cout << ... << args);
  std::cout << '\n';
}

template<typename ...Args>
void error(Args && ...args)
{
  (std::cerr << ... << args);
  std::cout << std::endl;
}
} // namespace utils
