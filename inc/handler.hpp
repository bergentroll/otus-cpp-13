#pragma once

#include <string>

#include "solver.hpp"

namespace otus {
  std::string formatJoinResult (Solver::JoinType &input);

  class Handler {
  public:
    std::string handleCommand(std::string_view command);

  private:
    using Tokens = std::vector<std::string>;

    Solver solver { };

    std::string handleInsert(Tokens const &tokens);
    std::string handleTruncate(Tokens const &tokens);
    std::string handleIntersection(Tokens const &tokens);
    std::string handleSymmetricDifference(Tokens const &tokens);
  };
}
