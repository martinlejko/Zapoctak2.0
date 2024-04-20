cmake_minimum_required(VERSION 3.26)
project(simple-software-renderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(simple-software-renderer main.cpp
        tgaimage.cpp
        tgaimage.h
        parser.cpp
        parser.h
        datatypes.cpp
        datatypes.h
        model.cpp
        model.h
        render.cpp
        render.h
)
