#include "solver.hpp"

using namespace std;
using namespace otus;

void Solver::insert(string const &tableName, int id, string const &name) {
  auto &target { getTableByName(tableName) };
  auto [iter, success] { target.getLocked().insert({ id, name }) };
  target.unlock();

  if (!success)
    throw InvalidOperation("duplicate " + to_string((*iter).first));
}

void Solver::truncate(string const &tableName) {
  auto &target { getTableByName(tableName) };
  target.getLocked().clear();
  target.unlock();
}

Solver::JoinType Solver::intersection() {
  JoinType result { };
  auto const &tableA { a.getSharedLocked() };
  auto const &tableB { b.getSharedLocked() };

  for (auto const &[id, name]: tableA) {
    try { result.push_back({ id, name, tableB.at(id) }); }
    catch (out_of_range const &) { }
  }

  a.sharedUnlock();
  b.sharedUnlock();
  return result;
}

Solver::JoinType Solver::symmetricDifference() {
  JoinType result { };
  auto const &tableA { a.getSharedLocked() };
  auto const &tableB { b.getSharedLocked() };

  for (auto const &[id, name]: tableA)
      if (tableB.count(id) == 0) result.push_back({ id, name, "" });
  for (auto const &[id, name]: tableB)
      if (tableA.count(id) == 0) result.push_back({ id, "", name });

  a.sharedUnlock();
  b.sharedUnlock();
  return result;
}

Solver::TableType & Solver::getTableByName(string const &tableName) {
  if (tableName == "A") return a;
  else if (tableName == "B") return b;
  else throw InvalidOperation("unknown table \"" + tableName + '"');
}
