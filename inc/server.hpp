#pragma once

#include <boost/algorithm/string.hpp>

#include <boost/bind.hpp>

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "solver.hpp"

namespace otus {
  using boost::asio::ip::tcp;
  using boost::asio::streambuf;

  class Session: public std::enable_shared_from_this<Session> {
  public:
    Session(tcp::socket &&socket, Solver &solver):
    socket(std::move(socket)), solver(solver) { }

    void start() {
      doRead();
    }

  private:
    // FIXME to cpp
    // FIXME EOF handling
    void doRead() {
      auto self(shared_from_this());

      boost::asio::async_read_until(
          socket,
          buffer,
          '\n',
          [this, self](auto const &ec, size_t bytesTransferred) {
            if (!ec) {
            buffer.commit(bytesTransferred);
            boost::asio::streambuf::const_buffers_type bufs { buffer.data() };
            std::string s(
                boost::asio::buffers_begin(bufs),
                boost::asio::buffers_begin(bufs) + bytesTransferred - 1); // Eliminate \n
            buffer.consume(bytesTransferred);

            std::cerr << "LINE: " << s << std::endl; // TODO

            // TODO Handler

            doWrite("MESSAGE!\n");
            }
            else {
              std::cerr << "Error while reading socket: " << ec.message() << std::endl;
            }
          });
    }

    void doWrite(std::string const &message) {
      auto self(shared_from_this());

      std::cerr << "doWrite()" << std::endl;
      socket.async_write_some(
          boost::asio::buffer(message),
          [this, self](auto const &ec, size_t) {
            if (!ec) doRead();
            else std::cerr << "Error while writing to socket: " << ec.message() << std::endl;
          });
    }

    tcp::socket socket;
    Solver &solver;
    streambuf buffer { };
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
