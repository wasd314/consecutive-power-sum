# 問題文

3つの正整数 $E, L, R$ に対し，$S(E, L, R)$ を
$$
S(E, L, R) \coloneqq \sum_{i = L}^R i^E
$$
で定めます．

正整数 $N$ が与えられます．次の3条件を全て満たす正整数の3つ組 $(E, L, R)$ を全て求めてください．
- <font color="red">$2$</font> $\le E$
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

問題文の条件を満たす3つ組を解と呼ぶことにします．本問題の制約下で解は有限個であることが証明できます．解が全部で $C$ 個あり，それらが辞書順で小さい方から順に
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

{{ import_sample_input(file="../rime-out/tests/000-sample-2024.in") }}
{{ import_sample_output(file="../rime-out/tests/000-sample-2024.diff") }}

次の式が成り立ちます．
- $S(3, 2, 9) = 2^3 + 3^3 + 4^3 + 5^3 + 6^3 + 7^3 + 8^3 + 9^3 = 2024$

$(3, 2, 9)$ は唯一の解です．


{{ import_sample_input(file="../rime-out/tests/000-sample-2025.in") }}
{{ import_sample_output(file="../rime-out/tests/000-sample-2025.diff") }}

解が複数あることもあります．$L = R$ でもよいことに注意してください．

次の式が成り立ちます．
- $S(2, 45, 45) = 45^2 = 2025$
- $S(3, 1, 9) = 1^3 + 2^3 + 3^3 + 4^3 + 5^3 + 6^3 + 7^3 + 8^3 + 9^3 = 2025$

$(2, 45, 45)$ の方が $(3, 1, 9)$ より辞書順で小さいので，この順に出力してください．


{{ import_sample_input(file="../rime-out/tests/000-sample-2026.in") }}
{{ import_sample_output(file="../rime-out/tests/000-sample-2026.diff") }}

解が1つもないこともあります．
