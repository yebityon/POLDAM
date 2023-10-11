# POLDAM

## 概要

このスクリプトは、[実行トレースのマークル木を用いたプログラム変更前後の差分検出法の提案](https://library.naist.jp/dspace/handle/10061/14739) で使用したスクリプトの具体的な実装です。

[selogger](https://github.com/takashi-ishio/selogger)から取得した実行トレースをもとに、マークル木を構築し、これを用いてプログラムの変更前後の差分を`dot`ファイルとして出力します。

`selogger`が出力したディレクトリのファイル群から、メソッド呼び出し、オブジェクトの様々な情報を取得するサンプル実装が含まれます。

`dev`は開発用のブランチです。`main`ブランチが安定板のリリースとなります。

## tree

```
.
├── Data
│   └── java8
├── dockerfiles
│   └── java8
├── Graph
│   └── src
├── Interpreter
│   └── src
├── MetaFileParser
│   ├── src
│   └── test
└── Util
    └── include
```

### Description

- Data
  - selogger や Java の Motivation Example が含まれるディレクトリです。docker-compose ではこのディレクトリが java コンテナにマウントされます
- dockerfiles
  - java8 の Dcockerfile が含まれています。
- Graph
  - `MetaFileParser`によってパースされた情報からマークル木を構築し、それらを比較するアルゴリズムを提供します
- Interpreter
  - `selogger`のイベントタイプなどの定義が含まれていいます。
- MetaFileParser
  - selogger によって出力されたログディレクトリを入力として、そのディレクトリ以下のファイルを解釈し、必要な情報を出力するパーサーを提供します。
- Util
  - このスクリプト内で用いている汎用的なクラスや関数を提供します

## Dependency

- boost 1.80
- g++-11
- selogger v0.5.0 ( for logging)
- GNU Make 4.3 (Optional)
- cmake version 3.22.1
## How to Use it

Motivation Example を実行するための最低限のステップを以下に示します。

1. Dependency を実行したい環境に用意してください
2. CMakeLists.txt　内の`set(BOOST_PATH Boost/boost_1_80_0)` を適宜環境に併せて変更してください
  
```bash
pwd # => /path/to/POLDAM
#build ディレクトリの作成
mkdir build
# 作成したbuildディレクトリに移動
cd build 
# cmakeの実行
cmake .. && make 
# 以下のようなファイルが生成されていると成功
ls 
CMakeCache.txt  CMakeFiles  cmake_install.cmake  libmetafileParser.a  Makefile  POLDAM
```

# Limitation
- The -c option specifies the class of the method; it has no effect when not used with the -m option.
- When the -m option is used, it must be used in conjunction with -c, otherwise core dumping may occur or the system may not function properly.
- When specifying a class with the `-c` option, use `package name` + `class name`. In this case, `.` in the package name should be replaced by `/`.
- This script have been fully rewritten, hence, it's slightly different from the one I used in thesis.
- Multithread is not supported.
- Only `-omni` option is supported.
- Object write instruction tracking is not supported.
- Defects4j motivation will be added soon.

# Coding Style

| 命名規則名  | 用途                             |
| ----------- | -------------------------------- |
| Pascal Case | `class`                          |
| camelCase   | `function`, `method`, `variable` |
| snake_case  | `file_name.cpp`                  |
| SNAKE_CASE  | `#define`, `enum`, `namespace`   |

# Roadmap
- Unit testの追加
- selogger最新版への追従

## Formatter

[Reference](https://qiita.com/shirakawa4756/items/55b509fb56cb1bb0c9a4)
