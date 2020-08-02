#include "server.hpp"

using namespace std;
using namespace otus;
namespace io = boost::asio;
namespace sys = boost::system;

void Server::doAccept() {
  acceptor.async_accept(
    socket,
    [this](sys::error_code ec) {
      if (!ec) std::make_shared<Session>(std::move(socket), handler)->start();
      doAccept();
    });
}
