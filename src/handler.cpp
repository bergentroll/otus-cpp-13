#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

#include "handler.hpp"

using namespace std;
using namespace otus;

string Handler::handleCommand(string_view command) {
  Tokens tokens;
  boost::split(tokens, command, [](char c) { return c == ' '; });

  transform(
      tokens[0].begin(),
      tokens[0].end(),
      tokens[0].begin(),
      ::toupper);

  if (tokens[0] == "INSERT") {
    return handleInsert(tokens);
  }
  else if (tokens[0] == "TRUNCATE") {
    return handleTruncate(tokens);
  }
  else if (tokens[0] == "INTERSECTION") {
    return handleIntersection(tokens);
  }
  else if (tokens[0] == "SYMMETRIC_DIFFERENCE") {
    return handleSymmetricDifference(tokens);
  }

  return "ERR unknown command \"" + tokens[0] + "\"\n";
}

string Handler::handleInsert(Tokens const &tokens) {
  if (tokens.size() != 4)
    return "ERR command \"INSERT\" takes exactly three args\n";

  int id;
  try {
    id = boost::lexical_cast<int>(tokens[2]);
  }
  catch (boost::bad_lexical_cast const &e) {
    return "ERR invalid id \"" + tokens[2] + "\"\n";
  }

  string const &tableName { tokens[1] };
  string const &name { tokens[3] };

  try {
    solver.insert(tableName, id, name);
  }
  catch (Solver::InvalidOperation const &e) {
    return "ERR " + string(e.what()) + '\n';
  }
  return "OK\n";
}

string Handler::handleTruncate(Tokens const &tokens) {
  if (tokens.size() != 2)
    return "ERR command \"TRUNCATE\" takes exactly one arg\n";
  else {
    string const &tableName { tokens[1] };
    try {
      solver.truncate(tableName);
    }
    catch (Solver::InvalidOperation const &e) {
      return "ERR " + string(e.what()) + '\n';
    }
  }
  return "OK\n";
}

string Handler::handleIntersection(Tokens const &tokens) {
  if (tokens.size() != 1)
    return "ERR command \"INTERSECTION\" takes no args\n";

  auto intersection { solver.intersection() };

  return formatJoinResult(intersection);
}

string Handler::handleSymmetricDifference(Tokens const &tokens) {
  if (tokens.size() != 1)
    return "ERR command \"SYMMETRIC_DIFFERENCE\" takes no args\n";

  auto symDiff { solver.symmetricDifference() };

  return formatJoinResult(symDiff);
}

string otus::formatJoinResult (Solver::JoinType &input) {
  sort(input.begin(), input.end(), [](auto const &a, auto const &b) {
      return get<0>(a) < get<0>(b);
      });

  stringstream result { };

  for (auto line: input) {
    auto id { to_string(get<0>(line)) };
    auto const &nameA { get<1>(line) };
    auto const &nameB { get<2>(line) };

    result << id << ',' << nameA << ',' << nameB << '\n';
  }

  result << "OK\n";

  return result.str();
}
