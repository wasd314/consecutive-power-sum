# $E$ の範囲

まず $E$ の範囲を考えます．$R = 1$ と仮定すると $L = 1, S(E, L, R) = 1^E = 1$ となり，$N \ge 2$ に反します．よって $R \ge 2$ より，
$$
2^E \le R^E \le \sum_{i = L}^R i^E = S(E, L, R) = N, \\\\
\therefore E \le \log_2 N
$$
と範囲を絞ることができます．従って $2 \le E \le \lfloor \log_2 N \rfloor$ の範囲で固定した各 $E$ について解が列挙できればよいです．


# $\Theta( N^{1/E} )$ 時間解法

$(E, L, R)$ が解であるとき，$M := \lfloor N^{1/E} \rfloor$ とすると，
$$
R^E \le \sum_{i = L}^R i^E = S(E, L, R) = N
$$
より $L \le R \le M$ が成り立ちます．よって数列 $B$ を
- $B_0 \coloneqq 0$
- $B_{i} \coloneqq B_{i - 1} + i^E \quad (i = 1, 2, \dots, M)$

で定めると，問題文の第3条件は
$$
S(E, L, R) = B_R - B_{L-1} = N
$$
と言い換えられます．$M$ 以下の各 $L$ に対し，$B_R = B_{L-1} + N$ なる $R$ は $\Theta(1)$ 時間で検索できるため，合わせて $\Theta(M) = \Theta(N^{1/E})$ 時間で解くことができます．

この解法は $E \ge 3$ に対しては十分高速ですが，$E = 2$ の場合には $\Theta( N^{1/2} )$ 時間かかってしまうため，さらなる高速化が必要です．


# $E = 2$: $\Theta( N^{1/3} \log N )$ 時間解法

本節では以下 $E = 2$ の場合を扱います．総和の項数を $W \coloneqq R - L + 1$ とおくと，
$$
\begin{aligned}
N
&= \sum_{i = L}^{L + W - 1} i^2 \\\\
&\ge \sum_{i = 1}^{W} i^2 \\\\
&= \frac{1}{6} W (W + 1) (2 W + 1) \\\\
&> \frac{1}{6} W \cdot W \cdot 2 W \\\\
&= \frac{1}{3} W^3, \\\\
\therefore W &\le \lfloor (3 N)^{1/3} \rfloor
\end{aligned}
$$
より $W$ を全探索することは可能であることが分かります．
各 $W$ について，$S(2, L, L + W - 1)$ は $L$ について狭義単調増加するため，$S(2, L, L + W - 1) = N$ となる $L$ は二分探索により $\Theta(\log N)$ 時間で求められます．従って $E = 2$ の場合を $\Theta( N^{1/3} \log N )$ 時間で解くことができます．


# まとめ

以上から，
- $E = 2$ の解を $\Theta( N^{1/3} \log N )$ 時間で，
- $E \ge 3$ の解を $\Theta( N^{1/3} + N^{1/4} + \dots + N^{1/\lfloor \log_2 N \rfloor}) = \Theta(N^{1/3})$ 時間で，

それぞれ列挙できるため，合わせて $\Theta( N^{1/3} \log N )$ 時間でこの問題を解くことができます．

