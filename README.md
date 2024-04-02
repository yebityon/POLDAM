# POLDAM

## 概要

このプログラムは、[実行トレースのマークル木を用いたプログラム変更前後の差分検出法の提案](https://ipsj.ixsq.nii.ac.jp/ej/index.php?active_action=repository_view_main_item_detail&page_id=13&block_id=8&item_id=217311&item_no=1)の論文と[Comparing Execution Trace Using Merkle-Tree to Detect Backward Incompatibilities](https://conf.researchr.org/details/saner-2024/saner-2024-early-research-achievement--era--track-/98/Comparing-Execution-Trace-Using-Merkle-Tree-to-Detect-Backward-Incompatibilities)[^1].の III.PROPOSED METHODの各ステップである 2) Merkle-Tree Constructionと 3) Calculate Hash Values で使用したアルゴリズムの具体的な実装です。

[^1]: SANER2024 Early Research Achievement (ERA) Track

最新の論文は[このフォルダー](./latest_papers/)から入手できます。
補足資料は[こちら](./latest_papers/SupplementaryMaterials.md)から確認できます。

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

## seloggerのセットアップ
`docker-compose.yml`を用いることで、seloggerとPOLDAMの実行環境を一度に用意できます。
`/Data/java8`以下のディレクトリで`selogger`をcloneしてください。

## POLDAM Options

TBD
