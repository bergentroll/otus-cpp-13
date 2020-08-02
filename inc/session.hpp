#pragma once

#include <algorithm>
#include <boost/asio.hpp>
#include <memory>

#include "handler.hpp"

namespace otus {
  class Session: public std::enable_shared_from_this<Session> {
  public:
    Session(boost::asio::ip::tcp::socket &&socket, Handler &handler):
    socket(std::move(socket)), handler(handler) { }

    void start() { doRead(); }

  private:
    void doRead();
    void doWrite(std::string const &message);

    boost::asio::ip::tcp::socket socket;
    Handler &handler;
    boost::asio::streambuf buffer { };
  };
}
