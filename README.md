# POLDAM

## 概要

このプログラムは、[実行トレースのマークル木を用いたプログラム変更前後の差分検出法の提案](https://library.naist.jp/dspace/handle/10061/14739) の論文内で使用したアルゴリズムの具体的な実装です。
[selogger](https://github.com/takashi-ishio/selogger)から取得した実行トレースを入力として、マークル木を構築し、`dot`ファイルとして出力することができます。
このレポジトリには、Seloggerのログをparseするプログラム`poldam/selogger_log_parser`, マークル木を構築するプログラム`poldam/graph`, pログとグラフのライブラリを用いて、論文内の実装に用いたプログラム`poldam/helper`が含まれています。

## tree

```bash 
.
├── CMakeFiles
├── CMakeLists.txt
├── Data
│   └── java8
├── Doxyfile
├── Doxyfile.bak
├── Examples
│   ├── sample_diff
│   └── sample_only_origin
├── Makefile
├── README.md
├── build
├── docker-compose.yml
├── dockerfiles
│   ├── c++
│   └── java8
├── docs
├── main.cpp
├── poldam //本体
│   ├── build // 生成されたプログラムはここに
│   ├── graph
│   ├── helper
│   ├── script
│   ├── selogger_log_parser
│   └── util
└── poldam_sample.sh
```

## Dependency

- boost 1.80
- g++-11, gcc-11
- selogger v0.5.0 ( for logging)
- GNU Make 4.3 
- cmake version 3.22.1

## Tutorial
1. CMakeLists.txt の`set(BOOST_PATH )` の部分をboostのパスに変更してください
以下のディレクトリとパスで実行します
```bash
❯ pwd
/your/home/POLDAM
❯ cd build
❯ cmake -D CMAKE_C_COMPILER=/opt/homebrew/bin/gcc-11 -D CMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-11 .. && make
❯ ls
CMakeCache.txt          CMakeFiles              Makefile                cmake_install.cmake     test
❯ ./test
```
