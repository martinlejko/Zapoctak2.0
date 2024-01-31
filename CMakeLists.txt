cmake_minimum_required(VERSION 3.26)
project(Zapoctak2_0)

set(CMAKE_CXX_STANDARD 20)

add_executable(Zapoctak2_0 main.cpp
        tgaimage.cpp
        tgaimage.h
        parser.cpp
        parser.h
        datatypes.cpp
        datatypes.h
        model.cpp
        model.h
)
