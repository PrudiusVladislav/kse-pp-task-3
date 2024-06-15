cmake_minimum_required(VERSION 3.28)
project(kse_pp_task_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(kse_pp_task_3 main.cpp)
add_library(kse_pp_task_3 SHARED encrypt.cpp)
