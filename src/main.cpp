#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "server.hpp"

using namespace otus;
using namespace std;
namespace io = boost::asio;
namespace po = boost::program_options;
using 
  boost::any,
  boost::any_cast,
  boost::lexical_cast,
  boost::bad_lexical_cast;

BOOST_STRONG_TYPEDEF(int, PortType);

void validate(any& v, const vector<string>& values, PortType *, int) {
  po::validators::check_first_occurrence(v);
  const string &str { po::validators::get_single_string(values) };
  try {
    v = lexical_cast<PortType>(str);
  } catch (bad_lexical_cast const &e) {
    throw po::error("unexpected port value");
  }
  auto port { any_cast<PortType>(v) };
  if (port < 1 || port > 65535) throw po::error("invalid port number");
}

int main(int argc, char const **argv) {
  int port;

  try {
    po::positional_options_description positional_description { };
    positional_description.add("port", 1);

    po::options_description description { "Hidden options" };
    description.add_options()
      ("port", po::value<PortType>()->value_name("PORT")->required(), "port to listen on");

    po::variables_map options;

    po::store(
      po::command_line_parser(argc, argv)
        .positional(positional_description)
        .options(description)
        .run(),
      options);
    po::notify(options);

    port = options["port"].as<PortType>();

    cerr << "Port has been set to " << port << endl;
  } catch (po::error const &e) {
    cerr << "Options error: " << e.what() << endl;
    return EXIT_FAILURE;
  }

  io::io_service context;
  try {
    Server server { context, port };
    context.run();
  }
  catch (boost::system::system_error const &e) {
    cerr << "Server error: " << e.what() << endl;
  }

  return 0;
}
