cmake_minimum_required(VERSION 3.0.0)
project(jc)

include_directories(src)

set(LIB_SRC
	src/javabuilder.cpp
	src/javafile.cpp
	src/javafunc.cpp
	src/javapool.cpp
	src/writer.cpp
)

add_library(jc ${LIB_SRC})

add_executable(test1 test1.cpp)
add_executable(test2 test2.cpp)
add_executable(test3 test3.cpp)
add_executable(test4 test4.cpp)

target_link_libraries(test1 jc)
target_link_libraries(test2 jc)
target_link_libraries(test3 jc)
target_link_libraries(test4 jc)
