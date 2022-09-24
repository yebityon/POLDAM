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
  - seloggerやJavaのMotivation Exampleが含まれるディレクトリです。docker-composeではこのディレクトリがjavaコンテナにマウントされます
- dockerfiles
  - java8のDcockerfileが含まれています。
- Graph
  - `MetaFileParser`によってパースされた情報からマークル木を構築し、それらを比較するアルゴリズムを提供します
- Interpreter 
  - `selogger`のイベントタイプなどの定義が含まれていいます。
- MetaFileParser
  - seloggerによって出力されたログディレクトリを入力として、そのディレクトリ以下のファイルを解釈し、必要な情報を出力するパーサーを提供します。
- Util
  - このスクリプト内で用いている汎用的なクラスや関数を提供します
## Dependency 
- boost 1.80
- g++-11 
- selogger v0.5.0 ( for logging)
- GNU Make 4.3 (Optional)

## How to Use it 

Motivation Exampleを実行するための最低限のステップをいかに示します。

1. Dependency　を実行したい環境に用意してください
2. このレポジトリ　を git clone してください
3. `POLDAM/Makefile`の中の`BOOST_PATH`の部分を適宜変更してください
4. `make main`を実行して`main.cpp`をコンパイルしてください
5. `./POLDAM  -o ./Data/java8/src/motivationExample/origin/selogger_out/ -t ./Data/java8/src/motivationExample/target/selogger_out/ -m notsupported -out sample --flow` を実行してください
6. `sample_diff.out`が出力されると成功です。

# Limitation
- `-m` option is not supported. it is useful when you try to parse log that contain test framework (like JUnit). It will be supported soon.
- This script have been fully rewritten, hence, it's slightly different from the one I used in thesis.
- Multithread is not supported.
- Only `-omni` option is supported.
- Object write instruction tracking is not supported.
- Defects4j motivation will be added soon.
