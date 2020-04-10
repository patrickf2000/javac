cmake_minimum_required(VERSION 3.0.0)
project(jc)

include_directories(src)

set(LIB_SRC
	src/javafile.cpp
	src/javafunc.cpp
	src/javapool.cpp
	src/writer.cpp
)

add_library(jc ${LIB_SRC})

add_executable(test1 test1.cpp)

target_link_libraries(test1 jc)
