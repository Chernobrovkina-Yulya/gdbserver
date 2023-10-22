cmake_minimum_required(VERSION 3.0)

project(gdb_server)
set(SRC src/main.c)

add_executable(main ${SRC})