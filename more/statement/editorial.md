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

で定めると，問題文の第3条件は
$$
B_R - B_{L-1} = N
$$
と言い換えられます．$M$ 以下の各 $R$ に対し，$B_{L-1} = B_{R} - N$ なる $L$ は $\Theta(1)$ 時間で検索できるため，合わせて $\Theta(M) = \Theta(N^{1/E})$ 時間で解くことができます．

この解法は $E \ge 4$ に対しては十分高速ですが，$E = 1, 2, 3$ の場合には $\Omega( N^{1/3} )$ 時間かかってしまうため，さらなる高速化が必要です．

以下，総和の項数を $W \coloneqq R - L + 1$ とおきます．$R = L + W - 1$ です．

# $E = 1$ の場合

$$
S(1, L, L + W - 1) = \frac{1}{2} (2L + W - 1) W = N, \\\\
W(W + 2L - 1) = 2N
$$
が成り立ちます．$2 L - 1$ が正の奇数であることに注意すると，次の必要条件が得られます．

- $W$ は $2N$ の正の約数
- $W \lt 2N/W$（つまり $W \lt \sqrt{2N}$ ）
- $W$ と $2N/W$ の偶奇が異なる

よって $2N$ を素因数分解し，$\sqrt{2N}$ 未満の $2N$ の正の約数を列挙し，各約数について対応する解があるか調べればよいです．$n$ の素因数分解の時間計算量を $F(n)$，$k$ 以下の $n$ の正の約数の個数を $d(n, k)$ とするとき，これは $F(2N) + \Theta( d(2N, \sqrt{2N}) )$ 時間で可能です．

素因数分解はポラードの ρ 法を用いると期待 $O(N^{1/4} \log N )$ 時間で可能です．また $d(2N, \sqrt{2N})$ は $2N$ の正の約数の個数 $d(2N)$ のおよそ半分であり，$d(2N)$ は本問題の制約下では $2N = 1\ 795\ 224\ 969\ 573\ 235\ 200$ のときの $115\ 200$ 個が最大です．


# $E = 2$ の場合
## $\Theta( N^{1/3} \log N )$ 時間解法

$$
\begin{aligned}
N
&= \sum_{i = L}^{L + W - 1} i^2 \\\\
&\ge \sum_{i = 1}^{W} i^2
= \frac{1}{6} W (W + 1) (2 W + 1)
\end{aligned}
$$
より，この不等号を満たす最大の整数 $W$ を $W_\text{max}$ として，$W$ の範囲は $1 \le W \le W_\text{max}$ に限られます．ここで
$$
\begin{aligned}
  N
  &\ge \frac{1}{6} W_\text{max} (W_\text{max} + 1) (2 W_\text{max} + 1) \\\\
  &= \frac{1}{3} W_\text{max}^3 + \frac{1}{2} W_\text{max}^2 + \frac{1}{6} W_\text{max} \\\\
  &\ge  \frac{1}{3} W_\text{max}^3, \\\\
  \therefore W_\text{max} &\le \lfloor (3 N)^{1/3} \rfloor
\end{aligned}
$$
であり，$W_\text{max} \in \Theta( N^{1/3} )$ が示せます（より精密には，$W_\text{max} = \lfloor (3 N)^{1/3} \rfloor$ もしくは $W_\text{max} = \lfloor (3 N)^{1/3} \rfloor - 1$ が成り立ちます）．$W$ を固定すると，$S(2, L, L + W - 1)$ は $L$ について狭義単調増加するため，$S(2, L, L + W - 1) = N$ なる $L$ は二分探索により $\Theta(\log N)$ 時間で求められます．従って $E = 2$ の場合を $\Theta( N^{1/3} \log N )$ 時間で解くことができます．

## $W$ に関する枝刈り

$S(2, L, L+W-1)$ は
$$
S(2, L, L+W-1)
= \frac{1}{6} W (6L^2 + 6 LW - 6L + 2W^2 - 3W + 1)
$$
と因数分解することができるので，$S(2, L, L+W-1) = N$ のとき
$$
W (6L^2 + 6 LW - 6L + 2W^2 - 3W + 1) = 6N
$$
より $W$ は $6N$ の約数であることが分かります．

