# 参考

[Consecutive Cubic Sum](https://yukicoder.me/problems/11795),
[Consecutive Power Sum (Small)](https://yukicoder.me/problems/12100),
[Consecutive Power Sum (Large)](https://yukicoder.me/problems/12101) はこの順におおよそ上位互換の関係にあります．


# 問題文

2つの正整数 $L, R$ に対し，$S(L, R)$ を
$$
S(L, R) \coloneqq \sum_{i = L}^R i^3
$$
で定めます．

正整数 $N$ が与えられます．次の2条件を全て満たす正整数の2つ組 $(L, R)$ を全て求めてください．
- $1 \le L \le R$
- $S(L, R) = N$


# 制約

入力される $N$ は $1$ 以上 $10^{18}$ 以下の整数です．


# 入力

入力は次の形式で標準入力から与えられます．
```
$N$
```


# 出力

問題文の条件を満たす2つ組を解と呼ぶことにします．本問題の制約下で解は有限個であることが証明できます．解は全部で $C$ 個あり，それらが辞書式順序で小さい方から順に
$$
(L_1, R_1), (L_2, R_2), \dots, (L_C, R_C)
$$
であるとき，次の形式で $C + 1$ 行出力してください．各行は空白区切りとし，最後に改行も出力してください．
```
$C$
$L_1\ R_1$
$L_2\ R_2$
$\vdots$
$L_C\ R_C$
```


# サンプル

{{ import_sample_input(file="../rime-out/tests/00_sample_2025.in") }}
{{ import_sample_output(file="../rime-out/tests/00_sample_2025.diff") }}

次の式が成り立ちます．
- $S(1, 9) = 1^3 + 2^3 + 3^3 + 4^3 + 5^3 + 6^3 + 7^3 + 8^3 + 9^3 = 2025$

$(1, 9)$ は唯一の解です．


{{ import_sample_input(file="../rime-out/tests/00_sample_2026.in") }}
{{ import_sample_output(file="../rime-out/tests/00_sample_2026.diff") }}

解が1つもないこともあります．


{{ import_sample_input(file="../rime-out/tests/00_sample_216.in") }}
{{ import_sample_output(file="../rime-out/tests/00_sample_216.diff") }}

解が複数あることもあります．$L = R$ でもよいことに注意してください．

次の式が成り立ちます．
- $S(3, 5) = 3^3 + 4^3 + 5^3 = 216$
- $S(6, 6) = 6^3 = 216$

$(3, 5)$ の方が $(6, 6)$ より辞書式順序で小さいので，この順に出力してください．
