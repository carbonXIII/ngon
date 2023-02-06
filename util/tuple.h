#pragma once

#include <tuple>

namespace util {
  constexpr auto tuple_for_each(auto&& tuple, auto&& func) {
    [&]<std::size_t... I>(std::index_sequence<I...>) {
      (((std::forward<decltype(func)>(func))(I, std::get<I>(std::forward<decltype(tuple)>(tuple))), false) || ...);
    }(std::make_index_sequence<std::tuple_size_v<std::decay_t<decltype(tuple)>>>{});
  }
}
