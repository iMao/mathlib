cmake_minimum_required(VERSION 3.5)

project(mathlib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(HDR 
    mat.h
    mat_exceptions.h
    mat_operations.h
)

set(SRC 
    mat.cpp
    mat_operations.cpp
)


add_executable(mathlib ${HDR} ${SRC} main.cpp)
