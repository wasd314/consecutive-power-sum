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
yukicoder-md -t -i statement/statement.md
yukicoder-md -t -i statement/editorial.md
```



