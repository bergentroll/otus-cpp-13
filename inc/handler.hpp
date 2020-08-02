#pragma once

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

#include "solver.hpp"

namespace otus {
  class Handler {
  public:
    std::string handleCommand(std::string_view command) {
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

        int id;
        try {
          id = boost::lexical_cast<int>(tokens[2]);
        }
        catch (boost::bad_lexical_cast const &e) {
          return "Error: invalid id \"" + tokens[2] + "\"\n";
        }

        auto const &name { tokens[3] };

        if (tokens[1] == "A") {
          solver.insertToA(id, name);
          return "OK\n";
        }
        else if (tokens[1] == "B") {
          solver.insertToB(id, name);
          return "OK\n";
        }
        else return "Unknown table \"" + tokens[1] + "\"\n";;
      }
      else if (tokens[0] == "TRUNCATE") {
        if (tokens.size() != 2)
          return "Error: command \"TRUNCATE\" takes exactly one arg\n";
        else if (tokens[1] == "A") {
          solver.truncateA();
          return "OK\n";
        }
        else if (tokens[1] == "B") {
          solver.truncateB();
          return "OK\n";
        }
        else return "Unknown table \"" + tokens[1] + "\"\n";;
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
