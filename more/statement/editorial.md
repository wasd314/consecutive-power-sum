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
と言い換えられます．$M$ 以下の各 $R$ に対し，$B_{L-1} = B_{R} - N$ なる $L$ は $\Theta(1)$ 時間で検索できるため，合わせて $\Theta(M) = \Theta(N^{1/E})$ 時間で解くことができます．

この解法は $E \ge 4$ に対しては $O( N^{1/4} )$ 時間と十分高速ですが，$E = 1, 2, 3$ の場合には $\Omega( N^{1/3} )$ 時間かかってしまうため，さらなる高速化が必要です．$E = 1, 2, 3$ の場合も $O( N^{1/4} )$ 時間で解くことを目指します．

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

よって $2N$ を素因数分解し，$\sqrt{2N}$ 未満の $2N$ の正の約数を列挙し，各約数を $W$ としたときに対応する解があるか調べればよいです．$n$ の素因数分解の時間計算量を $F(n)$，$k$ 以下の $n$ の正の約数の個数を $d(n, k)$ とするとき，これは $F(2N) + \Theta( d(2N, \sqrt{2N}) )$ 時間で可能です．

素因数分解は Pollard の ρ 法を用いると期待 $O(N^{1/4} )$ 時間で可能です．また $d(2N, \sqrt{2N})$ は本問題の制約下では $N = 985\,496\,152\,350\,226\,952\,635\,200$ のときの $737\,280$ が最大です．

---

さらなる考察により必要十分な列挙を行うこともできます．

$2N \eqqcolon 2^e \cdot r$ となるように正整数 $e$ および奇数 $r$ を取り，次の集合たちを考えます（文字はいずれも正整数とします）．
$$
  \begin{aligned}
    X_1 &\coloneqq \left\{\, (W, L) \;\middle|\; W(W + 2L - 1) = 2N  \,\right\}, \\\\
    X_2 &\coloneqq \left\{\, (W, W + 2L - 1) \;\middle|\; W(W + 2L - 1) = 2N  \,\right\}, \\\\
    X_3 &\coloneqq \left\{\, (a, 2^e b) \;\middle|\; a \cdot 2^e b = 2N \, (= 2^e r)  \,\right\}, \\\\
    X_4 &\coloneqq \left\{\, (a, b) \;\middle|\; a b = r \,\right\}, \\\\
    X_5 &\coloneqq \left\{\, a \;\middle|\; a \,\text{は}\, r \,\text{の約数} \,\right\}.
  \end{aligned}
$$

すると，$X_1$ と $X_2$ の間，$X_2$ と $X_3$ の間，$X_3$ と $X_4$ の間，$X_4$ と $X_5$ の間にそれぞれ全単射を定めることができます．

<details>
  <summary>全単射の構成</summary>

  $W$ と $W + 2L - 1$ の偶奇が異なることから，偶数の方は $2N$ に含まれる素因数 $2$ を総取りしている（$2^e$ の倍数である）ことに注意して，次のように構成できます．

  - $X_1 \to X_2 \colon \;$ $(W, L) \mapsto (W, W + 2L - 1)$ は明らか．
  - $X_2 \to X_1 \colon \;$ $(W, W + 2L - 1) \mapsto (W, L)$ は $L = [(W + 2L-1) - W + 1]/2$ より得られる．
  - $X_2 \to X_3 \colon \;$ $W$ と $W + 2L - 1$ の偶奇が異なるので，$W$ が偶数なら項を入れ替えて第2項が偶数としてよく，それは $2^e$ の倍数であるから $(a, 2^e b)$ とおける．
  - $X_3 \to X_2 \colon \;$ $(a, 2^e b)$ を昇順にソートする（$a > 2^e b$ なら項を入れ替える）と得られる．
  - $X_3 \leftrightarrow X_4 \colon \;$ $(a, 2^e b) \leftrightarrow (a, b)$ は第2項を定数倍すればよい．
  - $X_4 \to X_5 \colon \;$ $(a, b) \mapsto a$ は明らか．
  - $X_5 \to X_4 \colon \;$ $a \mapsto (a, b)$ は $b \coloneqq r / a$ とすればよい．

  以上で写像を定めることができました．然るべき写像のペアが互いに逆写像になっていることや，従ってこれらが全単射であることも，詳細は省きますが確認できます．

  $\square$
