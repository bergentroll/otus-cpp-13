#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "solver.hpp"

namespace otus {
  using boost::asio::ip::tcp;

  class Session: public std::enable_shared_from_this<Session> {
  public:
    Session(tcp::socket socket, Solver &solver):
    socket(std::move(socket)), solver(solver) { }

    void start() { doRead(); }

  private:
    void doRead() {
      auto self(shared_from_this());
      socket.async_read_some(
          boost::asio::buffer(data, maxLength),
          [this, self](boost::system::error_code ec, size_t length) {
            if (!ec) {
              try {
                request(std::string(data, length));
              }
              catch (...) {
              }
              doRead();
            }
          });
    }

    void request(std::string const &input) {
      std::vector<std::string> split { };
      //boost::split(split, input, ' ');
      //if (split[0] == "INSERT") {
      //  if (split[1] == "A") // FIXME
      //}
      //else if (split[0] == "TRUNCATE") {
      //}
      //std::cerr << split[0] << std::endl;
      std::cerr << "OLOLO" << std::endl;
    }

    tcp::socket socket;
    Solver &solver;
    static constexpr size_t maxLength { 1024 };
    char data[maxLength];
  };

  class Server {
  public:
    Server(boost::asio::io_service &context, int port):
    socket(context),
    acceptor(context, tcp::endpoint(tcp::v4(), port)) {
      doAccept();
    }

  private:
    void doAccept() {
      acceptor.async_accept(
          socket,
          [this](boost::system::error_code ec) {
            if (!ec) std::make_shared<Session>(std::move(socket), solver)->start();
            doAccept();
          });
    }

    tcp::socket socket;
    tcp::acceptor acceptor;
    Solver solver { };
  };
}
