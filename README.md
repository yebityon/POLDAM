# POLDAM

## 概要

このプログラムは、[実行トレースのマークル木を用いたプログラム変更前後の差分検出法の提案](https://library.naist.jp/dspace/handle/10061/14739) の論文内で使用したアルゴリズムの具体的な実装です。
[selogger](https://github.com/takashi-ishio/selogger)から取得した実行トレースを入力として、マークル木を構築し、`dot`ファイルとして出力することができます。

## レポジトリの構成
具体的な実装は`poldam/`以下に含まれます。
seloggerのログをparseするプログラム`poldam/selogger_log_parser`, 実際にマークル木を表現するためのAPIの`poldam/graph`,論文の実装で用いたヘルパープログラムである`poldam/helper`が含まれています。
`poldam/script`ディレクトリは、これらのライブラリを用いたスクリプトを作成するためのディレクトリです。このディレクトリ以下で作成したプログラムは、CMakeを用いたビルド時にライブラリをリンクし、実行ファイルを生成することができます。
`test.cpp`は、`poldam/selogger_log_parser`と``poldam/helpler`,`poldam/graph`を用いた論文内の実装のサンプルです。

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
  - seloggerとMotivation Exampleなどが含まれます。
- dockerfiles
  - selogger実行のためのDockerfileが含まれます
- poldam/graph
  - POLDAM内のマークル木の定義などが含まれます。
- poldam/helper
  - selogger_log_parserとgraphを用いてマークル木を構築するためのプログラムが含まれます。
- poldam/script
  - このディレクトリは、POLDAMのライブラリを用いた自作のスクリプトを作成する際に用います。このディレクトリ以下に配置したプログラムは、CMakeを用いてビルドしたとき、POLDAMの他のプログラム(poldam/helper, poldam/graph,poldam/util)とリンクされて実行ファイルが生成されます。
- poldam/selogger_log_parser
  - SELoggerのログのパーサーです。
  - POLDAMで用いる上での定義などが記述されています。
- poldam/util
  - utility向けのライブラリです。

## Dependency

- boost 1.80
- g++-11, gcc-11
- selogger v0.5.0 (for logging)
- GNU Make 4.3 
- cmake version 3.22.1

## How to start
`script/example.cpp`は論文内で用いた実装の再現です。
オプションのヘルプは次のようになっています
```bash
❯ pwd
/your/home/POLDAM
❯ cd build
# export boost path
❯ export POLDAM_BOOST_PATH=/YOUR/BOOST/PATH/boost_1_80_0 
# Compile POLDAM
❯ cmake -D CMAKE_C_COMPILER=/opt/homebrew/bin/gcc-11 -D CMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-11 .. && make
❯ ls
CMakeCache.txt          CMakeFiles              Makefile                cmake_install.cmake     example
❯ ./example -o "../Data/java8/src/motivationExample/origin/selogger_out" -t "../Data/java8/src/motivationExample/target/selogger_out"
```
## POLDAM Options
TBD
