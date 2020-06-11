cmake_minimum_required(VERSION 3.0.0)
project(jc)

find_package(BISON)
find_package(FLEX)

include_directories(libjava compiler)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(LIB_libjava
	libjava/javabuilder.cpp
	libjava/javafile.cpp
	libjava/javafunc.cpp
	libjava/javapool.cpp
	libjava/writer.cpp
	libjava/builder/jfunccall.cpp
	libjava/builder/jvar.cpp
)

add_library(jc ${LIB_libjava})

BISON_TARGET(parser compiler/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.yy.cpp)
FLEX_TARGET(scanner compiler/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/parser.lex.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

set(PARSER_SRC
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS}
	compiler/compiler.cpp
	compiler/main.cpp
)

add_executable(jcp ${PARSER_SRC})

target_link_libraries(jcp jc)



