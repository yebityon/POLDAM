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
├── main.cpp
└── poldam_sample.sh # sample script
```

### Description

- Data
  - The directory containing the selogger and Java Motivation Example. docker-compose mounts this directory in the java container
- dockerfiles
  - Contains the java8 Dcockerfile.
- poldam/graph
  - Provides an algorithm to build a Merkle tree from information parsed by `MetaFileParser` and compare them.
- poldam/helper
  - Set up basic information about the graph.
- poldam/script
  - This file is used to describe the execution details.
- poldam/selogger_log_parser
  - Contains definitions for `selogger` event types, etc.
  - Provides a parser that takes the log directory output by `selogger` as input, interprets files under that directory, and outputs necessary information.
- poldam/util
  - Provides generic classes and functions used in this script.

## Dependency

- boost 1.80
- g++-11
- selogger v0.5.0 ( for logging)
- GNU Make 4.3 (Optional)
- cmake version 3.22.1

## How to Use it

The minimum steps to run the Motivation Example are as follows.

1. Prepare the environment in which you want to run Dependency.
2. Change `set(BOOST_PATH Boost/boost_1_80_0)` in CMakeLists.txt according to your environment.
3. (For sample program users) Execute `Data/java8m/motivationExample/build.sh` in the java container.

```bash
# Scripts to build and execute sample programs
$ sh poldam_sample.sh
```

## Limitation

- The -c option specifies the class of the method; it has no effect when not used with the -m option.
- When the -m option is used, it must be used in conjunction with -c, otherwise core dumping may occur or the system may not function properly.
- When specifying a class with the `-c` option, use `package name` + `class name`. In this case, `.` in the package name should be replaced by `/`.
- This script have been fully rewritten, hence, it's slightly different from the one I used in thesis.
- Multithread is not supported.
- Only `-omni` option is supported.
- Object write instruction tracking is not supported.

## Coding Style

| Naming Rule Name |             Usage                |
| ---------------- | -------------------------------- |
| Pascal Case      | `class`                          |
| camelCase        | `function`, `method`, `variable` |
| snake_case       | `file_name.cpp`                  |
| SNAKE_CASE       | `#define`, `enum`, `namespace`   |

## Formatter

[Reference](https://qiita.com/shirakawa4756/items/55b509fb56cb1bb0c9a4)
