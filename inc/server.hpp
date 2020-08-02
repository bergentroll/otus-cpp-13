#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "session.hpp"

namespace otus {
  using boost::asio::ip::tcp;

  class Server {
  public:
    Server(boost::asio::io_service &context, int port):
    socket(context),
    acceptor(context, tcp::endpoint(tcp::v4(), port)) { doAccept(); }

  private:
    void doAccept();

    tcp::socket socket;
    tcp::acceptor acceptor;
    Solver solver { };
  };
}
