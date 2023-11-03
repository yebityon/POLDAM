cmake_minimum_required(VERSION 3.10)
project(Poldam)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR})

# Collect all cpp files in poldam directory
file(GLOB_RECURSE POLDAM_SOURCES "poldam/graph/*.cpp")
file(GLOB_RECURSE POLDAM_SOURCES "poldam/parser/*.cpp")
file(GLOB_RECURSE POLDAM_SOURCES "poldam/util/*.cpp")

# Add poldam library
add_library(poldam ${POLDAM_SOURCES})

# Collect all cpp files in poldam/script directory
file(GLOB_RECURSE SCRIPT_SOURCES "poldam/script/*.cpp")

# Add executable for each source file
foreach(SOURCE ${SCRIPT_SOURCES})
    get_filename_component(EXECUTABLE ${SOURCE} NAME_WE)
    add_executable(${EXECUTABLE} ${SOURCE})
    target_link_libraries(${EXECUTABLE} poldam)
endforeach()