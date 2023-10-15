cmake_minimum_required(VERSION 3.10)
project(POLDAM)

# Set your directories
set(METAFILE_DIR MetafileParser)
set(METAFILE_INCLUDE_DIR MetafileParser/include)
set(METAFILE_SRC_DIR MetaFileParser/src)

set(INTERPRETER_DIR Interpreter)
set(INTERPRETER_SRC_DIR Interpreter/src)

set(GRAPH_DIR Graph)
set(GRAPH_SRC_DIR ${GRAPH_DIR}/src)

set(UTIL_DIR Util)
set(UTIL_INCLUDE_DIR ${UTIL_DIR}/include/src)
set(UTIL_INCLUDE_TEST_DIR ${UTIL_DIR}/include/test)

set(BOOST_PATH /Users/prebell/Documents/boost_1_80_0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add your include directories
include_directories(${BOOST_PATH})
include_directories(${METAFILE_INCLUDE_DIR})
include_directories(${UTIL_INCLUDE_DIR})

# Create object libraries
add_library(util OBJECT ${UTIL_INCLUDE_DIR}/poldam_util.cpp)
file(GLOB_RECURSE LIB_METAFILE_PARSER ${METAFILE_SRC_DIR}/*.cpp)
file(GLOB_RECURSE LIB_INTERPRETER_PARSER ${INTERPRETER_SRC_DIR}/*.cpp)
file(GLOB_RECURSE LIB_POLDAMGRAPH_PARSER ${GRAPH_SRC_DIR}/*.cpp)
add_library(metafileParser ${LIB_METAFILE_PARSER})
add_library(interpreter OBJECT ${LIB_INTERPRETER_PARSER})
add_library(poldamGraph OBJECT ${LIB_POLDAMGRAPH_PARSER})

# Add main executable
add_executable(POLDAM main.cpp)
target_link_libraries(POLDAM util metafileParser interpreter poldamGraph)

# Add test executable
# add_executable(test_util ${UTIL_INCLUDE_TEST_DIR}/test_poldam_util.cpp)
# target_link_libraries(test_util util)
