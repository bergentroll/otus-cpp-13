#pragma once

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "thread_safe_table.hpp"

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

    void truncate(std::string const &tableName);

    JoinType intersection();
    JoinType symmetricDifference();

  private:
    using TableType = ThreadSafeTable;

    TableType a { }, b { };

    TableType & getTableByName(std::string const &tableName);
  };
}
