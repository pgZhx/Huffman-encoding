cmake_minimum_required(VERSION 3.26)
project(Huffman_Coding)

set(CMAKE_CXX_STANDARD 23)

include_directories(D:/Document/Code/Libraries/boost_1_83_0)
link_directories(D:/Document/Code/Libraries/boost_1_83_0/stage/lib)

add_executable(Huffman_Coding main.cpp
        Info.h
        TXT.cpp
        TXT.h
        BIN.cpp
        BIN.h)
