#pragma once

#include "solver.hpp"

namespace otus {
  class Handler {
  public:
    std::string handleCommand(std::string const &command) {
      return "Handler::handleCommand(\"" + command + "\")\n";
    }

  private:
    Solver solver { };
  };
}
