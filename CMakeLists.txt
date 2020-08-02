cmake_minimum_required(VERSION 3.10.2)

if($ENV{TRAVIS_BUILD_NUMBER})
  set(VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
  set(VERSION 0.0.0)
endif()
project(join_server VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Dependencies ##############################################################

set(SRC_PATH "src/")
set(TEST_PATH "test/")
include_directories("inc/")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(
  Boost 1.65
  COMPONENTS program_options system
  OPTIONAL_COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRECTORIES})

find_package(Threads REQUIRED)

### Main targets ##############################################################

add_library(
  ${CMAKE_PROJECT_NAME}-obj
  ${SRC_PATH}/handler.cpp
  ${SRC_PATH}/server.cpp
  ${SRC_PATH}/session.cpp)
target_link_libraries(
  ${CMAKE_PROJECT_NAME}-obj
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(
  ${CMAKE_PROJECT_NAME}
  ${SRC_PATH}/main.cpp)
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  ${CMAKE_PROJECT_NAME}-obj)

### Tests #####################################################################

if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
  add_executable(test-suite ${TEST_PATH}/test.cpp)

  target_link_libraries(
    test-suite
    PRIVATE
    ${CMAKE_PROJECT_NAME}-obj
    Boost::unit_test_framework)
  add_test(test-suite test-suite)
else()
  message(
    WARNING "Test suite will not be launched because Boost.Test not found.")
endif()

enable_testing()

### Packing ###################################################################

install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME}-server)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT bergentroll@insiberia.net)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
