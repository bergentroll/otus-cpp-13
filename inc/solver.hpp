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

    class InvalidOperation: public std::runtime_error {
    public:
      InvalidOperation(std::string const &message):
      std::runtime_error(message) { }
    };

    class TableNameError: public std::runtime_error {
    public:
      TableNameError(std::string const &message):
      std::runtime_error(message) { }
    };

    void insert(std::string const &tableName, int id, std::string const &name) {
      auto [iter, success] { getTableByName(tableName).insert({ id, name }) };
      if (!success)
        throw InvalidOperation("duplicate " + std::to_string((*iter).first));
    }

    void truncate(std::string const &tableName) {
      getTableByName(tableName).clear();
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

    TableType & getTableByName(std::string const &tableName) {
      if (tableName == "A") return a;
      else if (tableName == "B") return b;
      else throw InvalidOperation("unknown table \"" + tableName + '"');
    }

    TableType a { }, b { };
  };
}
