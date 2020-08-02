#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <string>

#include "handler.hpp"

using namespace std;
using namespace otus;

BOOST_AUTO_TEST_SUITE(HandlerTest);

BOOST_AUTO_TEST_CASE(error_duplicate) {
  Handler handler { };
  string expected { "ERR duplicate 10\n" };

  handler.handleCommand("INSERT A 10 lean");
  auto result { handler.handleCommand("INSERT A 10 pumpkin") };

  BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(intersection) {
  Handler handler { };
  string expected {
    "0,lean,flour\n"
    "1,sweater,wonder\n"
    "OK\n"
  };

  handler.handleCommand("INSERT A 0 lean");
  handler.handleCommand("INSERT A 1 sweater");
  handler.handleCommand("INSERT A 2 frank");
  handler.handleCommand("INSERT B 0 flour");
  handler.handleCommand("INSERT B 1 wonder");
  handler.handleCommand("INSERT B 3 selection");
  auto result { handler.handleCommand("INTERSECTION") };

  BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(symmetric_difference) {
  Handler handler { };
  string expected {
    "2,frank,\n"
    "3,,selection\n"
    "OK\n"
  };

  handler.handleCommand("INSERT A 0 lean");
  handler.handleCommand("INSERT A 1 sweater");
  handler.handleCommand("INSERT A 2 frank");
  handler.handleCommand("INSERT B 0 flour");
  handler.handleCommand("INSERT B 1 wonder");
  handler.handleCommand("INSERT B 3 selection");
  auto result { handler.handleCommand("SYMMETRIC_DIFFERENCE") };

  BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_SUITE_END();
