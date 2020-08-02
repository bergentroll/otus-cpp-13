#pragma once

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

#include "solver.hpp"

namespace otus {
  inline std::string format_join_result (Solver::JoinType &input) {
    std::sort(input.begin(), input.end(), [](auto const &a, auto const &b) {
          return std::get<0>(a) < std::get<0>(b);
        });

    std::string result { };

    for (auto line: input) {
      result += std::to_string(std::get<0>(line));
      result += " | ";
      result += std::get<1>(line);
      result += " | ";
      result += std::get<2>(line);
      result += '\n';
    }
    return result;
  }

  // TODO test
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
        return handleIntersection(tokens);
      }
      else if (tokens[0] == "SYMMETRIC_DIFFERENCE") {
        return handleSymmetricDifference(tokens);
      }

      return "Error: unknown command \"" + tokens[0] + "\"\n";
  }

  private:
    Solver solver { };

    std::string handleIntersection(std::vector<std::string> const &tokens) {
      if (tokens.size() != 1)
        return "Error: command \"INTERSECTION\" takes no args\n";

      auto intersection { solver.intersection() };

      return format_join_result(intersection);
    }

    std::string handleSymmetricDifference(std::vector<std::string> const &tokens) {
      if (tokens.size() != 1)
        return "Error: command \"SYMMETRIC_DIFFERENCE\" takes no args\n";

      auto symDiff { solver.symmetricDifference() };

      return format_join_result(symDiff);
    }
  };
}
