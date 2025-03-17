
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

問題文の条件を満たす2つ組が全部で $C$ 組あり，辞書順で小さい方から順に
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

{{ import_sample_input(file="../rime-out/tests/00-sample-2025.in") }}
{{ import_sample_output(file="../rime-out/tests/00-sample-2025.diff") }}

次が成り立ちます．
- $S(1, 9) = 1^3 + 2^3 + 3^3 + 4^3 + 5^3 + 6^3 + 7^3 + 8^3 + 9^3 = 2025$


{{ import_sample_input(file="../rime-out/tests/00-sample-2026.in") }}
{{ import_sample_output(file="../rime-out/tests/00-sample-2026.diff") }}

解が1つもないこともあります．


{{ import_sample_input(file="../rime-out/tests/00-sample-216.in") }}
{{ import_sample_output(file="../rime-out/tests/00-sample-216.diff") }}

解が複数あることもあります．$L = R$ でもよいことに注意してください．

以下が共に成り立ちます．
- $S(3, 5) = 3^3 + 4^3 + 5^3 = 216$
- $S(6, 6) = 6^3 = 216$

$(6, 6)$ より $(3, 5)$ の方が辞書順で小さいので，この順に出力してください．

