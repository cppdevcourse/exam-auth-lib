cmake_minimum_required(VERSION 3.10)
project(auth_server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

add_executable(auth_server)
target_sources(auth_server PRIVATE
  main.cpp
  server.hpp
  server.cpp
  udp_socket.hpp
  udp_socket.cpp
)
