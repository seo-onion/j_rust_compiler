cmake_minimum_required(VERSION 3.27)
project(Miniproyecto2025_2)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Miniproyecto2025_2
        ast.cpp
        ast.h
        main.cpp
        parser.cpp
        parser.h
        scanner.cpp
        scanner.h
        token.cpp
        token.h
        visitor.cpp
        visitor.h)
