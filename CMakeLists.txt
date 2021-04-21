cmake_minimum_required(VERSION 3.17)
project(seeerver)

set(CMAKE_CXX_STANDARD 14)
find_package(Boost QUIET REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIR})

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost Not found")
endif()
add_executable(seeerver main.cpp Connections.cpp Connections.h TcpProxy.cpp TcpProxy.h Log.cpp Log.h SqlParser.cpp SqlParser.h)

target_link_libraries(seeerver PUBLIC Boost::program_options)