<details>
<summary>気付き方</summary>

一般に $S(E, 1, R)$ は $R$ についての有理数係数多項式なので（ファウルハーバーの公式），
$$
S(E, L, L+W-1) = S(E, 1, L+W-1) - S(E, 1, L-1)
$$
  は $W$ と $L$ についての有理数係数多項式です．この多項式は $W = 0$ を代入すると
$$
S(E, 1, L+0-1) - S(E, 1, L-1) = 0
$$
と $0$ になります．よって因数定理より，$S(E, L, L+W-1)$ は $W$ で有理数係数多項式として割り切れます．更に係数の分母を払うために $D \cdot S(E, L, L+W-1)$ が整数係数多項式となるような正整数 $D$ を取ると（例えば $E = 2$ では上述のように $D = 6$ とすれば），$W$ は $D \cdot S(E, L, L+W-1) = D \cdot N$ の約数といえます．$\blacksquare$
</details>

よって $W_\text{max}$ 以下の $6N$ の正の約数だけ調べればよく，$\Theta( d(6N, W_\text{max}) \log N )$ 時間が達成できます．

本問題の制約下における $d(6N, W_\text{max})$ の最大値は確認できていませんが，高々 $13\ 002$ 個との上界が得られています．よって $W_\text{max}$ の最大値 $1\ 442\ 249$ と比べると $100$ 倍程度の改善が見込めます．

# $E = 3$ の場合

$E = 2$ の場合の手法は $E = 3$ の場合にも適用できます．

$$
\begin{aligned}
N
&= \sum_{i = L}^{L + W - 1} i^3 \\\\
&\ge \sum_{i = 1}^{W} i^3
= \frac{1}{4} W^2 (W + 1)^2
\end{aligned}
$$
より，この不等号を満たす最大の整数 $W$ を $W_\text{max}'$ として，$W$ の範囲は $1 \le W \le W_\text{max}'$ に限られます．ここで
$$
\begin{aligned}
  N
  &\ge \frac{1}{4} W_\text{max}'{}^2 (W_\text{max}' + 1)^2 \\\\
  &> \frac{1}{4} W_\text{max}'{}^4, \\\\
  \therefore W_\text{max}' &\le \lfloor (4 N)^{1/4} \rfloor
\end{aligned}
$$
であり，$W_\text{max}' \in \Theta( N^{1/4} )$ が示せるので，$\Theta( N^{1/4} \log N )$ 時間で解くことができます．また
$$
\begin{aligned}
  2 \cdot S(3, L, L+W-1)
  &\equiv 2 \cdot S(3, 0, W-1) \\\\
  &\equiv \frac{1}{2} W^2 (W+1)^2 \\\\
  &\equiv W \cdot \frac{1}{2} W (W+1) \cdot (W+1) \\\\
  &\equiv 0 && \pmod{W}  \\\\
\end{aligned}
$$
より $W$ は $2N$ の約数であることが分かるので，$W_\text{max}'$ 以下の $2N$ の正の約数だけ調べることで $\Theta( d(2N, W_\text{max}') \log N )$ 時間でも解くことができます．

# まとめ

以上から，
- $E = 1$ の解を $F(2N) + \Theta( d(2N, \sqrt{2N}) )$ 時間で，
- $E = 2$ の解を $\Theta( d(6N, W_\text{max}) \log N )$ 時間で，
- $E = 3$ の解を $\Theta( d(2N, W_\text{max}') \log N )$ 時間で，
- $E \ge 4$ の解を $\Theta( N^{1/4} + N^{1/5} + \dots + N^{1/\lfloor \log_2 N \rfloor}) = \Theta(N^{1/4})$ 時間で，

それぞれ列挙でき，$d(n, k) \le d(n, n) \in o(n^{1/4})$ であるため，合わせて期待 $O( N^{1/4} \log N )$ 時間程度でこの問題を解くことができます．

