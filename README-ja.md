# Problem: Consecutive Power Sum

このリポジトリでは，次の競技プログラミングの問題を管理しています．
- https://yukicoder.me/problems/11795
- https://yukicoder.me/problems/12100
- https://yukicoder.me/problems/12101

[Rime](https://github.com/icpc-jag/rime) および [yukicoder-md](https://github.com/koyumeishi/yukicoder-md) を利用しています．

## 自動化

このリポジトリで自動化されている各種手順の実行方法について述べます．パスは`PROBLEM`ファイルがあるディレクトリからの相対パスで示します．

### 想定解の自動生成

想定解法ごとに，Rimeが認識する解答プログラムのディレクトリを自動生成します．
```
python solution_generator/gen_py.py (python|pypy)
```

### 想定解のテスト

```
rime test .
```

### 問題文・解説文の生成

```
yukicoder-md -t -i statement/statement.md | sed -e 's/<h5>/<h5 class=\"shadow\">/g' | pbcopy
yukicoder-md -t -i statement/editorial.md | sed -e 's/<h5>/<h5 class=\"shadow\">/g' | pbcopy
```

ここで `sed -e 's/<h5>/<h5 class=\"shadow\">/g'` は Markdown ファイルにおけるレベル2見出し `##` の装飾をしています．

## テストケース名の形式

ファイル名は `f"{a}{b}-{method}-{serial:02}.{extension}"`の形式とする．ここで
- `a`: $N$ の範囲
  - `1`: $N \in [1, 10^{18}]$
  - `2`: $N \in (10^{18}, 10^{22}]$
  - `3`: $N \in (10^{22}, 10^{23}]$
  - `4`: $N \in (10^{23}, 10^{24}]$
- `b`, `method`: 生成方法
  - `b == 0`, `method == "handmade"`: 手製
  - `b == 1`, `method == "random"`: 解を乱択して $N$ を逆算（解の存在が保証される）
  - `b == 2`, `method == "random"`: $N$ を乱択（解の存在が保証されない）
- `serial`: `a`, `b` とも同じカテゴリ内の通し番号
  - $(\text{解の個数}, N)$ の昇順
  - $0$-origin
- `extension`: 拡張子
  - `"in"`: 入力
  - `"diff"`: 想定出力
  - `"txt"`: yukicoder に登録するテストケース（入力，出力）

である．例外としてサンプルは
- `a == 0`
- `b == 0`
- `method == "sample"`
- `serial` は $N$ 自体

すなわち `f"00-sample-{N}.{extension}"` とする．

`extension` のうち `"in"`, `"diff"` は Rime からの要請および仕様に基づき，`"txt"` は yukicoder の要請（入出力の拡張子を揃える）によるものである．