</details>

これらを合成することで，$X_1$ と $X_5$ の間の全単射が定まります．つまり，$r$ の正の約数と $E = 1$ の解が 1 対 1 に対応します．よって $r$ の正の約数が列挙できれば良く，これは $F(r) + \Theta(d(r, r))$ 時間で可能です．

$d(r, r)$ は本問題の制約下では $N = 846\,910\,755\,925\,976\,287\,420\,875$ のときの $368\,640$ が最大です．

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
より，この不等号を満たす最大の整数 $W$ を $W_2$ として，$W$ の範囲は $1 \le W \le W_2$ に限られます．ここで
$$
  \begin{aligned}
    N
    &\ge \frac{1}{6} W_2 (W_2 + 1) (2 W_2 + 1) \\\\
    &\gt \frac{1}{6} W_2 \cdot W_2 \cdot 2 W_2 \\\\
    &= \frac{1}{3} W_2^3, \\\\
    \therefore W_2 &\le \lfloor (3 N)^{1/3} \rfloor
  \end{aligned}
$$
であり，$W_2 \in \Theta( N^{1/3} )$ が示せます（より精密には，$W_2 = \lfloor (3 N)^{1/3} \rfloor$ もしくは $W_2 = \lfloor (3 N)^{1/3} \rfloor - 1$ が成り立ちます）．$W$ を固定すると，$S(2, L, L + W - 1)$ は $L$ について狭義単調増加するため，$S(2, L, L + W - 1) = N$ なる $L$ は二分探索により $\Theta(\log N)$ 時間で求められます．従って $E = 2$ の場合を $\Theta( N^{1/3} \log N )$ 時間で解くことができます．

## $W$ に関する枝刈り

$$
  S(2, 1, R) = \frac{1}{6} R(R + 1)(2R + 1)
$$
より，
$$
  6 \cdot S(2, 1, R) = R(R + 1)(2R + 1)
$$
は $R$ についての整数係数多項式なので，
$$
  6 \cdot S(2, L, L+W-1) = 6 \cdot S(2, 1, L+W-1) - 6 \cdot S(2, 1, L-1)
$$
は $W$ と $L$ についての整数係数多項式です．この多項式は $W = 0$ を代入すると
$$
  6 \cdot S(2, 1, L+0-1) - 6 \cdot S(2, 1, L-1) = 0
$$
と $0$ になります．よって因数定理より $6 \cdot S(2, L, L+W-1)$ は $W$ で整数係数多項式として割り切れます．つまり，解に対応する $W$ は $6 \cdot S(2, L, L+W-1) = 6N$ の約数といえます．

よって $W_2$ 以下の $6N$ の正の約数だけ調べればよく，$\Theta( d(6N, W_2) \log N )$ 時間が達成できます．

本問題の制約下における $d(6N, W_2)$ の最大値は，$N = 985\,496\,152\,350\,226\,952\,635\,200$ のときの $126\,085$ と思われます（未証明）．$126\,264$ 以下であることは証明されています．


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
より，この不等号を満たす最大の整数 $W$ を $W_3$ として，$W$ の範囲は $1 \le W \le W_3$ に限られます．ここで
$$
  \begin{aligned}
    N
    &\ge \frac{1}{4} W_3{}^2 (W_3 + 1)^2 \\\\
    &> \frac{1}{4} W_3{}^4, \\\\
    \therefore W_3 &\le \lfloor (4 N)^{1/4} \rfloor
  \end{aligned}
