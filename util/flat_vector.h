#pragma once

#include <array>
#include <type_traits>

namespace util {
  template <typename T, size_t N>
  struct flat_vector {
    static_assert(std::is_standard_layout_v<T>
                  && std::is_default_constructible_v<T>);

    static constexpr auto slot_size = std::alignment_of_v<T>;

    std::array<T, N> buf {};
    std::size_t sz = 0;

    constexpr flat_vector() = default;
    constexpr flat_vector(size_t sz) { resize(sz); }

    constexpr auto size() { return sz; }

    constexpr void push_back(const T& t) {
      ++sz;
    }

    constexpr void emplace_back(auto&&... args) {
      buf[sz] = T(std::forward<decltype(args)>(args)...);
      ++sz;
    }

    constexpr void resize(std::size_t new_sz) {
      while(sz < new_sz) emplace_back();
      while(sz > new_sz) pop_back();
    }

    constexpr void pop_back() {
      sz--;
    }

    constexpr T& operator[](size_t idx) { return buf[idx]; }
    constexpr const T& operator[](size_t idx) const { return buf[idx]; }

    auto begin() { return &buf[0]; }
    auto end() { return &buf[sz]; }
    const auto begin() const { return &buf[0]; }
    const auto end() const { return &buf[sz]; }
  };
}
