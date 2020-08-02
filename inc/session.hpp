#pragma once

#include <algorithm>
#include <boost/asio.hpp>
#include <memory>

#include "solver.hpp"

namespace otus {
  class Session: public std::enable_shared_from_this<Session> {
  public:
    Session(boost::asio::ip::tcp::socket &&socket, Solver &solver):
    socket(std::move(socket)), solver(solver) { }

    void start() { doRead(); }

  private:
    // FIXME EOF handling
    void doRead();
    void doWrite(std::string const &message);

    boost::asio::ip::tcp::socket socket;
    Solver &solver;
    boost::asio::streambuf buffer { };
  };
}
