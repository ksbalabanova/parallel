cmake_minimum_required(VERSION 3.16)

project(parallel_labs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(lab1)
#add_subdirectory(lab2)
#add_subdirectory(lab3)
#add_subdirectory(lab4)
#add_subdirectory(lab5)
#add_subdirectory(lab6)
#add_subdirectory(lab7)
#add_subdirectory(lab8)