$$
であり，$W_3 \in \Theta( N^{1/4} )$ が示せるので，$\Theta( N^{1/4} \log N )$ 時間で解くことができます．また
$$
  \begin{aligned}
    2 \cdot S(3, L, L+W-1)
    &\equiv 2 \cdot S(3, 0, W-1) \\\\
    &\equiv \frac{1}{2} W^2 (W-1)^2 \\\\
    &\equiv W \cdot \frac{1}{2} W (W-1) \cdot (W-1) \\\\
    &\equiv 0 && \pmod{W} \\\\
  \end{aligned}
$$
より $W$ は $2N$ の約数であることが分かるので，$W_3$ 以下の $2N$ の正の約数だけ調べることで $\Theta( d(2N, W_3) \log N )$ 時間でも解くことができます．

本問題の制約下における $d(2N, W_3)$ の最大値は，$N = 985\,496\,152\,350\,226\,952\,635\,200$ のときの $24\,176$ と思われます（未証明）．$24\,213$ 以下であることは証明されています．


# まとめ

以上から，
- $E = 1$ の解を $F(2N) + \Theta( d(2N, \sqrt{2N}) )$ 時間で，
- $E = 2$ の解を $\Theta( d(6N, W_2) \log N )$ 時間で，
- $E = 3$ の解を $\Theta( N^{1/4} \log N )$ 時間もしくは $\Theta( d(2N, W_3) \log N )$ 時間で，
- $E \ge 4$ の解を $\Theta( N^{1/4} + N^{1/5} + \dots + N^{1/\lfloor \log_2 N \rfloor}) = \Theta(N^{1/4})$ 時間で，

それぞれ列挙でき，$d(n, k) \le d(n, n) \in o(n^{1/4})$ であるため，合わせて期待 $O( N^{1/4} \log N )$ 時間や $O( N^{1/4} )$ 時間でこの問題を解くことができます．


# 実装例

- $F(N) + \Theta( N^{1/4} )$ 時間（Pollard's rho 素因数分解，$W$ に関する枝刈り，尺取り法）：[PyPy3 (4482 ms)](https://yukicoder.me/submissions/1129524)，[C++23 (435 ms)](https://yukicoder.me/submissions/1129525)


# 補足

## 細かな実装の差異について

