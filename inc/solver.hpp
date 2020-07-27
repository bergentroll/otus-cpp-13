#pragma once

#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace otus {
  class Solver {
  public:
    // TODO std::string_view
    using IntersectionType =
      std::vector<std::tuple<int, std::string, std::string>>;

    bool insertToA(int id, std::string const &name) {
      return a.insert({ id, name }).second;
    }

    bool insertToB(int id, std::string const &name) {
      return b.insert({ id, name }).second;
    }

    void truncateA() { a.clear(); }

    void truncateB() { b.clear(); }

    IntersectionType intersection() const {
      IntersectionType result { };
      for (auto const &[id, name]: a) {
        try { result.push_back({ id, name, b.at(id) }); }
        catch (std::out_of_range const &) { }
      }
      return result;
    }

    IntersectionType symmetricDifference() const {
      IntersectionType result { };
      for (auto const &[id, name]: a)
          if (b.count(id) == 0) result.push_back({ id, name, "" });
      for (auto const &[id, name]: b)
          if (a.count(id) == 0) result.push_back({ id, "", name });
      return result;
    }

  private:
    using TableType = std::unordered_map<int, std::string>;

    TableType a {}, b {};
  };
}
