﻿# CMakeList.txt : CMake project for RoboCrypto, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)

project ("bypto")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

set(SOURCE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
