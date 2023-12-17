cmake_minimum_required(VERSION 3.0)

project(gdb_server)

add_executable(server src/tcp-connection.cpp src/main.cpp src/packet.cpp src/request.cpp src/utility.cpp)