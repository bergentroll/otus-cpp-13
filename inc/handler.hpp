#pragma once

#include <string>

#include "solver.hpp"

namespace otus {
  std::string formatJoinResult (Solver::JoinType &input);

  // TODO test
  class Handler {
  public:
    std::string handleCommand(std::string_view command);

  private:
    Solver solver { };

    std::string handleIntersection(std::vector<std::string> const &tokens);
    std::string handleSymmetricDifference(std::vector<std::string> const &tokens);
  };
}
