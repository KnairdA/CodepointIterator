cmake_minimum_required(VERSION 2.8)
project(CodepointIterator)

set(
	CMAKE_CXX_FLAGS
	"-std=c++11 -W -Wall -Wextra -Winline -pedantic"
)

include_directories(
	src/
)

add_executable(
	test
	test.cc
	src/utility.cc
	src/codepoint_iterator.cc
)

target_link_libraries(
	test
	gtest
)

add_custom_command(
	TARGET test
	POST_BUILD COMMAND ./test
)
