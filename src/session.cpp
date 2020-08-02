#include <iostream>

#include "session.hpp"

using namespace std;
using namespace otus;
namespace io = boost::asio;

void Session::doRead() {
  auto self(shared_from_this());

  io::async_read_until(
      socket,
      buffer,
      '\n',
      [this, self](auto const &ec, size_t bytesTransferred) {
        if (!ec) {
        buffer.commit(bytesTransferred);
        io::streambuf::const_buffers_type bufs { buffer.data() };
        string s(
            io::buffers_begin(bufs),
            io::buffers_begin(bufs) + bytesTransferred - 1); // Eliminate \n
        buffer.consume(bytesTransferred);

        cerr << "LINE: " << s << endl; // TODO

        doWrite(handler.handleCommand(s));
        }
        else {
          cerr << "Error while reading socket: " << ec.message() << endl;
        }
      });
}

void Session::doWrite(string const &message) {
  auto self(shared_from_this());

  cerr << "doWrite()" << endl;
  socket.async_write_some(
      io::buffer(message),
      [this, self](auto const &ec, size_t) {
        if (!ec) doRead();
        else cerr << "Error while writing to socket: " << ec.message() << endl;
      });
}
