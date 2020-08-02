#pragma once

#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace otus {
  class Solver {
  public:
    using JoinType =
      std::vector<std::tuple<int, std::string, std::string>>;

    class DuplicateError: public std::runtime_error {
    public:
      DuplicateError(std::string const &message):
      std::runtime_error(message) { }
    };

    // TODO Refactor insertion
    void insertToA(int id, std::string const &name) {
      auto [iter, success] { a.insert({ id, name }) };
      if (!success)
        throw DuplicateError("duplicate " + std::to_string((*iter).first));
    }

    void insertToB(int id, std::string const &name) {
      auto [iter, success] { b.insert({ id, name }) };
      if (!success)
        throw DuplicateError("duplicate " + std::to_string((*iter).first));
    }

    void truncateA() { a.clear(); }

    void truncateB() { b.clear(); }

    JoinType intersection() const {
      JoinType result { };
      for (auto const &[id, name]: a) {
        try { result.push_back({ id, name, b.at(id) }); }
        catch (std::out_of_range const &) { }
      }
      return result;
    }

    JoinType symmetricDifference() const {
      JoinType result { };
      for (auto const &[id, name]: a)
          if (b.count(id) == 0) result.push_back({ id, name, "" });
      for (auto const &[id, name]: b)
          if (a.count(id) == 0) result.push_back({ id, "", name });
      return result;
    }

  private:
    using TableType = std::unordered_map<int, std::string>;

    TableType a { }, b { };
  };
}
