#pragma once

namespace GAM
{
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
} // namespace utils