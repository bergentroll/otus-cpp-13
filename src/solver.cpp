#include "solver.hpp"

using namespace std;
using namespace otus;

void Solver::insert(string const &tableName, int id, string const &name) {
  auto [iter, success] { getTableByName(tableName).insert({ id, name }) };
  if (!success)
    throw InvalidOperation("duplicate " + to_string((*iter).first));
}

Solver::JoinType Solver::intersection() const {
  JoinType result { };
  for (auto const &[id, name]: a) {
    try { result.push_back({ id, name, b.at(id) }); }
    catch (out_of_range const &) { }
  }
  return result;
}

Solver::JoinType Solver::symmetricDifference() const {
  JoinType result { };
  for (auto const &[id, name]: a)
      if (b.count(id) == 0) result.push_back({ id, name, "" });
  for (auto const &[id, name]: b)
      if (a.count(id) == 0) result.push_back({ id, "", name });
  return result;
}

Solver::TableType & Solver::getTableByName(string const &tableName) {
  if (tableName == "A") return a;
  else if (tableName == "B") return b;
  else throw InvalidOperation("unknown table \"" + tableName + '"');
}
