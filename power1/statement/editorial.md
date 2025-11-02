# $E$ の範囲

まず $E$ の上界を与えます．$R = 1$ と仮定すると，$L = 1, S(E, L, R) = 1^E = 1$ が従うので $N \ge 2$ に反します．よって $R \ge 2$ より，
$$
  2^E \le R^E \le \sum_{i = L}^R i^E = N, \\\\
  \therefore E \le \log_2 N
$$
が成り立ちます．従って以下 $E = 1, 2, 3, \dots, \lfloor \log_2 N \rfloor$ の各場合について解を列挙することを考えます．


# $\Theta( N^{1/E} )$ 時間解法

$(E, L, R)$ が解であるとき，
$$
  R^E \le \sum_{i = L}^R i^E = S(E, L, R) = N, \\\\
  \therefore R \le N^{1/E}
$$
より $M := \lfloor N^{1/E} \rfloor$ とすると $1 \le L \le R \le M$ が成り立ちます．よって数列 $B$ を
- $B_0 \coloneqq 0$
- $B_{i} \coloneqq B_{i - 1} + i^E \quad (i = 1, 2, \dots, M)$

で定めると，問題文の第2条件は
$$
B_R - B_{L-1} = N
$$
と言い換えられます．$M$ 以下の各 $R$ に対し，$B_{L-1} = B_{R} - N$ なる $L$ は $\Theta(1)$ 時間や $\Theta(\log M)$ 時間で検索できるため，合わせて $\Theta(M) = \Theta(N^{1/E})$ 時間や $\Theta(M \log M) = \Theta(N^{1/E} \log N)$ 時間で解くことができます．

もしくは尺取り法を用いることで，$M$ 以下の各 $R$ に対して $B_{L-1} = B_{R} - N$ なる $L$ の列挙を合計 $\Theta(M)$ 時間で行っても良いでしょう．

---

これらの解法は $E \ge 2$ に対しては $O( N^{1/2} \log N )$ 時間と十分高速ですが，$E = 1$ の場合には $\Omega( N )$ 時間かかってしまうため，さらなる高速化が必要です．


# $E = 1$ の場合

総和の項数を $W \coloneqq R - L + 1$ とおきます．$R = L + W - 1$ です．

$$
  S(1, L, L + W - 1) = \frac{1}{2} (2L + W - 1) W = N, \\\\
  W(W + 2L - 1) = 2N
$$
が成り立ちます．$2 L - 1$ が正の奇数であることに注意すると，次の必要条件が得られます．

- $W$ は $2N$ の正の約数
- $W \lt 2N/W$（つまり $W \lt \sqrt{2N}$ ）
- $W$ と $2N/W$ の偶奇が異なる

よって $\sqrt{2N}$ 未満の $2N$ の正の約数を列挙し，各約数を $W$ としたときに対応する解があるか調べればよいです．これは $\Theta( \sqrt{2N} ) = \Theta( N^{1/2})$ 時間で可能です．


# まとめ

以上から，
- $E = 1$ の解を $\Theta( N^{1/2} )$ 時間で，
- $E \ge 2$ の解を $\Theta( N^{1/2} + N^{1/3} + \dots + N^{1/\lfloor \log_2 N \rfloor}) = \Theta(N^{1/2})$ 時間もしくは $\Theta( N^{1/2} \log N + N^{1/3} \log N + \dots + N^{1/\lfloor \log_2 N \rfloor} \log N) = \Theta(N^{1/2} \log N)$ 時間で，

それぞれ列挙できるため，全体でも $\Theta( N^{1/2} )$ 時間もしくは $\Theta( N^{1/2} \log N )$ 時間でこの問題を解くことができます．


# 実装例

- $\Theta( N^{1/2} )$ 時間（試し割り法，尺取り法）：[PyPy3 (151 ms)](https://yukicoder.me/submissions/1128670)，[C++23 (28 ms)](https://yukicoder.me/submissions/1129607)
- $\Theta( N^{1/2} )$ 時間（試し割り法，連想配列）：[PyPy3 (442 ms)](https://yukicoder.me/submissions/1128681)
- $\Theta( N^{1/2} \log N )$ 時間（試し割り法，二分探索）：[PyPy3 (408 ms)](https://yukicoder.me/submissions/1128694)