先の実装例は $E = 1, 2, 3$ の場合において解説通りに上界以下の約数（順に $d(2N, \sqrt{2N})$ 個，$d(6N, W_2)$ 個，$d(2N, W_3)$ 個）だけを列挙するものでした．しかし，上界を超えるものを含め一旦全て列挙してしまっても個数はそれほど多くならないため，このように実装しても良いでしょう：[PyPy3 (5944 ms)](https://yukicoder.me/submissions/1129534)，[C++23 (530 ms)](https://yukicoder.me/submissions/1129533)．実際 $d(2N, 2N), d(6N, 6N)$ の最大値は $N = 985\,496\,152\,350\,226\,952\,635\,200$ のときの $1\,474\,560, 1\,769\,472$ です．

一方，特に $E = 2$ の場合において $W$ を $[1, W_2]$ の範囲で全て試し，$6N$ の約数であるなら二分探索により $L$ を求め，そうでないなら単にスキップする $\Theta( N^{1/3} + d(6N, W_2) \log N )$ 時間の解法は，割り切れるか判定するために 128 bit 整数の剰余演算を $10^8$ 回程度行う必要があり，実行時間が著しく悪化します：[PyPy3 (> 10000 ms)](https://yukicoder.me/submissions/1129585)，[C++23 (5421 ms)](https://yukicoder.me/submissions/1129582)．


## 素因数分解について

Pollard の ρ 法で正整数 $N$ を素因数分解する際には，主に次の2つの計算ができればよいです．

1. $N$ 未満の非負整数 $a, b$ から $a b \bmod N$ を求める
2. $N$ 以下の正整数 $n$ に対する素数判定

ただし前処理により $N, n$ が $3$ 以上の奇数の場合にできれば十分です．

1つ目については，128 bit 整数型で Montgomery Reduction を利用すると実現できます．単純な実装では 256 bit 整数型が必要そうですが，$a, b$ をそれぞれ 64 bit 整数2つに分割して筆算をする要領で回避できます<sup> [1]</sup>．

2つ目については，Miller–Rabin 素数判定法を用いることにします．Miller–Rabin 素数判定法は一般には確率的なアルゴリズムですが，$n$ の上限が分かっている場合にはテストする底の集合をうまく選ぶことで決定的なアルゴリズムとしても使えます．より形式的には，

$$
  \forall a \in A. \;
  \big[ \, \text{$n$ は底 $a$ に対する strong probable prime ($a$-sprp)} \, \big]
$$

なるとき「$n$ は $A$-sprp である」と言うことにして（本節独自の用法），底の集合を $A$，上限を $U$ として

$$
   \forall n \in [3, U)\colon \text{奇数}. \;
    \big[ \, \text{$n$ は素数} \iff \text{$n$ は $A$-sprp} \, \big]
$$

が証明されているならば，$A$ の各元を底としてテストするだけで $U$ 未満の奇数 $n$ に対しては正しく決定的に素数判定ができることが保証される，ということです．中でも有用な（ $\# A$ は小さいが $U$ が大きい）$U$ と $A$ の組が多数報告されています<sup> [2], [3]</sup>．ここでは次の3組を紹介します<sup> [4], [5]</sup>．

<!--
if (n < 18446744073709551616_u128) return test_miller_rabin({2, 325, 9375, 28178, 450775, 9780504, 1795265022});
if (n < 318665857834031151167461_u128) return test_miller_rabin({2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37});
if (n < 3317044064679887385961981_u128) return test_miller_rabin({2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41});
-->
- $U_1 \coloneqq 18\,446\,744\,073\,709\,551\,616 \,(= 2^{64} \approx 1.8 \cdot 10^{19}), \; A_1 \coloneqq \{\, 2, 325, 9\,375, 28\,178, 450\,775, 9\,780\,504, 1\,795\,265\,022 \,\}$
- $U_2 \coloneqq 318\,665\,857\,834\,031\,151\,167\,461 \,(\approx 3.2 \cdot 10^{23}), \; A_2 \coloneqq \{\, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 \,\}$
- $U_3 \coloneqq 3\,317\,044\,064\,679\,887\,385\,961\,981 \,(\approx 3.3 \cdot 10^{24}), \; A_3 \coloneqq \{\, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 \,\}$

特に最後の3組目について $U_3 > 10^{24}$ なので，本問題の制約下では $A_3$-sprp 判定をすれば十分とわかります．

なお，一般に $A$ の保証範囲外には $A$-sprp である（テストを全て通過してしまう）合成数が存在します．実際上記の3組のうち初めの2組については $U_1, U_2 < 10^{24}$ であり，各々に対して反例となる合成数がテストケースに含まれています<sup> [5], [6]</sup>．


### 参考文献

[1] [128bit 整数型を使わない 64bit modint - Yu_212’s diary](https://yu212.hatenablog.com/entry/2023/12/14/203400)

[2] [Miller–Rabin primality test - Wikipedia](https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test#Testing_against_small_sets_of_bases)

[3] [SPRP bases](https://miller-rabin.appspot.com/)

[4] OEIS Foundation Inc. (2025), Smallest odd number for which Miller-Rabin primality test on bases <= n-th prime does not reveal compositeness., Entry A014233 in The On-Line Encyclopedia of Integer Sequences, https://oeis.org/A014233.

[5] Sorenson, J., Webster, J. (2016). Strong pseudoprimes to twelve prime bases. _Mathematics of Computation_, *86*(304), 985–1003. https://doi.org/10.1090/mcom/3134

[6] Shallue, A., Webster, J. (2024). Advances in tabulating Carmichael numbers. _Research in Number Theory_, *11*(1). https://doi.org/10.1007/s40993-024-00598-3
