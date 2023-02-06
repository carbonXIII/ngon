#include "ahocorasick.h"

#include <string_view>

using namespace std::literals::string_view_literals;

static constexpr auto acf = ahocorasick::compile<256>(std::tuple {
  "this"sv,
  "is"sv,
  "a"sv,
  "test"sv
});

#include <iostream>

int main() {
  std::string test = "this is a cool test.";

  ahocorasick::StateMachine<acf> sm;
  for(auto ch: test) {
    sm.push(ch);
    std::cout << sm.term() << std::endl;
  }

  return 0;
}
