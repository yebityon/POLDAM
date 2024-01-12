cmake_minimum_required(VERSION 3.10)
project(Poldam)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# MODIFY HERE
# TODO:BOOST_PATH should be set from environment variable
set(BOOST_PATH YOUR/BOOST_PATH)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${BOOST_PATH})

file(GLOB_RECURSE POLDAM_GRAPH "poldam/graph/*.cpp")
add_library(graph OBJECT  ${POLDAM_GRAPH})# Collect all cpp files in poldam directory

file(GLOB_RECURSE POLDAM_SELOGGER_LOG_PARSER "poldam/selogger_log_parser/*.cpp")
add_library(selogger_log_parser OBJECT  ${POLDAM_SELOGGER_LOG_PARSER})

file(GLOB_RECURSE POLDAM_UTIL "poldam/util/*.cpp")
add_library(util OBJECT ${POLDAM_UTIL})

file(GLOB_RECURSE POLDAM_HELPER "poldam/helper/*.cpp")
add_library(helper OBJECT ${POLDAM_HELPER})

# Collect all cpp files in poldam/script directory
file(GLOB_RECURSE SCRIPT_SOURCES "poldam/script/*.cpp")

# Add executable for each source file
foreach(SOURCE ${SCRIPT_SOURCES})
    get_filename_component(EXECUTABLE ${SOURCE} NAME_WE)
    add_executable(${EXECUTABLE} ${SOURCE})
    target_link_libraries(${EXECUTABLE} util selogger_log_parser helper graph)
    # MacOS sonomaでlinkerのエラーが出るため一時的に対処
    set_target_properties(${EXECUTABLE} PROPERTIES LINK_FLAGS "-Xlinker -ld_classic")
endforeach()