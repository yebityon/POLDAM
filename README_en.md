# POLDAM

## abstract

This script is a concrete implementation of the script used in [実行トレースのマークル木を用いたプログラム変更前後の差分検出法の提案](https://library.naist.jp/dspace/handle/10061/14739).

It takes execution traces obtained from [selogger](https://github.com/takashi-ishio/selogger), as input, constructs a Merkle tree, and can output it as a `dot` file.

## Repository Structure 
The specific implementations are contained within the `poldam/` directory. 
This includes the program `poldam/selogger_log_parser` for parsing selogger logs, the API `poldam/graph` for representing Merkle trees, and the helper program `poldam/helper` used in the implementation of the paper.

The `poldam/script` directory is intended for creating scripts that utilize these libraries. Programs created under this directory can link the libraries and generate executable files during the build process using CMake.

test.cpp is a sample implementation from the paper that uses `poldam/selogger_log_parser`, `poldam/helper`, and `poldam/graph`.
## tree

```bash
.
├── CMakeLists.txt
├── Data
│   └── java8 #selogger and java program for sample
├── Doxyfile
├── Doxyfile.bak
├── poldam
│    ├── graph
│    ├── helper
│    ├── script
│    ├── selogger_log_parser
│    └── util
├── Makefile
├── README.md
├── build
├── docker-compose.yml
├── dockerfiles
│   ├── c++
│   └── java8
├── docs
└── poldam_sample.sh 
```

## Description
- Data
  - This directory contains the selogger and Java Motivation Example. docker-compose mounts this directory in the Java container.
- dockerfiles
  - Contains the Dockerfile for Java 8.
- poldam/graph
  - Provides a Definition, an algorithm to build and compare a Merkle tree.
- poldam/helper
  - Provides a helper program to build a graph, i.e., an example program that combines selogger_log_parser and graph.
- poldam/script
  - With the default configuration, CMake builds the cpp files located in this directory and links them with poldam/helper, poldam/graph, and poldam/util. To create your own script using these libraries, place your program in this directory.
- poldam/selogger_log_parser
  - Provides a parser that takes the log directory output by selogger as input, interprets the files in that directory, and outputs necessary information.
  - Contains definitions for selogger event types, among other things.
- poldam/util
  - Provides generic classes and functions used in these scripts.

## Dependency

- boost 1.80
- g++-11, gcc-11
- selogger v0.5.0 (for logging)
- GNU Make 4.3 
- cmake version 3.22.1

## How to start?
1. Please Modyfy `set(BOOST_PATH)` in CMakeLists.txt

```bash
❯ pwd
/your/home/POLDAM
❯ cd build
# Compile POLDAM
❯ cmake -D CMAKE_C_COMPILER=/opt/homebrew/bin/gcc-11 -D CMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-11 .. && make
❯ ls
CMakeCache.txt          CMakeFiles              Makefile                cmake_install.cmake     example
❯ ./example -o "../Data/java8/src/motivationExample/origin/selogger_out" -t "../Data/java8/src/motivationExample/target/selogger_out"
```
## POLDAM Options
TBD
