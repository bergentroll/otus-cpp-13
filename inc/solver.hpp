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

    void insert(std::string const &tableName, int id, std::string const &name);

    void truncate(std::string const &tableName) {
      getTableByName(tableName).clear();
    }

    void truncateA() { a.clear(); }
    void truncateB() { b.clear(); }

    JoinType intersection() const;
    JoinType symmetricDifference() const;

  private:
    using TableType = std::unordered_map<int, std::string>;

    TableType a { }, b { };

    TableType & getTableByName(std::string const &tableName);
  };
}
