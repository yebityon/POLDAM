# POLDAM

## abstract

This script is a concrete implementation of the script used in [実行トレースのマークル木を用いたプログラム変更前後の差分検出法の提案](https://library.naist.jp/dspace/handle/10061/14739).

Based on the execution trace obtained from [selogger](https://github.com/takashi-ishio/selogger), a Merkle tree is constructed, which is used to output the differences between before and after changes to the program as a `dot` file.

It contains sample implementations of method calls and retrieval of various information about objects from a set of files in a directory output by `selogger`.

The `dev` branch is the development branch. The `main` branch is the stable release.

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

### Description
- Data
  - This directory contains the selogger and Java Motivation Example. docker-compose mounts this directory in the Java container.
- dockerfiles
  - Contains the Dockerfile for Java 8.
- poldam/graph
  - Provides an algorithm to build and compare a Merkle tree from information parsed by MetaFileParser.
- poldam/helper
  - Provides a helper program to build a graph, i.e., an example program that combines selogger_log_parser and graph.
- poldam/script
  - CMake provided in this directory automatically compiles cpp files and links them with poldam/helper, poldam/graph, and poldam/util. To create your own script using the above libraries, place the program in this directory.
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

## Tutorial
Modify `set(BOOST_PATH)` in CMakeLists.txt to your path.

```bash
❯ pwd
/your/home/POLDAM
❯ cd build
❯ cmake -D CMAKE_C_COMPILER=/opt/homebrew/bin/gcc-11 -D CMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-11 .. && make
❯ ls
CMakeCache.txt          CMakeFiles              Makefile                cmake_install.cmake     test
❯ ./test
```

## Limitation

- The -c option specifies the class of the method; it has no effect when not used with the -m option.
- When the -m option is used, it must be used in conjunction with -c, otherwise core dumping may occur or the system may not function properly.
- When specifying a class with the `-c` option, use `package name` + `class name`. In this case, `.` in the package name should be replaced by `/`.
- This script have been fully rewritten, hence, it's slightly different from the one I used in thesis.
- Multithread is not supported.
- Only `-omni` option is supported.
- Object write instruction tracking is not supported.
