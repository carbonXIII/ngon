#pragma once

#include <vector>
#include <queue>
#include <array>
#include <tuple>

#include <util/tuple.h>
#include <util/bitset.h>
#include <util/flat_vector.h>

namespace ahocorasick {
  static constexpr std::size_t MaxNodes = 1024;

  template <std::size_t E, std::size_t M>
  struct Node {
    std::array<std::size_t, E> next {};
    util::bitset<M> term {};
  };

  template <std::size_t E, std::size_t M>
  using TransitionTable = util::flat_vector<Node<E, M>, MaxNodes>;

  template <std::size_t E>
  constexpr auto compile(const auto& rules) {
    constexpr auto M = std::tuple_size_v<std::decay_t<decltype(rules)>>;

    std::vector<Node<E, M>> trie;
    trie.emplace_back();

    auto lambda = [&](std::size_t idx, auto& rule) {
      std::size_t node = 0;
      for(auto sym: rule) {
        if(!trie[node].next[sym]) {
          trie[node].next[sym] = trie.size();
          trie.emplace_back();
        }
        node = trie[node].next[sym];
      }

      trie[node].term[idx] = 1;
    };

    util::tuple_for_each(rules, lambda);

    TransitionTable<E, M> out(trie.size());

    // std::queue doesn't have a constexpr constructor yet
    std::vector<std::pair<std::size_t, std::size_t>> q;
    q.push_back({0, 0});

    for(int i = 0; i < q.size(); i++) {
      auto [node, slink] = q[i];

      out[node].term = trie[node].term | trie[slink].term;

      for(std::size_t sym = 0; sym < E; sym++) {
        if(trie[node].next[sym]) {
          out[node].next[sym] = trie[node].next[sym];
          q.push_back({
              trie[node].next[sym],
              node == 0 ? 0 : out[slink].next[sym]
            });
        } else {
          out[node].next[sym] = trie[slink].next[sym];
        }
      }
    }

    return out;
  }

  template <auto compiled_rules>
  struct StateMachine {
    void push(int sym) {
      history.push_back(compiled_rules[history.back()].next[sym]);
    }

    template <int N = 1>
    void pop_suffix() {
      history.resize(history.size() - N);
    }

    auto term() {
      return compiled_rules[history.back()].term;
    }

    std::vector<int> history {0};
  };
};
