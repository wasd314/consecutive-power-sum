
# 問題文

3つの正整数 $E, L, R$ に対し，$S(E, L, R)$ を
$$
S(E, L, R) \coloneqq \sum_{i = L}^R i^E
$$
で定めます．

正整数 $N$ が与えられます．次の3条件を全て満たす正整数の3つ組 $(E, L, R)$ を全て求めてください．
- <font color="red">$3$</font> $\le E$
- $1 \le L \le R$
- $S(E, L, R) = N$


# 制約

入力される $N$ は <font color="red">$2$</font> 以上 $10^{18}$ 以下の整数です．


# 入力

入力は次の形式で標準入力から与えられます．
```
$N$
```


# 出力

問題文の条件を満たす3つ組が全部で $C$ 組あり，辞書順で小さい方から順に
$$
(E_1, L_1, R_1), (E_2, L_2, R_2), \dots, (E_C, L_C, R_C)
$$
であるとき，次の形式で $C + 1$ 行出力してください．各行は空白区切りとし，最後に改行も出力してください．
```
$C$
$E_1\ L_1\ R_1$
$E_2\ L_2\ R_2$
$\vdots$
$E_C\ L_C\ R_C$
```


# サンプル

{{ import_sample_input(file="../rime-out/tests/00-sample-00.in") }}
{{ import_sample_output(file="../rime-out/tests/00-sample-00.diff") }}

次が成り立ちます．
- $S(3, 1, 9) = 1^3 + 2^3 + 3^3 + 4^3 + 5^3 + 6^3 + 7^3 + 8^3 + 9^3 = 2025$


{{ import_sample_input(file="../rime-out/tests/00-sample-01.in") }}
{{ import_sample_output(file="../rime-out/tests/00-sample-01.diff") }}

解が1つもないこともあります．


{{ import_sample_input(file="../rime-out/tests/00-sample-02.in") }}
{{ import_sample_output(file="../rime-out/tests/00-sample-02.diff") }}

解が複数あることもあります．$L = R$ でもよいことに注意してください．

以下が共に成り立ちます．
- $S(3, 1, 8) = 1^3 + 2^3 + 3^3 + 4^3 + 5^3 + 6^3 + 7^3 + 8^3 = 1296$
- $S(4, 6, 6) = 6^4 = 1296$

$(3, 1, 8)$ より $(4, 6, 6)$ の方が辞書順で小さいので，この順に出力してください．

