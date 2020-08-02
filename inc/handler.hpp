#pragma once

#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "solver.hpp"

namespace otus {
  class Handler {
  public:
    std::string handleCommand(std::string const &command) {
      std::vector<std::string> tokens;
      boost::split(tokens, command, [](char c) { return c == ' '; });

      std::transform(
          tokens[0].begin(),
          tokens[0].end(),
          tokens[0].begin(),
          ::toupper);

      if (tokens[0] == "INSERT") {
        if (tokens.size() != 4)
          return "Error: command \"INSERT\" takes exactly three args\n";
        return "insertion\n";
      }
      else if (tokens[0] == "TRUNCATE") {
        if (tokens.size() != 2)
          return "Error: command \"TRUNCATE\" takes exactly one arg\n";
        return "truncating\n";
      }
      else if (tokens[0] == "INTERSECTION") {
        if (tokens.size() != 1)
          return "Error: command \"INTERSECTION\" takes no args\n";
        return "intersection\n";
      }
      else if (tokens[0] == "SYMMETRIC_DIFFERENCE") {
        if (tokens.size() != 1)
          return "Error: command \"SYMMETRIC_DIFFERENCE\" takes no args\n";
        return "sym diff\n";
      }

      return "Error: unknown command \"" + tokens[0] + "\"\n";
    }

  private:
    Solver solver { };
  };
}
