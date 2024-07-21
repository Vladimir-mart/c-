cmake_minimum_required(VERSION 3.12.4)
project(cpp)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(SetTest test.cpp Set.cpp)
target_link_libraries(SetTest Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})