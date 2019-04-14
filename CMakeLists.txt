cmake_minimum_required(VERSION 3.13)
project(Filmes C)

set(CMAKE_C_STANDARD 99)

add_executable(Filmes main.c bool.h commands.h commands.c movie.h movie.c)