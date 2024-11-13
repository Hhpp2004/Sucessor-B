cmake_minimum_required(VERSION 3.22.1)
project(3_trabalho C)

set(CMAKE_C_STANDARD 11)

add_executable(3_trabalho main.c)

target_link_libraries(3_trabalho m)
target_compile_options(3_trabalho PRIVATE -Wall -Wextra -g)
