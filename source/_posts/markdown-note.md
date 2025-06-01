---
title: markdown_note
catalog: true
header-img: img/header_img/roman.png
subtitle: The quick brown fox jumps over the lazy dog
top: 9
comments: true
date: 2025-06-01 05:02:58
tags:
catagories:
updated:
---

# Markdown note

## 字体
*斜体*或_斜体_
**粗体**
***加粗斜体***
~~删除线~~
在一行的末尾添加两个或多个空格，然后按回车键,即可创建一个换行(`<br>`)。

## [代码块识别](https://www.jianshu.com/p/05c41cf1a7c4)

## 链接 
欢迎来到[梵居闹市](http://blog.leanote.com/freewalk)
欢迎来到[梵居闹市](http://blog.leanote.com/freewalk "梵居闹市")

> 不同的 Markdown 应用程序处理URL中间的空格方式不一样。为了兼容性，请尽量使用%20代替空格。  
> `[link](https://www.example.com/my%20great%20page)`

> 如果是图片在前面加感叹号`！`即可   
如 `![xxx](xxx)`

> 图片带链接   
 `[![沙漠中的岩石图片](/assets/img/shiprock.jpg "Shiprock")](https://markdown.com.cn)`

我经常去的几个网站[Google][1]、[Leanote][2]以及[自己的博客][3]
[Leanote 笔记][2]是一个不错的[网站][]。

## 引用

> #### The quarterly results look great!
>
> - Revenue was off the chart.
> - Profits were higher than ever.
>
>  *Everything* is going according to **plan**.

> 第一层
> > 第二层
> 这样是跳不出去的
> >> 还可以更深

## 表格

| Syntax      | Description |
| ----------- | ----------- |
| Header      | Title       |
| Paragraph   | Text        |

### 表格对齐

| Syntax      | Description | Test Text     |
| :---        |    :----:   |          ---: |
| Header      | Title       | Here's <br> this   |
| Paragraph   | Text        | And more      |


---

### 文字居中
<center>Markdown</center>

### 文字左对齐
<p align = "left">Markdown</p>

### 文字右对齐
<p align="right" >Markdown</p>

---

### Markdown 字体颜色改变 
- [ ] 绿色字体：<font color =green>Markdown</font> 
- [ ] 红色字体：<font color = red>Markdown</font> 
- [ ] 蓝色字体：<font color =blue>Markdown</font> 

### Markdown 字体大小改变 
- [ ] size为1：<font size ="1">Markdown</font> 
- [ ] size为5：<font size = "5">Markdown</font> 
- [ ] size为10：<font size ="10">Markdown</font> 

 ### markdown 字体样式改变 

- [ ] [微软雅黑字体](https://www.zhihu.com/search?q=%E5%BE%AE%E8%BD%AF%E9%9B%85%E9%BB%91%E5%AD%97%E4%BD%93&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra=%7B%22sourceType%22%3A%22answer%22%2C%22sourceId%22%3A942069774%7D): <font face ="微软雅黑">Markdown</font> 

- [ ] 宋体字体：<font face = "宋体">Markdown</font> 
- [ ] 楷体字体：<font face ="楷体">Markdown</font>


## 数学函数公式

一个`$`不占一行
2个``$$``占一行

$$f(x, y) = 100 * \lbrace[(x + y) * 3] - 5\rbrace$$

$$ f(n)= \begin{cases} n/2, & \text {if $n$ is even} \\ 3n+1, & \text{if $n$ is odd} \end{cases} $$

顶点数组：
$
\begin{array}{|c|c|c|c|c|}
\hline
v_0 & v_1 & v_2 & v_3 & v_4 \\
\hline
\end{array}
$

边数组：
$
\begin{array}{c|c}
    \color{lime}{↓}&
    \color{yellow}\begin{matrix}
        v_0 & v_1 & v_2 & v_3 & v_4 \\
    \end{matrix}
    \\
    \hline
    \color{teal}
    \begin{matrix}
        v_0 \\ v_1 \\ v_2 \\ v_3 \\ v_4 \\
    \end{matrix}
    &
    \color{fuchsia}
    \begin{matrix}
        0 & ∞ & ∞ & ∞ & 6 \\
        9 & 0 & 3 & ∞ & ∞ \\
        2 & ∞ & 0 & 5 & ∞ \\
        ∞ & ∞ & ∞ & 0 & 1 \\
        ∞ & ∞ & ∞ & ∞ & 0 \\
    \end{matrix} 
\end{array}
$

## 数组 颜色

$$
\left[
\begin{array}{c:c}
\begin{matrix}
a & b & c  \\
d & e & f  \\
g & h & i  \\
 j & k & l
\end{matrix}&
\begin{matrix}
a  \\
b  \\
c  \\
d
\end{matrix}
\end{array}
\right]
\tag{6.1}
$$

文字A$
\bigl(
\begin{smallmatrix}
a & b  \\
c & d
\end{smallmatrix}
\bigr)
$文字B

$$\begin{cases}
a_1x+b_1y+c_1z=d_1\\
a_2x+b_2y+c_2z=d_2\\
a_3x+b_3y+c_3z=d_3\\
\end{cases}
$$


$
\begin{bmatrix}
{a_{11}}&{a_{12}}&{\cdots}&{a_{1n}}\\
{a_{21}}&{a_{22}}&{\cdots}&{a_{2n}}\\
{\vdots}&{\vdots}&{\ddots}&{\vdots}\\
{a_{m1}}&{a_{m2}}&{\cdots}&{a_{mn}}\\
\end{bmatrix}
$

---



$$\begin{array}{c|lll}
{↓}&{a}&{b}&{c}\\
\hline
{R_1}&{c}&{b}&{a}\\
{R_2}&{b}&{c}&{c}\\
\end{array}$$

## 数学阵列公式

$$
        \begin{matrix}
        1 & x & x^2 \\
        1 & y & y^2 \\
        1 & z & z^2 \\
        \end{matrix}
$$

$$
        \begin{pmatrix}
        1 & a_1 & a_1^2 & \cdots & a_1^n \\
        1 & a_2 & a_2^2 & \cdots & a_2^n \\
        \vdots & \vdots & \vdots & \ddots & \vdots \\
        1 & a_m & a_m^2 & \cdots & a_m^n \\
        \end{pmatrix}
$$

$\begin{matrix} 1 & 2 \\ 3 & 4 \\ \end{matrix}$
$\begin{pmatrix} 1 & 2 \\ 3 & 4 \\ \end{pmatrix}$
$\begin{bmatrix} 1 & 2 \\ 3 & 4 \\ \end{bmatrix}$
$\begin{Bmatrix} 1 & 2 \\ 3 & 4 \\ \end{Bmatrix}$
$\begin{vmatrix} 1 & 2 \\ 3 & 4 \\ \end{vmatrix}$
$\begin{Vmatrix} 1 & 2 \\ 3 & 4 \\ \end{Vmatrix}$

$$
\left[
    \begin{array}{cc|c}
      1&2&3\\
      4&5&6
    \end{array}
\right]
$$

$$
        f(n) =
        \begin{cases}
        n/2,  & \text{if $n$ is even} \\
        3n+1, & \text{if $n$ is odd}
        \end{cases}
$$

$$
        \left.
        \begin{array}{l}
        \text{if $n$ is even:}&n/2\\
        \text{if $n$ is odd:}&3n+1
        \end{array}
        \right\}
        =f(n)
$$

$$
\left\{
\begin{array}{c}
a_1x+b_1y+c_1z=d_1 \\
a_2x+b_2y+c_2z=d_2 \\
a_3x+b_3y+c_3z=d_3
\end{array}
\right.
$$

$$
\begin{cases}
a_1x+b_1y+c_1z=d_1 \\
a_2x+b_2y+c_2z=d_2 \\
a_3x+b_3y+c_3z=d_3
\end{cases}
$$

$$
x = a_0 + \cfrac{1^2}{a_1
          + \cfrac{2^2}{a_2
          + \cfrac{3^2}{a_3 + \cfrac{4^4}{a_4 + \cdots}}}}
$$

$$
x = a_0 + \frac{1^2}{a_1+}
          \frac{2^2}{a_2+}
          \frac{3^2}{a_3 +} \frac{4^4}{a_4 +} \cdots
$$

$$
\begin{array}{cc}
\mathrm{Bad} & \mathrm{Better} \\
\hline \\
e^{i\frac{\pi}2} \quad e^{\frac{i\pi}2}& e^{i\pi/2} \\
\int_{-\frac\pi2}^\frac\pi2 \sin x\,dx & \int_{-\pi/2}^{\pi/2}\sin x\,dx \\
\end{array}
$$

$$
\begin{array}{cc}
\mathrm{Bad} & \mathrm{Better} \\
\hline \\
\{x|x^2\in\Bbb Z\} & \{x\mid x^2\in\Bbb Z\} \\
\end{array}
$$

$$
\begin{array}{cc}
\mathrm{Bad} & \mathrm{Better} \\
\hline \\
\iiint_V f(x){\rm d}z {\rm d}y {\rm d}x & \iiint_V f(x)\,{\rm d}z\,{\rm d}y\,{\rm d}x
\end{array}
$$

$$
\begin{array}{cc}
\mathrm{Bad} & \mathrm{Better} \\
\hline \\
\int\int_S f(x)\,dy\,dx & \iint_S f(x)\,dy\,dx \\
\int\int\int_V f(x)\,dz\,dy\,dx & \iiint_V f(x)\,dz\,dy\,dx
\end{array}
$$

<http://example.com/>
<address@example.com>

---


Here is a footnote reference,[^1] and another.[^longnote]

## Endnotes

[^1]: Here is the footnote.
[^longnote]: Here's one with multiple blocks.

[1]:http://www.google.com "Google"
[2]:http://www.leanote.com "Leanote"
[3]:http://http://blog.leanote.com/freewalk "梵居闹市"
[网站]:http://http://blog.leanote.com/freewalk
