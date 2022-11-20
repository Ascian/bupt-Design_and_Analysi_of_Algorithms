# 算法设计与分析实验报告

## 动态规划

### 实验题目

有N件物品和一个容量为$V$的背包。第$i$件物品的大小是$w_i$，价值是$v_i$。求解将哪些物品装入背包可使这些物品的费用总和不超过背包容量，且价值总和最大。

输入文件名为`bag.in`。输入共两行。第一行有2个整数$V$和$N$，用一个空格隔开，$V$代表背包容量，$N$代表物品数目。接下来的$N$行每行包括两个整数，分别表示某件物品的大小和其价值。 

输出文件名为`bag.out`。输出共两行。第一行包含1个整数表示最大价值，第二行为表示最优组合的0-1序列。

### 实验过程

#### 基本算法

此问题的形式化描述是，给定$c>0, w_i>0, v_i>0(1\leq i\leq n)$，要求找出一个$n$元0-1向量$(x_1, x_2,...,x_n)$, $x_i\in\{0, 1\}(1\leq i \leq n)$，使得$\sum_{i=1}^{n}w_ix_i\leq c$，而且$\sum_{i=1}^nv_ix_i$达到最大。因此，0-1背包问题是一个特殊的整数规划问题：

$$
max \sum_{i=1}^n v_i x_i
\left\{
       \begin{array}{lr}
		\sum_{i=1}^n w_i x_i \leq c\\
		x_i \in \{0, 1\} & 1 \leq i \leq n\\
       \end{array} 
\right.
 $$

设$(y_1, y_3, ..., y_n)$是所给背包问题的一个最优解，则$(y_2, y_3, ..., y_n)$是下面相应子问题的最优解：
$$max \sum_{i=2}^n v_i x_i
\left\{
    \begin{array}{lr}
    \sum_{i=2}^n w_i x_i \leq c - w_1y_1 \\
    x_i \in \{0,1\} & 2 \leq i \leq n \\
    \end{array}
\right. $$
因此0-1背包问题具有最优子结构性质。

设所给0-1子问题
$$max \sum_{k=i}^n v_k x_k
\left\{
       \begin{array}{lr}
		\sum_{k=i}^n w_k x_k \leq j\\
		x_k \in \{0, 1\} & i \leq k \leq n\\
       \end{array} 
\right. $$
的最优值为$m(i, j)$，即背包容量为$j$，可选物品为$i, i+1, ..., n$时，0-1背包问题的最优值。由0-1背包问题的最优子结构性质，可以简历计算$m(i, j)$的递归式如下：
$$m(i, j)=
\left\{
    \begin{array}{lr}
    max \{m(i+1, j), m(i+1, j-w_i) + v_i\} & j \geq w_i \\
    m(i+1, j) & 0 \leq j < w_i \\
    \end{array}
\right.$$
$$m(n, j) = 
\left\{
    \begin{array}{lr}
    v_n & j \geq w_n \\
    0 & 0 \leq j < w_n \\
    \end{array}
\right. $$
由此可设计0-1背包问题的动态规划算法。

首先求得$m(n, j)$

    int jMax = w[n] - 1 > c ? c : w[n] - 1;
	for (int j = 0; j <= jMax; j++)
		m[n][j] = 0;
	for (int j = w[n]; j <= c; j++)
		m[n][j] = v[n];

之后使用二重循环求出所有$m(i, j)(1 \leq i \leq n)$

    for (int i = n - 1; i > 1; i--)
	{
		jMax = jMax = w[i] - 1 > c ? c : w[i] - 1;
		for (int j = 0; j <= jMax; j++)
			m[i][j] = m[i + 1][j];
		for (int j = w[i]; j <= c; j++)
			m[i][j] = m[i + 1][j] > (m[i + 1][j - w[i]] + v[i]) ? m[i + 1][j] : m[i + 1][j - w[i]] + v[i];
	}
	m[1][c] = m[2][c];
	if(c >= w[1])
		m[1][c] = m[2][c] > (m[2][c - w[1]] + v[1]) ? m[2][c] : m[2][c - w[1]] + v[1];

最终得到一个大小为$n \times c$的二维矩阵，矩阵中每一个元素的获得都只需要常数级计算，因此该算法的时间复杂度为$O(nc)$。同时需要$n \times c$的二维数组存储二维矩阵，因此空间复杂度也为$O(nc)$。

若需要求出最优组合的0-1序列，只需要对二维矩阵进行分析即可得出。由问题的最优子问题性质可知，若$m(i, j)$与$m(i+1, j)$不等，则$(i, j)$处的最大值是在加入物品$i$后得到的，$x_i$应为1，否则为0，算法具体如下：

    for (int i = 1; i < n; i++)
	{
		if (m[i][c] == m[i + 1][c])
			x[i] = 0;
		else
		{
			x[i] = 1;
			c -= w[i];
		}
	}
	x[n] = (m[n][c]) ? 1 : 0;

#### 改进算法

基本算法有两个比较明显的缺点。其一是算法要求所给物品的重量$w_i(i \leq i \leq n)$是整数。其次，当背包容量$c$很大时，算法需要的计算时间较多，使用到重叠子问题的几率也很低，动态规划解决重叠子问题的优势基本没有体现出来。

我们容易观察到，函数$m(i, j)$是关于j的阶梯状单调不减函数。跳跃点是这一函数的描述特征，在一般情况下，函数$m(i, j)$可由其数量有限的跳跃点位移确定。因此，算法只需要计算并存储跳跃点而无需计算存储所有整数点。

用$p[i]$存储函数$m(i, j)$的所有跳跃点。初始时，$p[n+1]=\{(0, 0)\}$。由前文所求得的递归式可知，函数$m(i, j)$由函数$m(i+1, j)$与函数$m(i+1, j-w_i)+v_i$求得。因此函数$m(i ,j)$的跳跃点集$p[i]$包含于函数跳跃点集$p[i+1]$与函数$m(i+1, j-w_i)+v_i$的跳跃点集$q[i+1]$的并集中。

由于$(s, t)\in q[i+1]$当且仅当$w_i \leq s \leq c$且$(s-w_i, t-v_i)\in p[i+1]$。因此，$q[i+1]$可有如下方式获得：
$$q[i+1]=\{(j+w_i, m(i, j)+v_i)|(j, m(i, j))\in p[i+1]\}$$

同时，设$(a, b)$和$(c ,d)$是$p[i+1] \cup q[i+1]$的两个跳跃点，则当$c \geq a$且$d < b$时，$(c, d)$受控于$(a, b)$，$(c, d)$便不是$p[i]$的跳跃点，除受控点之外，$p[i+1] \cup q[i+1]$的其他跳跃点均为$p[i]$的跳跃点。

由此可设计0-1背包问题的改进动态规划算法。使用二维数组`pt`存储所有$p[i](1 \leq i \leq n)$的所有跳跃点，数组`head`存储$p[i]$第一个跳跃点在`pt`中的位置，设置变量`left`和`right`分别指向上一个已求得点集$p[i+1]$的起始点和最后一个点的位置，`next`指向当前点集$p[i]$存放下一个点的位置。

随后，由大到小依次计算$p[n], p[n-1], ..., p[1]$。

    for (int i = n; i >= 1; i--)
    {
        \*计算p[i]*\
    }

计算p[i]的过程，需要依次遍历$q[i+1]$中的点，并将$p[i+1] \cup q[i+1]$中的非受控点依次加入$p[i]$。

    for (int j = left; j <= right; j++)	
    {
        if (pt[j][0] + w[i] > c)
            break;
        Type y = pt[j][0] + w[i], m = pt[j][1] + v[i];	//新跳跃点
        \*非受控点加入q[i]*\
    } 

首先需要将新加入跳跃点左侧的点加入到当前点集$p[i]$

    while (k <= right && pt[k][0] < y)	
    {
        pt[next][0] = pt[k][0];
        pt[next++][1] = pt[k++][1];
    }

若新跳跃点处有着m值更大的跳跃点，则新跳跃点为受控点，不需要加入新点集，为方便起见将该点改为m值更大的跳跃点

    if (k <= right && pt[k][0] == y)	
    {
        if (m < pt[k][1])
            m = pt[k][1];
        k++;
    }

若新跳跃点不为受控点将其加入当前点集

    if (m > pt[next - 1][1])	
    {
        pt[next][0] = y;
        pt[next++][1] = m;
    }

随后，跳过新加入点右侧的所有受控点

    while (k <= right && pt[k][1] <= pt[next - 1][1])
		k++;

最后k将会指向第一个不受控于新跳跃点的跳跃点用于下一次循环。

将$q[i+1]$中所有非受控点加入后，离开循环，最后再将$p[i+1]$中剩余非受控点加入新点集$p[i]$，并修改`left`和`right`指向$p[i]$的起始点位置和最后一个点位置，`next`指向下一个点集$p[i-1]$的起始位置

    while (k <= right)
    {
        pt[next][0] = pt[k][0];
        pt[next++][1] = pt[k++][1];
    }
    left = right + 1;
    right = next - 1;
    head[i - 1] = next;

待所有跳跃点集都计算出，最后求出的跳跃点`pt[next - 1][1]`即为最大价值。上述算法中，计算$q[i+1]$需要$O(|p[i+1]|)$的时间，合并$p[i+1]$和$q[i+1]$并清除受控点也需要$O(|p[i+1]|)$的时间。从跳跃点集$p[i]$的定义可知，$p[i]$的大小不会超过$2^{n-i+1}$。由此可得，总的时间复杂度为
$$O\left(\sum_{i=2}^n |p[i+1]|\right) = O\left(\sum_{i=2}^n 2^{n-i}\right) = O(2^n)$$
由于需要存储所有跳跃点集，空间复杂度也为$O\left(\sum_{i=2}^n 2^{n-i}\right) = O(2^n)$。相比于基本算法，当$n$较大且$c$较小时需要更多的计算时间和辅助空间；当$n$较小而$c$非常大时，计算时间和辅助空间有较大的优化。

当所给物品质量$w_i$是整数时，$|p[i]| \leq  c+1 (1 \leq i \leq n)$，改进后算法的时间复杂度和空间复杂度可降至$O(min \{nc, 2^n\})$。

若需要继续求得最优组合的0-1序列，只需要对跳跃点数组`pt`进行分析就可求得。由$p[i]$定义可知，若$p[i]$中的跳跃点是$q[i+1]$的点，则该点处最大值是在加入物品$i$后得到，$x_i$为1；若是$p[i+1]$的点，则$x_i$为0。算法如下：

    Type j = pt[head[0] - 1][0], m = pt[head[0] - 1][1];	//m为容量c
	for (int i = 0; i <= n; i++)
	{
		x[i] = 0;
		for (int k = head[i + 1]; k <= head[i] - 1; k++)
		{
			if (pt[k][0] + w[i] == j && pt[k][1] + v[i] == m)	//跳跃点(j, m)为函数m(i+1, j-w[i])+v[i]的跳跃点
			{
				x[i] = 1;
				j = pt[k][0];
				m = pt[k][1];
				break;
			}
		}
	}

### 实验结果

测试集使用python快速生成，依据给定$(n,c)$自动随机生成$(w_i, v_i)$。共生成四个测试集，其中$(n,c)$分别等于$(10, 1000000), (1000000, 10)$，$w_i$分别为整数和浮点数。实验结果如下:

||$(10, 1000000)$,整数|$(1000000, 10)$,整数|$(10, 1000000)$,浮点数|$(1000000, 10)$,浮点数|
|---|---|---|---|---|
|基本算法|57|451|$\times$|$\times$|
|改进算法|0|182|0|error|

单位为ms，0说明花费时间小于1ms

当$w_i$为整数时，改进算法的效率明显好于基本算法。
当$w_i$为浮点数时，改进算法的的时间复杂度只与$n$有关，因此当$c$很大时，也几乎不会影响改进算法的时间效率。但当$n$较大时，时间复杂度和空间复杂度呈指数集增长，当$n  \geq 30$时，就已经无法在短时间内求出结果了。

### 实验总结 

通过本次试验中，本人一共尝试了两种算法解决0-1背包问题，一种基于动态规划通用思路，直接计算背包存放物品的最大价值，第二种对算法进行跳跃点优化后得到的改进算法。

经过测试可以看出，当所给物品重量$w_i$为整数时，背包容量越大，改进算法在时间和空间效率上的提升越大。当$w_i$为浮点数时，改进算法的的时间复杂度只与$n$有关，因此当$c$很大时，也几乎不会影响改进算法的时间效率。但当$n$较大时，时间复杂度和空间复杂度呈指数集增长，此时就不在适合使用改进算法解决问题。

基本算法改进思路：当$w_i$为浮点数且物品数量$n$很大时，使用改进算法显然不是一个好办法。此时可以对基本算法进行修改，改用`map({i, j}, m)`存储重叠子问题，背包容量便不会受到类型限制。同时当背包容量较大时，基本不会遇到重叠子问题，使用map存储不在需要对于那些不会用到的大量的子问题进行计算存储。

### 算法源代码

#### 基本算法

    template<class Type>
    void Trackback(Type** m, int* w, int c, int n, int* x)
    {
        for (int i = 1; i < n; i++)
        {
            if (m[i][c] == m[i + 1][c])
                x[i] = 0;
            else
            {
                x[i] = 1;
                c -= w[i];
            }
        }
        x[n] = (m[n][c]) ? 1 : 0;
    }

    template<class Type>
    Type Knapsack(int n, int c, Type* v, int* w, int* x)
    {
        Type** m = new Type* [n + 1];
        for (int i = 1; i <= n; i++)
            m[i] = new Type[c + 1];
        int jMax = w[n] - 1 > c ? c : w[n] - 1;
        for (int j = 0; j <= jMax; j++)
            m[n][j] = 0;
        for (int j = w[n]; j <= c; j++)
            m[n][j] = v[n];
        for (int i = n - 1; i > 1; i--)
        {
            jMax = jMax = w[i] - 1 > c ? c : w[i] - 1;
            for (int j = 0; j <= jMax; j++)
                m[i][j] = m[i + 1][j];
            for (int j = w[i]; j <= c; j++)
                m[i][j] = m[i + 1][j] > (m[i + 1][j - w[i]] + v[i]) ? m[i + 1][j] : m[i + 1][j - w[i]] + v[i];
        }
        m[1][c] = m[2][c];
        if(c >= w[1])
            m[1][c] = m[2][c] > (m[2][c - w[1]] + v[1]) ? m[2][c] : m[2][c - w[1]] + v[1];

        Trackback<Type>(m, w, c, n, x);
        Type result = m[1][c];
        for (int i = 1; i <= n; i++)
            delete[] m[i];
        delete[] m;
        return result;
    }

#### 改进算法

    #include <cmath>

    template<class Type>
    void Trackback(int n, Type* w, Type* v, Type (*pt)[2], int* head, int* x)
    {
        Type j = pt[head[0] - 1][0], m = pt[head[0] - 1][1];	//m为容量c
        for (int i = 0; i <= n; i++)
        {
            x[i] = 0;
            for (int k = head[i + 1]; k <= head[i] - 1; k++)
            {
                if (pt[k][0] + w[i] == j && pt[k][1] + v[i] == m)	//跳跃点(j, m)为函数m(i+1, j-w[i])+v[i]的跳跃点
                {
                    x[i] = 1;
                    j = pt[k][0];
                    m = pt[k][1];
                    break;
                }
            }
        }
    }

    template<class Type>
    Type Knapsack(int n, Type c, Type* v, Type* w, int* x, bool is_int = true)
    {
        int size = pow(2, n + 1) - 1;
        if (is_int)	//n为整数时，只需要min(n(c + 1), 2^n+1 - 1)大小的辅助空间
        {
            if (size <= 0 || n * (c + 1) < size)
                size = n * (c + 1);
        }		
        Type(*pt)[2] = new Type[size][2];

        int* head = new int[n + 2];	//指向跳跃点集p[i]起始点的位置
        head[n + 1] = 0;
        pt[0][0] = 0;
        pt[0][1] = 0;
        //设置left和right分别指向上一个已求得点集p[i+1]的起始点和最后一个点的位置
        //next指向当前点集p[i]存放下一个点的位置
        int left = 0, right = 0, next = 1;	
        head[n] = 1;	
        //依次求出p[n],p[n-1],...,p[1]
        for (int i = n; i >= 1; i--)	
        {
            int k = left;
            //遍历函数m(i+1, j-w[i])+v[i]的跳跃点，并将非受控点加入新点集p[i]
            for (int j = left; j <= right; j++)	
            {
                if (pt[j][0] + w[i] > c)
                    break;
                Type y = pt[j][0] + w[i], m = pt[j][1] + v[i];
                //加入新跳跃点左侧的所有跳跃点到当前点集
                while (k <= right && pt[k][0] < y)	
                {
                    pt[next][0] = pt[k][0];
                    pt[next++][1] = pt[k++][1];
                }
                //若新跳跃点处有着m值更大的跳跃点，则新跳跃点为受控点
                if (k <= right && pt[k][0] == y)	
                {
                    if (m < pt[k][1])
                        m = pt[k][1];
                    k++;
                }
                //若新跳跃点不为受控点将其加入当前点集
                if (m > pt[next - 1][1])	
                {
                    pt[next][0] = y;
                    pt[next++][1] = m;
                }
                //跳过新加入点右侧的所有受控点
                while (k <= right && pt[k][1] <= pt[next - 1][1])
                    k++;
                //此时k指向第一个不受控于新跳跃点的跳跃点
            }
            //将剩余非受控点加入新点集
            while (k <= right)
            {
                pt[next][0] = pt[k][0];
                pt[next++][1] = pt[k++][1];
            }
            left = right + 1;
            right = next - 1;
            head[i - 1] = next;
        }
        Trackback<Type>(n, w, v, pt, head, x);
        Type result = pt[next - 1][1];
        delete[] pt;
        delete[] head;
        return result;
    }




