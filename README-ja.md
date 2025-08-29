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
```sh
python ./solution_generator/gen_py.py (python|pypy)
```

### 想定解のテスト

```sh
rime test .
```

### 問題文・解説文の生成

```sh
cat ./statement/statement.md | sed -E -e 's/\\([,;{}])/\\\\\1/g' | yukicoder-md -t | sed -e 's/<h5>/<h5 class="shadow">/g' | pbcopy
cat ./statement/editorial.md | sed -E -e 's/\\([,;{}])/\\\\\1/g' | yukicoder-md -t | sed -e 's/<h5>/<h5 class="shadow">/g' | pbcopy
```

ここで
- `sed -E -e 's/\\([,;{}])/\\\\\1/g'` は数式中の `\,` などに含まれる `\` を重ねてエスケープします
- `sed -e 's/<h5>/<h5 class=\"shadow\">/g'` は Markdown ファイルにおけるレベル2見出し `##` の装飾をします

### yukicoder 向けテストケースファイルの生成

テストケースファイルの拡張子について，Rime は入力ファイルは `.in`，出力ファイルは `.diff` としていますが，yukicoder では入力ファイル名と出力ファイル名は拡張子まで含めて同一である必要があります．

そこで拡張子を `.in` や `.diff` から `.txt` に変更するシェルスクリプトを用意しています．

```sh
./testcase_generator/change_ext.sh
```

`rime test .` を実行すると `./rime-out/tests/` 以下に入出力ファイルが生成されるので，これらを `./testcase_generator/{in,diff}/` 以下に拡張子を変えてコピーします．


## テストケース名の形式

ファイル名は `f"{a}{b}_{method}_{serial:02}.{extension}"`の形式としています．ここで
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

です．

例外としてサンプルは
- `a == 0`
- `b == 0`
- `method == "sample"`
- `serial` は $N$ 自体

すなわち `f"00_sample_{N}.{extension}"` としています．

`extension` のうち `"in"`, `"diff"` は Rime の仕様に基づき，`"txt"` は yukicoder の仕様によるものです．

More の制約を迷っているため，

- `a <= 1` のケースだけ見る: $N \le 10^{18}$
- `a <= 2` のケースだけ見る: $N \le 10^{22}$
- `a <= 3` のケースだけ見る: $N \le 10^{23}$
- `a <= 4` のケースだけ見る: $N \le 10^{24}$

ということがやりやすいように，ファイル名はまず `a` の昇順に並ぶようにしています．
