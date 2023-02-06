#pragma once

#include <cstdint>
#include <array>
#include <ostream>

namespace util {

  template <std::size_t N>
  struct bitset {
    using T = uint64_t;
    constexpr static auto bits = sizeof(T) * 8;
    std::array<T, (N + bits - 1) / bits> v {};

    constexpr bitset() = default;

    struct reference {
      bitset* parent;
      const std::size_t n;

      constexpr reference& operator=(bool v) {
        const uint64_t mask = (T{1} << (n % bits));
        parent->v[n / bits] =
          (parent->v[n / bits] & ~mask)
          ^ (v ? mask : 0);
        return *this;
      }

      constexpr operator bool() const {
        const uint64_t mask = (T{1} << (n % bits));
        return parent->v[n / bits] & mask;
      }

      constexpr bool operator~() const {
        const uint64_t mask = (T{1} << (n % bits));
        return !(parent->v[n / bits] & mask);
      }

      constexpr reference& flip() {
        const uint64_t mask = (T{1} << (n % bits));
        parent->v[n / bits] = parent->v[n / bits] ^ mask;
        return *this;
      }
    };

    struct const_reference {
      const bitset* parent;
      const std::size_t n;

      constexpr operator bool() {
        const uint64_t mask = (T{1} << (n % bits));
        return parent->v[n / bits] & mask;
      }

      constexpr bool operator~() {
        const uint64_t mask = (T{1} << (n % bits));
        return !(parent->v[n / bits] & mask);
      }
    };

    constexpr reference operator[](std::size_t n) { return {this, n}; }
    constexpr const_reference operator[](std::size_t n) const { return {this, n}; }

    constexpr bitset operator|(const bitset& o) const {
      bitset ret;
      for(int i = 0; i < v.size(); i++) {
        ret.v[i] = v[i] | o.v[i];
      }
      return ret;
    }

    friend std::ostream& operator<<(std::ostream& os, const bitset& b) {
      for(int i = 0; i < N; i++)
        os << b[i];
      return os;
    }
  };

}
