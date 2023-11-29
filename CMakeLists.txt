cmake_minimum_required(VERSION 3.0)

project(gdb_server)

add_executable(server src/serial-connection/server_part.c src/packets.c src/requests.c)

add_executable(client src/serial-connection/client_part.c)