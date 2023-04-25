cmake_minimum_required(VERSION 3.25)
project(lab1 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(lab1
        src/shell.c
        src/shell.h
        src/sim.c src/sim.h)
