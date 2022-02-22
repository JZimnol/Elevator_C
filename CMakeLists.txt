cmake_minimum_required(VERSION 3.21)
project(Elevator C)

set(CMAKE_C_STANDARD 99)

add_executable(Elevator main.c elevator.h elevator.c)
