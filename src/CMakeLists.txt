cmake_minimum_required(VERSION 3.26)
project(simple-software-renderer)

set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)

include_directories(${spdlog_INCLUDE_DIRS})

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
        logging.cpp
        logging.cpp
        logging.h
        argparse.cpp
        argparse.h
        IParserStrategy.h
        WaveFrontParserStrategy.h
        CSVParserStrategy.h
)
target_link_libraries(simple-software-renderer PRIVATE spdlog::spdlog)
