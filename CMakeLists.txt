cmake_minimum_required(VERSION 3.10)
project(auth_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

add_executable(auth_server)
target_sources(auth_server PRIVATE
  auth.cpp
  auth.hpp
  command.cpp
  command.hpp
  main.cpp
  server.cpp
  server.hpp
  udp_session.cpp
  udp_session.hpp
  udp_socket.cpp
  udp_socket.hpp
)
