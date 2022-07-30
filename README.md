## Parser

基本的には二つに分ける

ログのインタプリタ
→ 　イベントごとにパースする内容を追加する

ログの塊を一つずつに分割し

ログをパースする。一行ずつストリームを読む。
イベントごとに何をするか決める

イベントタイプとその他の情報を返

## Graph

比較
グラフ構築

##　頂点情報
頂点情報でグラフになにを追加するかを考える

## コーディング規約

命名規則

### Class, Struct

PascalClass

### method, function, variable

camelCase

### filename, std

snake_case

### Enum, macro

SNAKE_CASE

# Limitation

- Multiple Object type parser is not supported
- The Interprete for the line which has 　 Object Type is not supported
- Method factory pattern in under development.
