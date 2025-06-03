---
date: 2023-05-31 06:25:21
top: 99999910
title: 高质量C++读书笔记
catagories: 
- 计算机科学
---

[cppreference.com](https://zh.cppreference.com/w/%E9%A6%96%E9%A1%B5)

# 高质量C++读书笔记
## 1. C++/C 程序 基本概念

内存页
缺页中断 页面调度 页面交换

动态内存分配 运行时搜索

`ocrmypdf  --sidecar test4.txt -l chi_sim+eng --deskew --jobs 4 -s 高质量程序设计指南_C++_C语言_第三版_扫描版.pdf test4.pdf`

内部名称 `_main`

C的连接规范
C语言中
编译单元（文件作用域） static函数
extern 连接类型 global 作用域的 全局函数

C++
作用域：编译单元 class struct union namespace
同一个作用域同名函数 —— 重载函数

name-manglin 
```C++
class Sample_1
{
	char m_name[16];
public:
	void foo(char* newName);
	void foo(int age);
}

Sample_1_foo@pch@1 
Sample_1_foo@int@1 
Sample 2_foo@pch@1
Sample_2_foo@int@1
Lippman的《inside The C++ object model》
```
连接规范 linkage specification

关系到编译器采用什么样的name-mangling方案重命名这些标识符的问题

同一个标识符在不同的编译单元或模块中具有不一致的连接规范 就会产生不一致的内部名称 导致程序连接失败

通用连接规范属 C连接规范 extern "C"
```C++
// 类型 函数 变量 常量 指定连接规范
extern "C" void WinMainCRTStartup();
extern "C" const CLSID CLSID_DataConverter;
extern "C" struct Student {......};
extern "C" Student g_Student;

// 代码限定连接规范
#ifdef _cplusplus
extern "C"{
#endif

...

#ifdef _cplusplus
}
#endif

// 声明指定某个标识符 连接规范为 exter"C" 其对应定义也要指定extern "C"
#ifdef _cplusplus
extern "C"{
#endif

int _cdecl memcmp(const void*, const void*, sizt_t); //声明

#ifdef _cplusplus
}
#endif

#ifdef _cplusplus
extern "C"{
#endif

int _cdecl memcmp(const void* p, const void* a , sizt_t len)
{
	....//功能实现
}

#ifdef _cplusplus
}
#endif
```

C++/C 中 ， 全 局 变量 (extern或 static 的 ) 存放 在 程序 的 静态数据 区 
全 局 变量 提供 初 值 编 译器会 自动 地将 0 转换 所 需要 的 类 型 来 初始 化 它们

无 法 决定 当两 个 编译 单元连接 在 一 起 时 哪 一 个 的 全 局 变量 的 初始 化 于 另 一 个 编译 单元 的 全 局变量 的 初始 化

启动 函数 、IO 系统 函 数 存储 管理 、RTTI、 动态决议 动 态 链接 〈DLL) 等 都 会 调用 C 运行 时 库中 的 函数
C 运行 时 有 多 线程 和 单线 程


编译 预 处 理 、 编译 和 连接 器 工作 的 阶段 合 “ 编 译 ”。

预 编译 伪 指令 类 〈 型 定义 外部 对 象 声 、 函 数 原型、 标识符 、 各 种修饰符号 (const、static 等 ) 及 类 成 员 的访问说 明 符 (public、private、protected) 和 连接 规范 调 用 规范 等仅在 编译 器进行 语法检查 、 语 义 检查 和 生成 目标文件 〈.obj 或 .o 文件 ) 及 连接 的 时 候起 作用 的

容器 越界，访问 虚 函数 ，动态 决议 ，函 数 动态 连接 、 动 内 存 分 配 异 常 处 和 RTTI等则 是在 运行 时 才 会 出 和 发 作用

```C++
int *pInt = new int[10];
pint += 100;                         // 越界 但 是 还 没有 形成 越界 访问
cout << *pInt<<endl;          // 越界 访问 ! 可 能 行 也 可 能 不 !
*pInt = 1000;                      // 越界访问 ! 即使偶尔 不 出问题 但 不能 确保永远不 出问题 


class Base {
public:
	virtual void Say() { cout<< "Base::Say() was invoked!\n"; 
};
	
class Derived : public Base {
private: / 改变 访问 权限 合 法但 不 是好 风格 !
	virtual void Say(){ cout << “Derived::Say() was invoked!\n”;}
};
// 测试
Base *p = new Derived;
p->Say(); //输出: Derived::Say() was invoked !
// 出 乎 意料 地绑 定 到 了 一 private 函数 身上 


```
独立编译技术
每 一 个 源代| 码 文件 〈 源 文件 及 其 递归 包含 的 有 头 文件 展开 ) 就 是 一 个 最小 的 编译 单元 每 个 编译 单元 可 以独立 编译 而不 需要 知道 他 编译 单元 的 存在 及 其 编译 结果 。


32位 操作 系统 上 ，int 类 型 的 变量 就 拥有 4 字 节 的 内 存单 元 而 double 类 型 的 变量 占 8 字 节 的 内 存单

	标准 C 语言 支持 基本 〈 内 ) 数据 类 有 int、long、float、double、char、void， 以 及 它们 和 signed、unsigned、*、& 等 的 组 《 有些组 合 是 不 支持 的 例 如 void& )。 标准 C++ 在 这 些 类 型 的 基础 上 增加 了 bool 类 型 并 同时 增加 了 两 内 置 的 符号 常量 true All false (Se).

 标准 C 中 ，int 为 默认 类 型 也 就 是 说 如果 你 明确 指定 函数 的 形 类 型 或函数返回值类型 则它们的类型为 int,
标准C++不支持默认类型，但在模板中 有 “默认 类 型 参数 ”的 概念 。


某些 基于 RISC《〈 精 简 指令 集计 算 机 ) 的 CPU 比如 SPARC、PowerPC 等 
对 内存 中 基本 数据类 型 变量 采用 
高 字 节 (BYTE) 和 高 (WORD) 在 低地 址 存放 、 低字节 和 低 字 在 地 址 存放 的 
Big Endian 存储格式 〈 即 高 字 节 、 高 字 在 前 或 地 址 大的 字 节 结尾 ), 
并 且 把 最 高 字节 的 地 址 作为 变量 的 首 地址

。在 这 种 自然 的 存储 格式 中 ，
| 要求 变 量在 内 存 中 的 存放 位 必须 自然 对 ,否则 CPU 会 报告 异常 所 谓 自然 对齐 ，
| 就 基本数据 类 〈 主 要 short、int 和double) 的 变量 不 能 简单 地 存储 于 内 存 中 的
任意 地 址 处 它 们 的 起 地 址 必须 能 够 被 它们 的 大 小 整除

在 32 位 平台 下 ，int 和 指针 类 型 变量 的 地 址 应 该 能 4 整除，而 short 变量 的 地 址 都应 该 是 偶数 ，bool和 char 则 没有 特别 要 求

![](高质量C++读书笔记/Pasted%20image%2020230531104928.png)

Intel 系列 CPU 采用 Little Endian 存储 格式 来 存放 基本 类 型 变量 即 低 字节和低字 在 低地 址 存放 、 高 字 节 和 高 字 在 地 址 存放 《〈 即 低 字 节 、 低 字 在 前 或地 址 小 的 字 节 结尾 )， 并 且 把 最低 字节 的 地 址 作为 变量 的 首 地 址

![](高质量C++读书笔记/Pasted%20image%2020230531105144.png)
 ## 隐式转换
 显 地 使 用 强制 类 型 转换由 此 可 能 造成 的 安全 隐患 由 程序 员 负 责
 这 里 安全 性 主要包括 两 个 方面 内 存单 元访问 的 安全 和 转换 结果 的 安全 性
 主要 表现 
 内 存 访问 范围 的 扩张 
 内 存 的 截断 
 尾 数 的 截断 
 值 的 改变 和 
 溢出
 一 个 低级 数据 类 型 对 象 总 是 优先 转换 为 能 够 容纳 得 下 它 的 大 值 的、 占用 内 存;最少 的 高 级 类 型 对 象
 
示例 4-5 中 的 转换 是 安全 的 并 不 需要 强制 编 译 器 

首先 隐 式 地 100 提升: double《〈 作 为 它 的 整数 部 分 的 一 个 临时 变量 
然 才 将这 个 临时 变量 赋值 dl;
同样 ，i 也 会 首先 隐 式 地 提升 double《〈 其 值作为 它 的 整数 部 ) 的 一 个 临时 变量 ，
| 然后 才 赋值 d2。 
当 编译 器 认为 这 些 临时 变量 不再 需要 时 就 适时 地 把 它们 销毁 
```C++
示例 4-$
double di = 100;
int i= 100;
double d2 = i
```

 直接 将派生 类对 象 转换 基 类 对 ，这 虽然 会 生 内 存 截断 但 是 无 从 内 存 访问 还 从 转换 结果 来说 都是 安全 
 
![](高质量C++读书笔记/Pasted%20image%2020230531114341.png)
![](高质量C++读书笔记/Pasted%20image%2020230531114341.png)

[新特性](#新特性.md)

 C 语言 环境 中 我们 可 以 先 把 一 种 具体 类 型 指针 
 如 int* 转 换 void* 类 型 然 
 再 把void* 类 型 转换 double* 类 型 
 而 编译 器 不 认为 这 是错误 。 然 这样的 做 法 确实 存在 着 不 易 察 党 的 安全 问题 
 ( 内 存 扩张 和 截断 )， 这 是 标准C 语 言 的 一 个 缺陷 

### 强制转换

### 数据类型转换
```C++
double d3 = 1.25e+20;
double d4 = 10.25;
int i2 = (int)d3;
int i3 = (int)d4;

按照 从 浮 点 数 到 整 型 数 的 转换 语义 结 果 应 是 截 去 浮 点 数 的 小 数部分而 保留其 整数 部分
因 此 i 会得 到 10，

d3 的 整数 部 分远远 超 了 一 个int 所 能 表示 的 范围 
i2 会 溢出
```

### 指针转换
基本 数据 类 型 间 的 **指针转换** 一 般 说 必然 造成 内 存 截 断 或 内 存 访问 范围扩张
在 32 位 系统 ，int、long、float 都 具有 4 字 节 的 空间
虽 然 不 会 造成 内 存 截断 内 存 扩张
 它们 之间 的 **指针 转换**
改变 了 **编译 器**对 指针 所指向 的 内 存单 元 的 **解释方式**

```C++
示例 4-8
double d5 = 1000.25;
int *pInt = (int*)&d5;
int i4 = 100;
double *pDbl = (double*)&i4


从 内 存 访问 角度 来 , 你 通过 pInt 访问 它 指向 double 类 型 变量 d5 是 安全 的
(后面的 4 字 节 “截断 ”了 ， 可访问 内 存 范围 缩小 )，但 是 其 值 绝对 不 会 d5 的 整数部分1000， 
而 是 位 d5 开头 4 字节 中 的 内 容 并 解释 int 类 型 数 
这 个 数 是 不可预料 的 


通过 pDbl 访问 int 类 型 变量 这 ， 得 到 的 数据 不一 定 就 100， 况 且造成 了 可 访问 内 存 范围 “扩张 >。 
如 果 你往 里 面 写 数据 就 会 产生 运行 时 错误

```

![](高质量C++读书笔记/Pasted%20image%2020230531131558.png)


```C++
Base objB2;
Derived *pD2 = (Derived *)&objB2;

存在 的问题 : 通过 pD2 能 够 访问 的 内 存范围 “扩张 ”了 4 字节 ,如果 访问 m_c可 能 引发 运行 时 错误 ,因为 pD2 指向 的 对 象 根本 就 没有 成 m_e 的 空间

```

![](高质量C++读书笔记/Pasted%20image%2020230531131959.png)


(1) 不 可 以 **基 类** 对 象 直接 转换 为 **派生 类** 对 象 无 论 直接 **赋值** 还 是**强
制 转换** 因 为 这 不 “自然 的 ”;
(2) 对 于 **基本 类 型** 的**强制 转换** 一 定 要 区 分 **值的截断** 与 **内 存 截断** 的 不 同;
(3) 如 果 你 坚持 要使 用强制 转换 必 须 同时 确保 **内存访问的安全性** 和 **转换结果的安全性** ;
(4) 如 果 确信 需 要 数据 类 型 转换 请 尽量使 显 式 ( 即强制 ) 数据 类型 转换 让 人 们 知道 发 了 什么 事 避 免让 编译 器 静 悄悄地 进行 隐 式 的数据 类 型 转换 。

### 标识符
标 识 符 可 以 任意 长 
但 是 标准 C 语言规定 编 译器 只 取 前 31 个字符 作为 有 效 的 标识符 
而 标准 C++ 则 取 前 255 个 字符 作为 有 效 的 标识 。


每 一 个 标识 符 都 具有 如 下 的 几 个 属性 
值 
值 类 型 
名 字 、 
存 储 类 型 
作 用域范围 
连 接 类 型 （可 见 性)、 
生 存 期 等

C 函数 函 数 名 其 实 就是 
函数 代码
在 内 存 中 的 首 地 址 在 编译 时 就 可 以 确定 其 值 因 此 是 一 个 常量 这 是 它 的 值 
值的 类 型 是 函数 指针 类 型 
存 储 类 型 默认 extern， 除 非 声明 为 static;， 
作 用 域 范围为 文件 作用域 ; 
连接 类 型 默认 为 外 连接 , 除非 声明 为 static; 
生存 期 为 永久 〈 即 静态 )。

### 转义序列 
### 运算符
#### 算术运算符 关系运算符 逻辑运算符
#### 函数调用 类型转换 成员选择
类型转换运算符 
运行时类型识别运算符（typeid）
作用域解析(::)
动态内存分配 释放
类成员指针运算符
#### 运算符基本特性就是优先级和结合律
![](高质量C++读书笔记/Pasted%20image%2020230531133944.png)


### 表达式
### 常量表达式 算术表达式 关系表达式 逻辑表达式 复合表达式 逗号表达式 条件运算符表达式 位运算表达式

常量表达式在编译时就可求值

能 够 在 编译 时 求 的 程序 元 素 是 否需要 分 配 运行 时 的 存储 空间 呢 ?
1. 基本 数据 类 型 的 字面常量 枚举常量 、sizeof()、 常 量 表达 式 等 就 不需要分配存储 空间 ， 因 此 也 没有 存储 类型

2. 字符 串 常 量 、const 常量 (尤其 ADTUDT 的 const 对 ) 都 要 分配 运行 时 的 存储 空间 即 有 特定 的 存储 类 型

在 用 运算 “&&” 的 表达 式 ， 要 尽量 把 有 可 能 FALSE 的 表达 式 放在 “&&” 的 左边
在 用 运算 “||” 的 表达 式 中 要 尽量把 有可 能 TRUE 子表达式 “||” 左边.

### 基本控制结构
### 选择判断结构

```C++
假设 有 两 个浮 点 变量 x 和 y， 精 度 定义 EPSILON = le-6， 则 错误 的 比较 方式
如下:
	if(x = y) / 隐 含 错误 的 比较
	if(x != y) / 隐 含 错误 的 比较
应 该 转化 为 正确 的 比较 方式 :
	if(abs(x - y) <= EPSILON) Ux Fy
	if(abs(x - y) > EPSILON) Ux BEF y
同 理 ，x 与 零 值比较 的 正确 方式 :
	if(abs (x) <= EPSILON) /x 等 0
	if(abs (x) > EPSILON) Ix REF 0
```

if (NULL == p)

![](高质量C++读书笔记/Pasted%20image%2020230531135348.png)

![](高质量C++读书笔记/Pasted%20image%2020230531135436.png)

![](高质量C++读书笔记/Pasted%20image%2020230531135630.png)


数组 元 素 的 访问 是 真正 的 随机 访问 〈 直 接地 址 计算 )。 如 果 整个 数组 能 在 一
内 存 页 中 容纳 ， 那 么 在 对 整个 数组 进行操作 的 过 中 至少不 会 了 访问 数组 元 素
而 出 现 缺 页 中 断 页 面 调度 和 页 交换 等 情况 只 需要 一次 外 存 读取 操作 就 可 以
数组 所 在 的 整个 页 面 入 内 存 然 后 直接 访问 内 存 可 了 

在 示例 4-14 中 ， 左 边 的 写法 比 右边 的 写法 多 执行 Nr] 次 逻辑 判断 并 且前 者
的 还 辑 判断 打 了 循环 “流水 线 ” 作 ， 使 得 编译 器 不 能 对 循环 进行 优化 处 ，
降低 了 效率 如 果 和 非常 大 最好 采用 右边 的 写法 可 以 提高 效率 如 果 N 非常 小 ，
两 者 效率 差别 并不 明显 采 用 左边 的 写法比较 ， 因 为 程序 更 加 简洁 。

![](高质量C++读书笔记/Pasted%20image%2020230531140922.png)

## 2. 第五章 常量

### 字面常量

```C++
x = -100.25f;
#define OPEN SUCCESS 0x00000001
char c = 'a';
char *pChar = "abcdef"; / 取 字 符 串常量的地址
intt *pint = NULL;

保存在符号表里 无法取地址

```

编译链接环境支持常量合并的 请打开

### 符号常量 

`#define` 预编译伪指令 字面常量
const 分配存储空间 外连接

### C++ 中 ，const定义 的 常量 要 具体情况 具体 对 待 
对 于 **基本数据 类 型 的 常量** 编 译 器 把 它 放 到 符号 表 中 而不 分 配 存 储空间 , 
而 ADITVUDT 的 **const 对 象** 则 需要 分配 存储 空间 (大对象)。
还 有 一 些情况 下也 需要 分 配 存储空间 ， 
	例如 强制 声明 为 extern 的 符号常量 
	或者 取 符号常量的地址 等 操作 ， 
	都 将强迫 编译 器 为 这 些 常量 分 配 存储 空间 以 满足 用 户 的 要 求 

对于 **基本 数据 类 型 const 常量** 编 译器 重新 在 内 存 中 创建 它 的 一 个 **拷贝**
你 通过 其 地 址 访问 到 的 就 这 个 拷贝 而 非 原始的 符号 常量

 对 于 构造 类 型 const 常量 实 上 它 是 编译 时不 允许修改 的 变量 ，
因此 如 果 你 能 绕 过 **编译 器 的 静态 类 型 安全 检查机 制** 就 可 以 在 运行 时 修改 内 存单 元，

```C++
const long Ing = 10;
long *pl = (long*)&lng; / 取 常 量 的 地 址
*pl = 1000; //迂回修改
cout << *pl << endl; / 1000， 修 改 的 是 拷贝 内 容 !
cout << Ing << end]; / 10， 原 始 常量 并 没有 变 !
class Integer {
public:
	Integer() : m_Ing(100) { }
	long m_Ing;
}
const Integer int_1;
Integer *pInt = (Integer*)&int 1; // 去除常数属性
pint->m_Ing = 1000;
cout << pInt->m_Ing << endl; // 1000, 修改const 对象
cout << int_].m_Ing << endl; // 1000, 迂回修改成功

```


1. 在 标准 C 语言 中 ，const 符号 常量 默认 是 外 连 楼 〔 分 配 存储 )， 
也 就是说你不能在两个或以上编译单元中同时定义一个同名的const符号常量
也不能 把 一 个const 符号 常量 定义 放 在 一 个头文件 中 而 在 多 个 编译 单元 中 同时 包含 头 文件 

2. 但 是 在 标准 C++ const符号 常量默认 是 内 连接 的 因 此 可 以 定义 在 头 文件 。 
当 在 不 同 的 编译单元 中 同时 包含 头 文件 时 编 译 器 认为 它们 是 不 同 的 符号 常量 
因 此每 个 编译 单元独立 编译 时 会 分 别 为 它们 分配 存 储 空间 ， 
而 在 连接 时 进行常量 合并 

### 契约性常量 

```C++

示例 5-3
void ReadValue(const int& num)
{
	cout << num;
}
int main(void)
{
	int n= 0;
	ReadValue(n); // 契约性 const, n 被 看 做 const
}
```

### 枚举常量

标 C++规定 枚 举 常量 的 值 可 以 扩展 的 并 非 受 限 于 一 般 的 整 数 的 范围
```C++
enum Gigantic
{
	SMALL = 10,
	GIGANTIC = 300000000000
}
```
至于 底层 如 何 实现 则 依赖 于 具体 的 环境 和 编译 器 厂商 可 能 会 不 同 的 语义 ，
请 查看 编译 器 文档 


以 C++ 程序 中 应 尽量 使 const 来 定义 符号 常量 包 括 字符 串 常量 。


非 静态 const 数据 成 员 是 属于 每 一对 象 的 成 员 **只在某个对象生存期内是常量** 
而 对 于 整个 类 来 说 它 是 可 变 ，
除非 static const。

因 为 类 可 以 创建 多 个 对 象 不 同 的 对 象 const 数据 成员 的 可以 不 同

不 能 在类声明 中初始 化 非 静态 const 数据 
```C++
class A
{
	const int SIZE = 100;   / 错误 企 图 在 类 声明 中初始 const 数据 员
	int array[SIZE];           / /错误 未 知 SIZE
};
```

非 静 const 数据 员 的初始化 只 能 在 类 的 构造 函数 的初始 化 列表 中 进行
```c++
class A
{
	...
	A(int size); / 构造 函数
	const int SIZE ;
};

A::A(int size) : SIZE(size) / 构造 函数 的初始 化 列表
{
	...
}
A all00);   // HR a HK SIZE (AA 100
A b(200)   // AR b HY SIZE {4% 200
```
怎样才能 建立 在 整个 类 中 都 恒定 的 常量 ? 别 指望 const 数据 成员 了 ， 
应该 用 类 中 的 枚 举 常 量 来 实现
枚举 常 量 不会 占用 对 象 的 存储 空间 它 们 在 编译 时 被 全 部 求 值 
更 何况 它 定义的 是 一 个 匿名 枚 举 类 型 
枚 举 常 量 的 缺点 是 不 能 表示 浮 点数 〈 如 PI-=3.14159) 和 字符串

 可 以 用 另 一 种 方法 来 定义 类 的 有 对 象 共享 的 常量 ，即 static const，
```C++
示例 5-8
class A
{
	public: // 有 些语言 实现 可 能 不 支持 这样 的初始 化 如 Visual C++
	static const int SIZE1 = 100; / 静态常量 员
	static const int SIZE2 = 200; / 静态 常量 员
	private:
	int array 1[SIZE1]; // 普通 员
	int array2[SIZE2]; // 普通 员
};
```
### 实际 用 中 如 何 定 义 常量
#### 1. C程序
##### 1.1 多个编译单元或模块公用 
1.1.1 在某个公用头文件中将符号常量**定义为static并初始化**
```C++
// CommonDef.h 
static const int MAX+LENGTH =1024;
``` 
然后每一个使用它的编译单元 `#include` 该头文件即可；
1.1.2 在头文件中使用 **宏定义**
1.1.3 在 某个公用的头文件中将符号常量声明为**extern**
```C++
// CommonDef.h
extern const int MAX_LENGTH;
```
并且在某个源文件中**定义一次**; 
```C++ 
const int MAX_LENGTH = 1024;
```
然后每个使用它的编译单元`#include` 上述头文件即可
1.1.4 整形常量 在某个公用**头文件**定义**enum**类型 然 后 每 一 个使 用 它 的 编 详单 `#include` 该头 文件 即 可 
##### 1.2 只为一个编译单元使用
1.2.1 直接于编译单元开头位置将符号常量定义为static 并初始化
```C++
foo.c
static const int MAX_LENGTH = 1024;
```
1.2.2 同1.1.1
1.2.3 同1.1.3 或在编译单元开头定义enum类型

#### 2. C++程序
##### 2.1 多个编译单元或模块公用 
2.1.1 在某 个 公用 的 头 文件 中 直接 在 某 名 字 空间 中 或者 全 局 名 字 空间 中 定义 符号常量 并 初始 化 有 无static无 所 谓 )， 例 :
```C++
// CommonDef.h
const int MAXLENGTH = 1024;
```

每一个使用它的编译单元 `#include` 该 头 文件 即 可 
2.1.2 在 某 公用 头 文件 中 并 且 在 某 名 字 空 间 中 或全 局 名 字 空 间 中 将 符号 常量 声明为 extem 的 例 :
```C++
// CommonDef.h
extern const int MAX LENGTH;
```
并 且在 某 个 源 文件 中 定义 一次 并 初始 :
```C++
const int MAX_LENGTH = 1024;
```
然后 每 一 个使 用 它 的 编 详 单元 机 nclude 上 述 头 文件即可 
2.1.3 同1.1.3
2.1.4  定义 为 某 一 个 公用 类的 static const 数据成 员 并 初始
或 者定义为 类 内 的 枚 举 类 型 例 如 :
```C++
// Utility.h
class Utility {
public:
static const int MAX LENGTH;
enum {
TIME_OUT= 10
hs
// Utility.cpp
const int Utility:: MAX LENGTH = 1024;
```
然后 每 一 个使 用 它 的 编 详 单元 机 nclude 上 述 头 文件即可 
##### 2.2 只为一个编译单元使用
2.2.1 同2.1.1
2.2.2 直接 于 该 编译 单元 〈 源 文件 开头 定义 符号
量 并 初始 〈 有 无 static 无所 谓 )， 例 如 :
```C++
// foo.C
const int MAXLENGTH = 1024;
```
2.2.3 同1.2.3

### 总结

在 C 程序 中 ，const 符号 常量 定义 的默认 连接 类 (Linkage) 是 extern 的 即 外连接 (external linkage)， 就 全 局 变量 一 样 
因 此 ， 如 果 要 在 头 文件 中 定义 必 须使用 static 关键字 ， 
这 样 每 一 包含 头 文件 的 编译 单元 就会 分 别 有 该 常量 的 一份独立定 义 实体 〈 如 同 直接 在 每 一 个源 文件 中 分 别 定义 一 )， 
否则会 导致 “redefinition”的 编译 器诊断 信息 ;
如果 在源 文件 中 定义 除 非 明确 改变 它 的 连接 类 型 static 〈 实际 上 是 存储 类 型 static, 连接 类 型 内 连接 ) 的 ,
否则 其他 编译 单元 可 以通过 extern声明 来访问 


 C++ 程序 ，const 符号 常量定义 的 默认 连接 类 型 却 static 的 即 内 连(Cinternal linkage)， 
 就 class 的 定义 一 样 这就是在 头 文件 中 定义 而 不 需要 static关键 字 的 原因 

字符 串 常量 的 定义 和 整 型 常量 的 定义 差不多 ， 但 是其类 型 `const char *`， 因 此
们 常常 这样 定义 它们 ;

const char* const ERR_DESP NO_MEMORY = "There is no enough memory!";

可 以 在头 文件 中 定义 并初始 ， 也 可 以 在 源 文件 中 定义 并 初始 化 但 是 二 者 差别 大 :

1. 如 果 在 头 文 件 中 定义 并 初始 化 那 包含 了 该 头 文件 的 每 一 个 编译 单元
不仅 会 为 一个 常量 指针 常量 (const char * const) 创建 一 个独立 的 拷贝项 ，
而 且 也 会 为 那个 长长 的 字符 串 字 面 量 创建 一 个独立 的 拷贝 项 
就 相当 于在 每 一 个 编译单元 内 分 别 定 和 初 始化每 一 个 常量 ， 一次 一 个 样 
这是与 整 型 或浮点 型 常量 的 定义 不 同 〈 它 们 在初始化 完 后 不 再 需要 那个 字面
营 )。 
因 此 ， 每 一 个 编译单元 内 访问的 字符 串 常量 都 是它 自己 单独 创建 :
拷贝 衬间 的 开销 就 体现 在每 一 字符 串 字 面 量 的独立拷贝 上 ;

2.  如 果 采 用 方法 ， 在 头 文件 中 声明 所有 常量 指针 常量 而在 源 文 中 定义并初始化 它们 ， 
则 每 一 包含 头 文件 的 编译 单元 访问 的 不 仅 是 常量 指针常量 的 唯一 实体 
而 且 字 符 串 字面 常量 也 是 唯一 的实体 
这就 大 节约 了内 存 而卫 不 失效 率。
当然 我 们 完全 可 以 把 常量 合并 的 优化 交 给 编译 和 连接 器 来 完成 但 是 我 们

还 是 提倡 由 自己 来 优化 常量 的 定义 。

## 3. 函数
对 于** 静态 链接 库** 的 函数 库 或 类 库 如 果 你 调用 了 其 中 的 函数 《无 论 是 直接 调用
还 是 间接 调用 )， 
那 么 连接 器 从 相应 的 中 提取 这 些 函数 的 实现 代码 并 把它们 连接到 你 的 用 程序 ;
如 果 你 没有 调用 库 中 的 某 些 函数 则 连接 器 是不 会把 它们 的 实现代 码 连接 进来 的 即 使 该 包含 了 成 上 万 个 函数


如 果 你 用 的 是 动态 链接(DLL)， 则 运行 时 必须 将 有 DLL 都 找 到 运行 环境 的 相应 目录 下

如 果 程 中 任何 地 方 都 没有 调用 你 自己 编写 的 某 个 函数 的 话 
编译器也不 会 为 函数 生成 可 执行 代码


### 函数原型 定义

早先 C 语言 存在 函数 **前 置 声明** 的 概念 
因 为 C 语言 环境 中 同 一 作用 域中 不 能 出 现 同名 的 全 局 函数

有 了 函数 前 声明 ， 即 把 函数 的 定义 体 放 在 函数 调用 后 面 的 任何 地 方 也 无 ，连接 器在 连接 时 能 够 找到 。

是 函数 前 置 声明 并 没有 给 出 函数 可 接受 的 参数 类 型和 个 数 
于 是 编译 器 无 法对 函数 调用 语句 执行静态 类 型 安全 检查
〈 即 检查 实 参与参 的 个 数 类 型 及 顺序 等 是 呈 配 )。

导致 正确 的 参数 传递 从 而 出 现 运行时 错误 甚至 破坏 堆栈

解决 这 一问题 的 方法 就 是使 用 函数 原型 (ANSIISO C 从 C++ 借鉴 了 函数 原型 )。

{ 作 用 域 [函数 的 连接规范 ] 返回 类 [函数 的 调用 规范 ] 函数 (类 1 [ 形 参 1]，类型 2 [形参 2], ...);

函数调用 中 参数 传递 本 质 就 用 实参 来 **初始 化 形 参** 而 不 是 替换 形参
```C++
void f(int n, char *p); dni p 是 形参
template<class T>class C{...};
int main(void)
{
	char *q = “abcd”;
	f(5, q); /5 和 q 是 实参
	C<int> a; /int 是实 参
	return 0;
}
```

### 函数堆栈

函数 堆栈 实际 上 使 用 的 是 程序 的 堆栈 内 存 空间 ， 虽 然 程序 的 堆栈 段 是 系统程序 分 配 的 一 种 静态 数据 ， 但 是 函数 堆栈 却 是 在 调用 到 它 的 时 候 才 动 态 分 配 

函数 堆栈 却 是 在 调用 到 它 的 时 候 才 动 态 分 配


![](ebooks-copy/desktop/第5篇_C_C_%20内存布局与程序栈%20-%20知乎%20(2023_6_1%2002_50_50).html)


[C/C++ 修道院 - 知乎 (zhihu.com)](https://www.zhihu.com/column/c_1277937360727257088)

![](ebooks-copy/desktop/浅谈程序的内存布局%20(2023_6_1%2002_57_45).html)

![](高质量C++读书笔记/Pasted%20image%2020230601024956.png)

**栈段：**

　　1. 为函数内部的局部变量提供存储空间。
　　2. 进行函数调用时，存储“过程活动记录”。
　　3. 用作暂时存储区。如计算一个很长的算术表达式时，可以将部分计算结果压入堆栈。

**堆：**

　　堆能够根据需要自动增长。堆区域是用来动态分配的内存空间，用 malloc 函数申请的，用free函数释放。calloc、realloc和malloc类似：前者返回指针的之前把分配好的内存内容都清空为零；后者改变一个指针所指向的内存块的大小，可以扩大和缩小，它经常把内存拷贝到别的地方然后将新地址返回。

**数据段（静态存储区）：**

　　包括BSS段（Block Started by Symbol）的数据段。BSS段存储未初始化或初始化为0的全局变量、静态变量，具体体现为一个占位符，并不给该段的数据分配空间，只是记录数据所需空间的大小。数据段存储经过初始化的全局和静态变量。

**代码段：**

　　又称为文本段。存储可执行文件的指令；也有可能包含一些只读的常数变量，例如字符串常量等。

　　.**rodata段：**存放只读数据，比如printf语句中的格式字符串和开关语句的跳转表。也就是你所说的常量区。例如，全局作用域中的 const int ival = 10，ival存放在.rodata段；再如，函数局部作用域中的printf("Hello world %d\n", c);语句中的格式字符串"Hello world %d\n"，也存放在.rodata段。

　　但是注意并不是所有的常量都是放在常量数据段的，其特殊情况如下：

　　1）有些立即数与指令编译在一起直接放在代码段。

　　2）对于字符串常量，编译器会去掉重复的常量，让程序的每个字符串常量只有一份。

　　3）用数组初始化的字符串常量是没有放入常量区的。

　　4）用const修饰的全局变量是放入常量区的，但是使用const修饰的局部变量只是设置为只读起到防止修改的效果，没有放入常量区。  
　　5）有些系统中rodata段是多个进程共享的，目的是为了提高空间的利用率。

　　注意：程序加载运行时，.rodata段和.text段通常合并到一个Segment（Text Segment）中，操作系统将这个Segment的页面只读保护起来，防止意外的改写

### 函数调用规范  调用约定 〈CallingConvention)。

函 数 调用 规范 决定 了 函数 调用 的 **实 参 压 栈 退 栈 及 堆栈 释放 的 方式 **


 Windows 环境 下 常用 的 调用 规范 :
(1) `_cdecl`: 这 是 CHHC 函数 的 默认 调用 规范 ,参数 从右 向 左 依次 传递 并 压 入
堆栈 由 调用 函数 负责 堆栈 的 清 退 

(2) `_stdcall`， 这 是 Win API 函数使 用 的 调用 规范 。 参 数 从右 向 左 依次传递 并 压 入 堆栈 外 被 调用 函数 负责 摊栈 的 清 退
该 规范 生成 的 函数 代码“cdecl 更 小， 但 当 函 有 可 变个 数 的 参数 时 会 转 为 `_cdecl` 规范
(3) `_thiscall`: 是 C++ 非 静态 员 函 数 的 默认 调用 规范 不 能 用 个 数可 变 参数
(4) `_ fastcall`: 该规范 所 修饰 的 函数 的 实 参 将 直接 传递 到 CPU 寄存 器 中 而 不
是 内 存 堆栈 不能 用 于 成 员 函 数

凡是 接口 函数 必须 显 式 地 指定 其 调用 规范 除 接口 函数 类 的 非态 员 函 non-static member function)。

### 函数的连接规范

只 要 它们使 用 一 的 员 对 齐 方式 和 布局 方案 、 一 的 函数 调用 规范 、 一 致 virtual function 实现 方式 总 之就 是 一 致 C++ 对 象 模型

### 参数传递规则

C 语言 , 函数 的 参数 和 返回 的传递式 有 两 : 值传递 (pass by value) 和 地 址传递 〈 即 指针 传递 ，pass by pointer)

C++语言 中 增加 了 引用 传递 〈pass by reference)。 

如 果 函 数 有 参数 那使 void 而 不 要 空 着 
这 是 因为 标准 C 把 的 参数 列表 解释 为 可 以 接受任何 类 和 个 数 的 参数
而 标准 C++ 则 把 空 的 参数 列表 解释 为 可 以 接受任何 参数 
在 移植 C+HC 程序 时 尤其 要 注意 这 方面 的 不 同
```C++
void SetValue(int width, int height); / 良好 的 风格
void SetValue(int, int); / 不 良 风格
float GetValue(void); / 良好 的 风格
float GetValue(); / 不 良 的 风格
```

![](高质量C++读书笔记/Pasted%20image%2020230601050908.png)

![](高质量C++读书笔记/Pasted%20image%2020230601051429.png)

```C++
String operator + (const String &lh, const String &rh)
| {
	| String temp;
	temp.m_data = new char[strljen(s1.m_data) + strlen(s2.m_data) + 1]];
	strcpy(temp.m_data, s1.m_data);
	strcat(temp.m_data, s2.m_data);
	retumtemp; /执行 string 对 象 及 其 字符 串 内 容 的 拷贝
}
```
对 于相 加 函数 应 当 用 “返回 对 象 值 的 方式 返回 String 对 象 这 将把 局 部 对 temp 及 其 真正 的 字符 串 值 拷贝 一 给 调用 环境 的 接收 者 如 果 改 “返回 引用 ” 那 么 函数 返回值 是一 个指向 局 部 对 ttmp 的 “引用 ”( 即 地 址 )， 而 temp 在函数 结束 时 被 自动 销毁 将 导致 返回 “引用 ”无效


```C++
char * Func(void) " 
{
	char str[] = "hello world"; /str 数组 创建 在 函数 堆栈 上 ， 并 用 字符 串 / 常量 来 初始 化 在 末尾 自动 添加 ^0” 
	cout << sizeof(str) << endl; 
	HA2 cout << strlen(str) << end]; 
	#11 return str; / 该 语句 存在 隐患 ，str 指向 的 内 存单 元 将 被 释放 } 但 下 程序 则 是 正确 
}

const char * Func(void)
{
	const char *p = "hello world"; / 字符 串 常量 存放 在 程序 的 静态 数据 区
	/ 来 尾 自动 添加 \0'
	cout << sizeof(p) << endl; HA
	cout << strlen(p) << endl; HWA
	return p; / 返回 字符 串常量 的 地 址
}
```

![](高质量C++读书笔记/Pasted%20image%2020230601053851.png)
“创建 一个 临时 对 并 返回 它 ” 的 过程 是 不 同 的，**编 译 器**可 以 直接 把 临时 对 象 创 建 并初始 化 **外 部 存储 单元** 中 省 了 拷贝 和 析 构 开销 提 高 了 效率 

### 存储类型以及作用域规则

### 存储类型
extern 、auto 、static、register，

extern 和 static 用 来标识永久 生存 期 限 的 变量 和 函数 
而 auto 和 register 则 用 来 标识 临时 生存 期 限 的 变量

全 局 变量 和 全 局 函数 的 存储 类 型 extern 能 够 定义 在 它们之 后 的 同一个 编译 单元 内 的 函数 所 调用
变 量和 函数 显 式 地 加 上 exterm 声明 ，那么 其他 编译 单元 中 的 函数 也 能 调用 它们

显式生命为static的全局变量和全局函数具有static存储类型 只能被同一个编译单元内的函数调用

局 部 变量 默认 具有 auto 存储 类 型 除 非 static 或 register 来 定义
它们 的作用 域 是程序 块作用 域 连 接 类 型 都 内 连接 在 进入 函数 的 时候 创建 ，在 函数退出 的时 候 销毁
register 和 auto 只 能 用 于 声明 局 部 变量 和 局 部 常量 

全局常量默认 存储类型 static 
除非在定义了它的编译单元之外的其他编译单元中 显式的用extern声明 否则不能被访问
（就是拿过来用的时候 加个extern 声明一下）

局 部 符号 常量 (注意 不是 函数 内 出 现 的 字面 常量 ) 的 默认 存储 类 型 auto，
除非 显式 地 定义 static 或 register.

把 那些 经 常 用 的 变量 例如 循环 计数 器 直接 放 到 CPU 寄存 器 ， 可 以避免 在 寄存 和 内 存 之 间 频 地 交换 数据 因 此 能 提高 程序 的 运行 效率 

### 作用域规则

在 标准 C 语言 ， 这 些 范围包括 文件 函 数 程 序 和 函数 原型

标准 C++中 除了 这 4 种 外 有 两 作用 域 类 型 类 和 名 字 空 间 其 中 名 字 空 间 是 可 以 跨 文 件 +

S (label) 是 具有 函数 作用 域 的 惟一 一 种 标识 符 这 就 是 说 不 论 你 的 标号定义 在 函数 中 的 哪 一行 ， 也 不 论 定义 在 函数 内 撕套得 多 么 深 的 程序 块 内 它 都能够在 函数 体内 任何 一 个地 方 访问 , 因此 也 叫做 函数 级 的 标识 。 标号 一 般 用 在 goto| 语句 ， 如 果 goto 语句 没有 使 用 到 该 标号 那 么 该 标号 将 忽略

即 使部 变量 的 存储 类 型 声明 static， 它 仍然 具有 程序 作 用 

在 函数 中 毁套 的 程序 块 可 以 定义相同 名 字 的 变量 在 内 层 的 变量 玫 蔽外 层 的 同名 变量 
当局 部 变量 与 某一 个全 局 变量 同名 ， 在 函数 内 部将史 蔽 全 局 变量 
如 果 某 一 个 员 函 数 内 定义 了 与 类 的 某 一 个 数据员 同 名 的 局部 变量 ， 那么 这个 局 部 变量 将 遮蔽 该 同名 数据 成 。


如果 一 个 标识 符 能 够 在 其他 编译 单元 中 或 者 在 定义 它 的 编译单元 中 的 其 他 范围内 被 调用 ， 那 么 它 就 外 连接 的 外 连接 的 标识 符 需要 分 配 运行 时 的 存储 空间

如 果 一 个标识符 能 在 定义 它 的 编译单元 中 的 其他 范围 内 被 调用 但 是 不 能 在他 编译 单元 中 被 调用 那 么 它 就 内 连接

一个仅 能 够 在 声明 它 的 范围 内 被 调用 的名 字 是 无 连接 
```C++
void f0
{
int a; /a 是 连接 的
class B{ ... }; / 局 部 类 是 连接 的 具 有 程序 作用 域
}
```

![](高质量C++读书笔记/Pasted%20image%2020230601062801.png)

![](高质量C++读书笔记/Pasted%20image%2020230601062725.png)


对 ADT/UDT 的 输入 参数 应该 “ 值 传递 改 “const 公 传 递 "， 目的 提高 效率 例 如 ， 将 void Func(Aa) 7H void Func(const A &a).
对 于 基本 数据 类 型 的 输入 参数 不 要 “ 值 传递 ”的 方式 改 “const 信传递 "， 否 则 既 达 不 到 提高 效率 的 目的 又让 人 费解
例如， 不 要 
void Func(const int x) 改 void Func(const int 人 x)。

### const
```C++

void Func(constA &a)
void OutputString(const char * const pStr);
void StringCopy(char *strDest, const char *strSrc);
const char *str = GetString();

```

## 4. 指针 数组 字符串

int* a,b,c; 《=》int `*a`, b,c;

取 地 址 〈&) 和 反 引 用 〈(`*`)。

准确的说，与 编译器的目标平台 有关。 
如果目标平台是32位的，那么sizeof (void*)就是**4**，
如果是64位的，那么sizeof就是8，
如果是16位的，就是2。 sizeof (void*)的含义就是获取一个指针的大小。 
指针的本质就是内存地址，因此指针的大小和内存空间有关。 
32位的机器内存空间是2G（windows系统），
因此指针的大小必须是 log_2 (2times 1024^3) = 31 ，凑个整数那就是32bit。

### 数组
在 语义 上 ， 下 标 操作 符返 回 的 是一 个 元 素 的 引用 。a[3] = 100;
编译器 计算地 址 a + 3 * sizeof (inb， 得 到 0x004284FC， 并 返回 地 址 所 指 对的 引用 而不是 返回 “45” 这 个
a[3] = 100; / 转换 为 *a+ 3) = 100;
cout << a[3] << endl， / 转换 cout << *(a + 3) << endj;

组 名 字本 身 是 一 个 指针 ， 是 一 个 指针 常量 即 a 等价 int * const a， 
因 此你 不 能 试图 修改 数组 名 的 值 

a == &a[0]

int a[10] = {0};
int b[10] ={1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
a=b; /不仅语义 不 对 何 a 也 不 能 被修改

int b[100]; / sizeoftb) = 400 bytes， 未初始
int c[ ]= {1, 2,3, 4,5}; I FBP RA S, sizeof(c) =20 bytes, 初 始
int d[5] = {1, 2, 3, 4,5, 6, 7}; / 错误 ! 初始 值 越界
int e[10] = {5, 6, 7, 8, 9}; / 元 素 个 数 10， 指 了 前 5 个 元 素 的初始 值 剩
VW 下 的 元 素全 部 自动 初始 化 0
int [10] = {5,, 12,,2}; / 错误 ! 不 能 跳 过 中 间 的 某些 元 素

C++/C 可 以 在 运行 时 进行 数组 的 越界 访问 检查 
这 是 因为 数组 大小 的 信息 保存 在程序 中 的 某 地 方 一 般 
就 是 放 在 数组 第 一 个元 素 位 置 的 **前 面** 占用 一 int 变量 的 字节数 , 
它 地 址 a-sizeof(int)
```C++
int a[10] <> int* const a;
int b[3][4] <> int (* const b)[4];
int c[3][4][5] <> int (* const c)[4][5]:
```

数组 是 不 能 从 函数 return 语句 返回 
数组 可 以作为 函数 的 参数
```C++
void output(const int a[ ], int Size)
{
	cout << sizeof(a)<<endl; // 是 4 而 不 400
	for(int i = 0; i < size; i++)
	cout << a[i] << endl;
}
// main
int x[100] = { 0};
cout << sizeof(x) << endl; // 400
output( x, 100 );

void output(const int a[ ][20], int line)
{
	cout << sizeof(a) << endl; 4
	for( int i = 0; i < line; i++ ) {
		for( int j = 0; | < 20; j++) {
			cout << a[i][j] << endl;
		}
	}
}
// main
int x[10][20] = { { 100 } };
cout << sizeof(x) << endl; // 800
output( x, 10 );

void output(const int a[ ][20], int line)
cout << sizeof(a) << endl; 4
	for( int i = 0; i < line; i++ ) {
		for( int j = 0; | < 20; j++) {
			cout << a[i][j] << endl; // a 是 一 个 指向 一 维 数组 的 指针a+l 就 是 指向 二 行 的 指针
		}
	}
}
// main
int x[10][20] = { { 100 } };
cout << sizeof(x) << endl; // 800
output( x, 10 );

void output(const int (*a)j[20], int line) 
 a[i][j] 访问转换为 *(a + (i * 20 + j) * sizeof(int)).

```

char *p = new char[1025]; / 分 配 空间 
delete []p; / 删除 数组 空间

它 不 会删除 不 属于 它 内 存单 元 也不 会 泄漏 哪怕 是 一 字 节 的 内 存单 元 
因为你 明白 地 告诉 了 编译 : 这 是 在 释放 一 个 字符 数组 ,
“ 请 ” 它 去 p 指向 的 字符 数组 大小 信息 《数组 的 元 素 个 数 它 被 编译 器 保存 在 程序 的 某 个 地 方 )， 
然 后 按照 这 个 大 小 来 释放 动态 内 存 这 就 是 数组 的 释放 (delete[]) 的语义 

```C++
char (*p3)[4] = new char[5][4]; /OK! 退化 第 一 维 语 义 等
int (*p4)[5] = new int[3][5]; /OK! 退化 第 一 维 语 义 等
char (*p5)[5][7] = new char[20][5][7]; OK! 退化 第 一 维 语 义 等 价

delete [jp3; // 删除 p3
delete []p4; // 删除 p4
delete []p5; // 删除 p5
```

```C++
char arrChar_1[ ] = {'a’,’b',\0','d',’e"};
char arrChar_2[ ] = "hello";
char *p = “hello”;
cout << sizeof(arrChar_1) << endl; /S$， 表 示该 数组 $ 字 节
cout << strlen(arrChar 1) << endl; 2， 表示 字符 串 长 度 2
cout << sizeof(arrChar_2) << endl; /6， 表 示 数 组 6 字节
cout << strlen(arrChar_2) << endl; WS, RARER BREA 5
cout << sizeof(p) << endl; / 4， 表示 指针 p 占 4 字 节
cout << strlen(p) << endl; /5， 表 示字符 串 长 度 $

默认 char * 表 示 字 符 。 例 如 :
char ch ='a’; / 用 字符 'a来 初始 化 字符变量 ch
char *pChar = &ch; / 字符 指针 指向 字符变量
cout << pChar << endi; / 错 把 字符 指针 当做 字符 串
正确 用 法 :
cout << *pChar << endl; / 取 一 个 字符
```

某 些 字符 串 函 数 并 不 会 自动 目标 字符 串 结 尾 追 加 '\0'， 
例 如 strncpy和 strncat， 除 非你指定 于 值 比 源 串 的 长 度 大1
strcpy 和 strcat 会 把 源串 的 结束 符 一 并 拷贝 到 目标串 中 

```C++
typedef int _cdecl (* FuncPtr)( const char*); / 定义 一 种 函数 指针 类
FuncPtr fp_1= strlen ;
FuncPtr fp_2 = puts ;
double _cdecl (*fp_3)( double ) = sqrt ;

int len = fp_1(‘I am a software engineer.”) ;
double d = ( *fp_3 )( 10.25);

double _cdecl (* fp[5])( double ) = { sqrt, fabs, cos, sin, exp };
for (intk = 0;k <5; k++)
{
	cout << "Result :" << fp[k]( 10.25 )<< endl ;
}

```

一般 的 函数 调用 语句 可 以 在 编译 时 就 完成 这 个 绑 定《叫做 静态 决议 或 静态 连接 )
运行 时 连接

类 的 员 函 有 4 种 类 型 inline、virtual、static、normal。

inline 函数 在 运行 时会 展开 ， 虽 然 语 言 允许 取 其 地 址 但 是 没有 太 大 意义 。virtual 成 员 函 数 的 地 址 指 的其 vtable 中 的 位 置 ，
static成员 函 数 的 地 和 普通 全 局 函数 的 地 址 没有 任何 区 别 ;
普通 成 员 函 数 的 地 和 一 般 函 数 的 地 址 也 没有 区 别 
就 是 函数 代码 在 内 存 中 的 真实地址， 
但 是 由 于它 的 调用 要绑 定到 一个 实 实在 在 的 对 象 ， 
因 此无 论 是其 函数 指针的 声明 方式 还 是 其 地 址 的 获取 方法 都 比较 特别

```C++
示例 7-14。
class CTest {
public:
	void f( void ) { cout << "CTest::f0" << endl; } / 普通 员 函
	static void g( void ) { cout << "CTest::2()" << endl; } // HAR RA
	virtual void h( void ) { cout << "CTest::hQ)" << endl; } /W 虚拟 员 函
	//...
private:
	///...
};
void main()
{
	typedef void (*GFPtr)( void ); / 定义 一 个全局 函数 指针 类
	GFPtr fp = CTest::g ; 
										
	fp();                
	typedef void (CTest::*MemFuncPtr)( void); // 声明 类 成 员 函 数 指针 类
	MemFuncPtr mfp_] = &CTest::f; // 声明 成 员 函 数 指 针 变 并 初始
	MemFuncPtr mfp2 = &CTest::h; / /注意 获取 成 员 函 数 地 址 的 方法
	CTest theObj ;
	(theObj .*mfp_1)() ; / 使 用 对 和 成 员 函 数 指 针 调 用 成 员 函
	(theObj .*mfp_2)() ;
	CTest *pTest = &theObj ;
	(pTest->*mfp_1)(); / 使 用 对象 指针 和 成 员 函 数 指针 调用 成 员 函
	(pTest->*mfp_2)() ;
}
输出 结果 
CTest::g()
CTest::f0
CTest::hO
CTest::f0
CTest::h()
```
为 了 与 静态 员 函数 区 别 
取 virtual 函数 和 普通 员 函 数 的 地 址 需要 使用 “&” 运 算 
〈 取 静态 成 员 函 数 地址 也 可 以 使 用 它 但 是 没有 必要 )。

由 于 纯 虚 函数 没有 实现 体 而 非纯 虚 函 有 实现 体
且 虚 函数 都是 通过 vptr 和 vtable 来 间接 调用 的 
因 此 取 虚 函数 的 地 址 将 得 到该虚 函 数 实现体 vtable 中 的 索引号
要 想得到 虚 函 数 实现 体 的 真实 地 址 ,还需要 首先 从 对 象入手 ，
找到 vptr 的 位 置 进 而 找到 vtable 的 所 在 
然 后 根据 函数 指针 的 大 和虚 函 数 的索引 ， 取 出 虚 函 数 的 真实 地 址 

### 引用与指针
int m;
int& n=m;
n 既 不 是m 的 拷贝 也 不 是指向 m 的指针 ， 其实n 就 m 自己 

引用 在 创建 的 同时 必须初始 化 即 引用 到 一个 有 效 对象

const int& rint = 0;

创建 一 个 临时 的 int 对象 用 0 来 初始它 然 后 再 用 它 来 初始 化 引用 rint， 
而 该 临时 对 象 将 一 直 保 到 rint 销毁 的 时 候 才 会 销毁。
所以，不 要 用 字面 常量 来 初始化引用

```C++
给 引用 赋值 并 不 是 改变 和 原始 对 象 的 绑 定 关系
int a=10,b= 1000;
int& rInt =a; /rint 引用 到 a，rInt 等 10
rInt = b; // rInt Al a 都 变 成 1000
```

引用 的 创建 和 销毁 并 不 会 调用 类 的 拷贝 构造 函数 和 析构 函 数
在 语言 层面 引 用 的 用 和对 象 一 样
在 二 进 制 层面 引 用 一 般 都 是 通过指针 来 实现 的 只 不过 编译 帮 我 们 完成 了 转换 

引 用 的 主要 用 途修饰 函数 的 形参 和 返回 值

## 5. 第八章 C++/c高级数据类型

 C 语言 stuct， 有 时 也 称 用户 自 定义 数据 类 (User defined Type, UDT)。
 抽象 数据 类 型 (Abstract Data Type, ADT)，
 在 C++ 环境 ， 我 们 把 C 风格 struct 叫做 POD (Plain Old Data) 对象

 structyclass 当做 参数 传递 给 函数 ， 默 认为 值 传 递 其 中的 数组 将 全 部 拷贝 到 函数 堆栈 
 当 你 UDTADT 中 包含 数组 员 的 时 候 最 好使 指针 或 引用传递该 类 型 对 象 
 并 且 一 定 要 防 让 数组 元 素 越界 否 则 它 覆盖 后的 结构成 员 

memset (&s, 0x00, sizeof (Student)) 
// 可 以 仅指 定 第 一成 员 的初 值 来初始 POD 对 象 后面 的 员 将 全 部 自动初始 化 为0， 就 数组 的始 化 一样
Student s = {0};

一

对 象 间 的 包含 指 一 类 型 对 象 充当 了 另 一 个 类 型 定义 的 数据 成 ，:从 而 也 就 充当 了 它 的 对 象 的 成 ， 即 两 对 象 间 存 has-a 关系
虽然 对 象 不 能 自 包含 但 可 以 自 引用 ， 而 且 两 个 类 型 可 以 交叉 引用 ， 这种 关系称 holds-a 关系
| 个 对 象 不能 自 包含 无 论 是 直接 的 还 间接 的 因 为 编译 器 无法 为 计算 sizeof 值 ， 也 就 不 知道 给这 样 对 象 分 配 多 少 存储 空间
![](高质量C++读书笔记/Pasted%20image%2020230601161745.png)

![](高质量C++读书笔记/Pasted%20image%2020230601161855.png)


而 于对 齐 〈 将 大 小 调整 到 机器 字 的 整数
) 的 考虑 每 对 象 的 存储 空间 中 可 能 会 存在 填补 字 节 ， 这 些 字 节 单元 不 会 初始
化 而 具有 上 次 用 留 下 “ 脏 值 ”( 随 机 值 )。 显 然 每 对 象填补 字节 的 内 容 是
,会 相同 的 这就是 说 如 果编译 器 支持使用 逐 位 比较 的默认 方法 来 比较同类型 对象
结果 肯 定 是 不 对

### 位域

C 语言 位 各成 员 的 类 型 必须 int、unsigned int、signed int 等 类 型 ，C++ 还 允 许 使 char、long 等 类 型
不 允许 用 指针 类 型 或 浮 点 类 型 作为 位 域 的 成 员 类 ，| 因为 它们 可 能 导致 无效 的 值 

```C++
struct DateTime
{
unsigned int year ;
unsigned int month :4;
unsigned int day  :5;
unsigned int hour  :5;
unsigned int minute : 6;
6unsigned int second : 6;
};
cout << sizeof (DateTime) << endl; //8
```
可 以 定义 非 具 名 的 位 域 成 ， 其 作用 是 相当 于 占 位 符 可 用 来 隔离 两 相 邻 的
位 域 成 。 如 示例 8-7， 由 于 第 二 个 位 域 员 没有 名 字 ， 因 此 不 能 直接访问 它 所 在 的位

```C+
struct DateTime
{
	//...
	unsigned int day :5;
	unsigned int :2
	unsigned int hour :5;
	//...
};
```

可 以 定义 长 度 0 的 位 域 成 ， 其作用是 迫使 下一 个 员 从 下 一 个 完整 机 器 字 〈Word) 开始 分 配 空间
```C++
struct DateTime
{ 
	//...
	unsigned int day 25;
	unsigned int :0;
	unsigned int hour 255
	//...
};
cout << sizeof(DateTime) << endl; // 12
```

即使位 域 有 成 员 的位 数总 和 达 不 到 整 字 节 的倍数 位 对 象 也会 对 齐 到 机 器 字 长
不 能使 用 访问 数组 元 素 的 方法(`[]`) 来访问位 域 员 的 单个 位 
如 果 有 这 种 需要 ， 请使 用 位运算 (~、区 、|、>>、<<、^ 及 其 = 的组 合 运算 ) 或 者使 `std::bitset<N>。`

### 成员对齐

对 于 复合 类 〈 一 般 指 结构 和 类 ) 的 对 象 如 果 它 的 起 始 地 址 能 够 满足 其 中 要求 最 严格 〈 或 最 高 的 那个 数据 员 的 自然 对齐 要 求 那 么 它 就 是 自然 对 齐 的
如果 那 个 数据 员 又 是一 个 复合 类 型 对 象 则 依次 类 推 直 到 最 后 都 基本 类 型数据 成 员 

在 C++/C 的 基本数据 类 型 中 如 果 不 考虑 enum 可 能 的 大 值 所 需 的 内 存 字节 数 ，double 就 对 齐 要 求最 严格 的 类 型 ， 其 次 int 和 float， 然 后 short、bool 和 char。

直接 依照 声明 顺序 来 存放 ， 即 复合 类 型 中 存在 多 个 访问 〈 即 C++ 类 中 的 每public. private 和 protected 访问 限定 )， 至 少 也 会 保证 每 个 内 的 有 数据 员 是| 按照 声明 顺序 来 存放 

至 于 先 声明 的 员 会 放 在 地 址 还 是 低地 址 处 完 全 是由 编译 器 实现 来 决定 的 而且 -- 般 都 采用 “按照 声明 的 先后 顺序 从 低地 址 到 高 地址 依次 放 各 个 成 ” 的 方案

为了对象数组 要实现自然对齐

```C++
typedef unsigned char BYTE;
enum Color{ RED = 0x01, BLUE, GREEN, YELLOW, BLACK};
struct Sedan // 私家 车
{
	bool m_hasSkylight; RRARS
	Color m_color; / 颜色
	beol m_isAutoShift; / 是 否 是 自动 档
	double m_price; / 价格 〈 元 )
	BYTE m_seatNum， / 座位数量
}
```

![](高质量C++读书笔记/Pasted%20image%2020230601165034.png)

![](高质量C++读书笔记/Pasted%20image%2020230601165745.png)


使 offsetof 宏 这 个 宏 专 门 用 来计算 数据 员 相 对 于 对 象 起始地址 的 真实 含 移 量 它 会 把 有 隐 含 员 也 计算 进去 比 如庶 函 表 指针vptr。 
```c++
std::cout << “offsetof(Sedan, m_hasSkylight) =” << offsetof(Sedan, m_hasSkylight) ;
std::cout << “offsetof(Sedan, m_color) = ” << offsetof(Sedan, m_color) ;
std::cout << “offsetof(Sedan, m_isAutoShift) = ” << offsetof(Sedan, m_isAutoShift) ;
std::cout << “offsetof(Sedan, m_price) =” <<< offsetof(Sedan, m_price) ;
std::cout << “offsetof(Sedan, m_seatNum) = ” << offsetof(Sedan, m_seatNum) ;
```

编 译器 不 会 随便 地 在 任意 一 个 逻辑 内 存 地址 上来 创建 C++/C的 变量 和 对 象 
它们 在内 存 中 的 起 地 址 需要 满足 一 定 的 条 件 数 据 成 员 也 并 一 定 是 挨 在一起 的 
而且每个 数据 员 的 地 址 也 不是 随便 安排 的 都 需要 经 过 编译器的 精心 规划 和 计算 
这 才能提高 对 及 其 员 的 访问 效率
```C++
Sedan s;
std::cout << “Address of s =” << (void* )&s ;
std::cout << “offset of m_hasSkylight =” << ((char*)&s.m_hasSkylight — (char*)&s) ;
std::cout << “offset of m_color = ” << ((char*)&s.m_color — (char*)&s) ;
std::cout << “offset of m_isAutoShift =” << ((char*)&s.m_isAutoShift — (char*)&s) ;
std::cout << “offset of m_price =” << ((char*)&s.m_price — (char*)&s);
std::cout << “offset of m_seatNum = ” << ((char*)&s.m_seatNum — (char*)&s);
```

综上 所 述 类 的 数据 成 员 类 型 的 选择 声 明 顺 序 即 排列 采 用 的 员 对 齐 方式
都 将影响对 象 的 实际 大小 和 访问 效率 

### 联合 union

联合 的 另 一 个 妙用 就 用 来 解析 一 个 寄存 器 或 多 字 节 内 存 变量 的 高 字 节 的 值
而 不 需要 我 们 手工 用 位 运算 符 来 解析 它们 。 
```C++
union KeyCode
{
	short keyNum ;
	char byteArr[2] ;
}
```

### 枚举

枚 举 类 型还 可 以 是 匿名

### 文件

文件 控制 块 (FCB) 的 数组

## 6. 第九章 编译预处理


```
(1) #include < >
(2) #Hinclude " "
```
第 一 种 形式 一 般 用 来 包含 开发 环境 提供 的 库 头 文件 它 指示 编译 预 处 理 器 开发 环境 设 定 的 搜索 路 中 查找 所 需 的 头 文件 
第 二 种 形式 一 般 用 来 包含 自己 编写 的头 文件 它 指示 编译 预处 理 器 首先 在 当前 工作 目录 下 搜索 头 文件 如 果 找 不 到 的 话再 到 开发 环境 设 定 的 路 径 中 去 查找 。


内 部 包含  卫 哨
 为了 避免 同一 个 编译 单元 包含 同一 个 头 文件 内 容 超 过 一次

```C++
// stddef.h
#ifndef_STDDEF H INCLUDED_
#define STDDEF_H INCLUDED _
					/ / 头 文件 内 容
#endif    /!STDDEF H_INCLUDED

// XXX.cpp
#include “stddef.h”
#include “stddef.h” /! No problem!

```

外 部 包含 卫  哨

当 一 个 头 文件 被 一 个源 文件 反 复 包 含 多 次 避免 多 次 查找 和 打开 头 文件 的 操作
```C++
#if !defined ( INCLUDED_STDDEF_H_)
#include <stddef.h>
#define _INCLUDED_STDDEF_H_
#endif //!_ INCLUDED _STDDEF_H_
```

在 头 文件 中 :
(1) 包含 当前 工程 中 所 需要 的 自 定义 头 文件 〈 顺 序 自 );
(2) 包含 第 三 方程 序库 的 头 文件 ;
(3) 包含标准 头 文件 。
在 源 文 中 :
(1) 包含 源 文件 对 应 的 头 文件 《如 果 存 );
(2) 包含 当前 工程 中 所 需要 的 自 定义 头 文件 ;
(3) 包含 第 三 方程 序 库 的 头 文件 ;
(4) 包含 标准 头 文件 


宏 定义 具有 文件作用

宏定义 `#define` 关键 字 后 出现 的 第一 个 连续字符 序列 作为 宏 名 
剩 下 的 部 作为 宏体

宏 不 会 进入 符号 
即 宏替换 后 出 了 语法 错误 编 译 器 也 会 将 错误 
定位 到 源 程序中 
而 不 是 定位 到 具体的 某个 宏 定义 

定义 带 参数 的 宏 ， 宏 和 左 括号 之 间 不 能 出 现 空格 否 则 使 用 时 会 出问题 
但 是 编译 器 不 检查 出 这 种 错误

```c++
define OUTPUT(word) cout << #word << endl
OUTPUTI( like swimming very much.);
cout << "I like swimming very much." << endl;

#define TEXT (str)#str
cout << TEXT(Hello World);
(str)#str(Hello World);

#define PI 3.14
#define PL 2 (2 * PI)

带 参数 的 宏 体 和 各个形 参 应 该 分 别 用 括号 括 起 来
#define SQUARE(x)((x) * (x))

不 要 在 引用 宏 定义 的 参数 列表 中 使 用 增 和 减 量 运算 符 否则 将 导致 变量的 多 次 求值
int n=5;
int x = SQUARE(n++};
int x = ((n++) * (n++));

其结果 将 30 而 不 是 期 望 25
```

用 宏 来 构造 一 些 重复 、 数据 和 函数 混合 的 功 能 较 特 殊 的代码 段 时候 其 优点就显示 出 来 。
```C++
#define DECLARE DYNAMIC(class_name) \
protected: \
static CRuntimeClass* PASCAL _GetBaseClass(); \
public: \
static const AFX_DATA CRuntimeClass class##class_name; \
virtual CRuntimeClass* GetRuntimeClass() const;
#define DECLARE _DYNCREATE(class_name) \
DECLARE_DYNAMIC(class_name) \
static CObject* PASCAL CreateObject();

#undef TEXT
```

不 要 使 用 宏 来 定义 新 类 型 名 应 该 使 typedef， 否则 容易 造成错误 
给 宏 加 注释 时 请 使 块 注释 〈`/* */`)， 而 要 使 用 行 注释 

对 于 较 长 的 用 频率 较高 的 重复 代码 片段 建 议 用 函数 或 模板
对 于 较 短 的 重复代码 片段 可 以 用 带 参数 的 宏 定义

### 条件编译
```
#if 0
	… / 希望 禁止 编译 的代码
	... / 希望 禁止 编译 的 代码
#endif
```

编译 伪 指 `#error` 用 于 输出 与 平台 、 环 境 有 关 的 信息

```C++
#if !defined(WIN32)
#error ERROR: Only Win32 platform supported!
#endif
#ifndef cplusplus
#error MFC requires C++ compilation (use a .cpp suffix)
#endif
```
编译伪指令#pragma 用于执行语言实现所定义的动作
```C++
pragma pack(push, 8) /* 对 象 员 对 齐 字 节数 所
#pragma pack(pop)
#pragma warning(disable:4069)  /*不要产生C4069警告 */
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
```

构 串 操作符 # 只 能修饰 带 参数 的 宏 的 形 参 它 将 实 参 的 字符 序列 
转换 字符 串 常量
而 不 是 实 参代表 的值

```C++
#define STRING(x) #x #x #x
#define TEXT(x) "class" #x "Info"
```

合并 操作符 ## 将 出现在 其 左右 的 字符 序列 合并 成 个新 的标识
```C++
#define CLASS NAME(name) class##name
#define MERGE(x, y) x##y#x

classSysTimer
meTome
```
![](高质量C++读书笔记/Pasted%20image%2020230602074658.png)


```C++
double * const pDouble = new(nothrow) double[ 10000000};
if( pDouble == NULL ) {
cerr << "allocate memory failed on line"<<(_ LINE -2)
<< "in file" << FILE <<endl;
}
```

### C++文件结构 程序板式

 头 文件 的 有 内 容 最 终都 会 被 合并 到 某 一 个 或 几 个 源 文件 ，
 如 此 将 一 个 包含 的 头 文件 递归 地 展开 后 形成 的 源 文件 叫 编译单元
 ```C++
 char *name;
int *x，y; / 此 y 不 会 误解 为 指针

typedef int* PINT;
typedef int& RINT;
PINT pl, p2; / 两 个 指针
RINT ril=i r2=j; /两 引用
```
### C++命名规则
## 7. C++面向对象程序设计方法

```C++
继承
class A {
public:
void Funcl(void);
void Func2(void);
};
class B : public A {
public:
void Func3(void);
void Func4(void);
};
main()
{
B b;
b.Funcl(); 
b.Func2();
b.Func3();
b.Func4();
}
```

C++ 虚函数，抽象基类， 动态绑定 (Dynamic binding) 多态 (Polymorphism)
构成 了 出 色 的 动态 特性

为 了 使 这 种 行为 可 行 我 们 把基类 Shape 中 的 函数 Draw0O 声 明 为 虚 函 数
然 后在 派生 类 中 重新定义 Draw() 
使 之绘制 正确 的 形状 这 种 方法叫覆盖 〈Override)。 
```c++
class Shape {
public:
virtual void Draw(void); // Draw() 
}
class Rectangle : public Shape {
public:
virtual void Draw(void); / /Draw0O 为 虚 函数 不写virtual 也会变成虚函数
}
```

抽 象 类 的唯 -- 目 的 是 让 其派生 类 继承 并 实现 它 的 接口 方法 (Method)， 
因 此 它 通 也 被 称 为抽象 基 类 〈Abstract Base Class) 

如 果 将 基 类 的 虚 函 数 声明 为 纯虚 函数 那 么 类 就 定义 了 抽象 基类 。
```C++
class Shape { // Shape 是 抽象 基类
ublic:
virtual void Draw(void) = 0; / Draw(0 为 纯 虚 函数
}
```

将一 个 函数初始 化 0 意味 着 函数 地 址 将为 0， 
这就是 在 告诉 编译 器 不 要 为 函数 编 址 从 而阻止 类 的 实例 化行为 
C++ 中 只 有虚 函 数 才可 以 被 初始 化 0

一般 的 信息 隐藏 是 把类 的 有 数据 员 声 明 private 或 protected 的 
并 提供 相应 get set 函数 来 访问 对象 的 数据

```C++
class [Rectangle {
public:
virtual ~IRectangle() {}
virtual float GetLength() const = 0 
virtual void SetLength( float newLength) = 0 ;
virtual float GetWidthQ const = 0 ;
virtual void SetWidth(float new Width) = 0 ;
virtual RGB GetColor() const = 0 ; // RGB : unsigned long
virtual void SetColor(RGB newColor) = 0 ;
virtual float CalculateArea() const = 0 ;
virtual void DrawQ) = 0;
static IRectangle* _stdcall CreateRectangleQ; /入 口 函数
void Destroy(){ delete this; }

class RectangleImpl : public [Rectangle {
public:
RectangleImplQ : m_length(1), m_width(1), m_color(OxOOFFEC4D){}
virtual ~RectangleImpl() {}
virtual float GetLength( const { return m_length; }
virtual void SetLength(float newLength) { m_length = newLength; }
virtual float GetWidth() const { return m_width; }
virtual void SetWidth(float newWidth) { m_width = newWidth; }
virtual RGB GetColor() const { return m_color; }
virtual void SetColor(RGB newColor) { m_color = newColor; }
virtual float CalculateArea() const { return m_length * m_width; }
virtual void +Draw() { cout << "RectangleImpl::Draw(Q)" << endl; }
private:
	float m_length;
	float m_width;
	RGB mcolor;
};
	IRectangle* stdcall IRectangle::CreateRectangleO
	{Return new(nothrow) RectangleImpl;}
	```

每 一 个 具有 虚 函 数 的 类 都 叫做 多 态 类， 
这 个 虚 函 数 或者是 从基 类 继承 来 或者是 自己 新 增加
C++ 编译 器 必须 为 每 一 多 态 类 至 少 创建 一 个 虚 函 数 表 (vtable )，
它 实 就 是一 个 函数 指针 数组 
其 中 存放 着这 个类 所 有 的 虚 函 数 的 地 址 及 该 类 的 类、型信息 
其 中 也 包括 那些 继承但 未 改写 《Overrides) 的 虚 函 数

每一 个 多 态 对有 一 个 隐 含 的 指针 成 ， 它 指向 所 属 类 型 vtable， 这就 vptr。

```C++
(*(p->_vptr[slotNum]))(p, arg-list); // 指针 当 作 数组 来 用 最 后 改写 为 指针 运算
```

派生 类 定义 中 的 名 字 〈 对 象 函数 ) 将 义无反顾 地 遮 项 〈( 即 隐藏 )掉 、
基 类 中 任何 同名 的 对 象 函数

基 于 这 样 的 规则 ， 如 果 派生 类 定义 了 一 个 与 基类 的 虚 函 数 同名 的 虚 函 数 
但 是 **参数 列表 有 所 不 同**
那 这就 不 会 编译 器 认为 是对 基 类 虚 函 数 的 改写 (Overrides)， 而 是 **隐藏**
所 以 也 不 可 能 生 运行 时 绑 定

要 想 达成 运行 时 **绑 定** 的 效果 ， 
派 生 类 和 基 类 中 同名 的 虚 函 数 必须 具有 相同 的 原型 ，
也 即 相同 Signature〈 返 回 类 型可 以 不 同 这是 C++ 的 一 个 特征 一 协 变)。

```C++
class IRectangle {
public:
virtual ~IRectangle() {}
virtual void Draw() = 0 ;
}
class RectangleImpl : public [Rectangle {
public:
RectangleImplQ) : m_length(1), m_width(1), m_color(Ox0OFFEC4D) {}
virtual ~RectangleImpl() {}
virtual void Draw(int scale) { cout << "RectangleImpl::Draw(int)" << endl; } // (1)
virtual void Draw() { cout << "RectangleImpl::Draw()" << endl; } (2)
private:
float m_length;
float m_width;
RGB mcolor;
};

void main(void)
{
IRectangle *pRect = [Rectangle::CreateRectangle();
if (pRect == NULL) exit(-1);
pRect->Draw(); // FA-F pRect MatAS2R HW TRectangle*, ATLAH IRectangle::
/Draw0 执行 静态 类 型 检查 但 由 pRect 指向 的 对 象实际 是
/{ Rectanglelmpl 9, FAM hse Fl Rectanglelmpl::Draw()!
pRect->Draw(200); /W 同 理 由 于 下 ectangie 类 并 没有 此 类 原型 的 函数 因 此 拒绝
/ 编译 除 pRect 的 类 型 RectangleImpl* 。 此 Draw(0) 非 彼
/DrawgO
pRect->Destroy();
pRect = NULL;
}

```

如 果 RectangleImpl 不 重 定义 Draw0O 函 数 那 下 面
代码 ， 
```C++
RectangleImpl *pRectImpl = new RectangleImpli
pRectImpl->DrawQ); /f (3)
pRectImpl->Draw(200); // OK!
```
 将 无法 编译 因 为 (3) 处 调用 的 Draw0 是 基 类 的 函数 它 RectangleImpl 中 的 同名
| BR Drawn eT, BRT!

### 运行时多态  
```C++
void Draw(Shape *pShape) // SAHA
{
pShape->Draw(); / 或 : (*pShapej.Draw();
}
main()
{
Circle aCircle;
Cube aCube;
Sphere aSphere;
:Draw(&aCircle); // 绘制 一 circle
:Draw(&aCube); /W 绘制 一 cube
:Draw(&aSphere); / 绘制 一 Sphere

```

RTTI也是一种运行时多态 dynamic_cast<> 和 typeid 运算 符 
依赖 RTTI 会 导 致 严重 的 效率 低下

如 果 确 实 需要使 用 多 态 数 组 请使 STL 容器 配合 普通 指针 或者 智能 指针 
```C++
typedef SmartPtr<Shape> ShapeSmartPtr;
ShapeSmartPtr shapes[3];
ShapeSmartPtr p(new Shape(Point(1, 1)));
ShapeSmartPtr q(new Circle(Point(2, 2), 5));
ShapeSmartPtr r(new Rectangle(Point(3, 3), Point(4, 4)));
shapes[0] = p;
shapes[1] = q;
shapes[2] = 1;
for (int i = 0; 1 < 3; ++i)
shapes[i]->Draw();
```
### C++对象模型


![](高质量C++读书笔记/Pasted%20image%2020230602093920.png)


如 果 基 类 已 经 插入 了 vptr， 则 派生 类 将 继承 和 重用 vptr;

如 果 派 生 类 是 从 多 个 基 类 继承 或 有 多 个 继承 分 《从 所 有根类 开始 算起 )， 
而 中 若干 个 继承 分 上 出 了 多 态 ， 
则 派生 类 将 从 这些 分 中 的每 个 分 上 继承 一 个vptr， 
编 译 器 也 将 为 它 生 成 多 个 vtable， 
有 几 vptr 就生成 几 个 vtable (每 vptr 分 别 指向 中 一 )， 
分 别 与它 的 多 态 基 类 对 应 ;

vptr 在 派生 类 对 象 中 的 相对 位 置 不 会 随 着 继承层次 的 逐渐 加 深 而 改变 
并且现在 的 编译 器 一 般 都 vptr 放 在 有 数据 员 的 最 前 

为 了 支持 RTTI， 为 一 多 态 类 创建 一 type_info 对 象 
并 把 地 址 保存在 vtable 中 的 固定 位

![](高质量C++读书笔记/Pasted%20image%2020230602094557.png)

vtable 也 是一 个 函数 指针 数组 按 理 说 也 只 能 存放 类 型 相同 的 函数 指针 。
```C++
typedef void (__cdecl *PVFN)(void); / 通用 的 虚 函 数 指针 类
typedef struct {
	type_info *_pTypelnfo;
	PVFN _arrayOfPvfn{]; / 虚 函 数 个 数 由初始 化 语句 确定
} VIABLE;
```

如 果派 生 类 改写 了 基 类 的 虚 函 数 
则 这 个 函数 的 地址 在派生 类 vtable 中 的位 置 它 在其 基类 vtable 中 的 位 置 一致
(覆盖)

虚 函 数 第一 次 出现时 它 vtable 中 的 位 置 旦 确定 就 不随 派生 层次 的 增加 而 改变 
除 非 改 了 它 和 其 他 虚 函 数 class 中 第 一 次声明 的 顺序 

派生 类 没有 改写 基 类 虚 函 数 被 继承 下来并 插入派生 类 vtable 中 
(与 该函数 所 在 基 类对 应 下 来 的 那个 vtable)， 
且 在 派生 类 vtable 中 的 位 置 其基 类 vtable 中 的 位 相 同 

![](高质量C++读书笔记/Pasted%20image%2020230602102846.png)


 类的 静态 数据 员 可 以 class 的 定义 中 直接 初始 化 
 但 是要 清楚 : 这 只 是声明并 给 它 提供 了 一 个 初 值 而 已 
 还 必须 在 某 一 个 编译 单元 中 把 它 定义 一 次〈 即 分 配 内存)。

## 8. 对象的初始化 拷贝 析构


”初始 化 是 在 对 象 创建 的 同时 使 初 直接 填充 对 象 的 内 存单 元 因 此
不 有 数据 类 型 转换 中 间 过 程 也 就 不 会 产生 临时 对 象

赋值 则 是在 对 象 创建 好 后 任何 时 候 可 以 调用 的 而 且 可 以 多 次 调用 的 函数 
由 于它 调用 的 “=” 运 算 符 
因 此 可 能 需要 进行 类 型 转换 即 会 产生 临时 对象

如果程序员没有为一个多态类显式地定义 默认构造函数 拷贝构造函数 析构函数或拷贝赋值函数 
那么 编译 器会 自动 地 生成 相应 的 函数 它 们 都public inline的 ,
并 在 其中 插入 正确 初始 化 或 修改 vptr 数 据 员 值 代码 ，
从 而 确保基 类 对 和 派生 类 对 象 构造 时 及 在 它们 之 间 拷贝时 
vptr 能 够向 重新 指向 恰当 的 vtable. 
这样 四 个 函数 就 分 别 叫 做 
非 平凡 默认 构造函数 
非 平凡 拷贝 构造 函数 
非 平凡 析 构 函数 和 
非 平凡 拷贝 赋值 函数 

在 构造 函数 体内 来 初始 化 数据 不 是 真正 意义 上 的初始化 而 是 赋值

由 于 构造 函数 是 创建 一 对象 时 自动 调用 的 第 一 个 成员 函数 
因 此 我 们也 愿意 把 构造 函数 体内 的 赋值 语句 当成 初始 化 来 看

真正 的 初始 化 是 用 所 谓 “初始 化表达式表”( 简称 初始 化 列表 ) 进行
初始 化 列表 位 于 构造 函数 参数 表 之 后 在 函数 {} 之 前

如 果 类 存在 继承 关系 派 生类可 以 直接 在其 初始 化 列表 里 
调用 基 类 的 特定构造 函数 以 向 它 传递 参数 
因 为 我们 不 能 在 初始 化 对 象 时 访问 基 类 的 数据 成 员
```C++
class A {
public:
A(int x); /A 的 构造 函数
vas B : public A {
B(int x, int y); /B 的 构造函数
Buin x, int y) : A(x) / 在 初始 化 列表 里 调用 A 的 构造 函数
{
}
```

类 的 非 静 const 数据 成 和 引用 成 员 只 能 在 初始 化 列表 里 初始 化 
因 为它们 只 存在 初始 化 语义 而 不 存在 赋值 语义
```
int m;
int &n=m;

class A {
A(void); BRA
A(const A& other); / 拷贝 构造 函数
A& operator =( const A& other); / 赋值 函数
};
class B {
public:
B(const A& a); /B 的 构造 函数
Private:
A ma; / 成 员 对
};
/ (1) 采用 初始 化 列表 的 方式初始
B::B(const A& a) : m_a(a)
{
}
(2) 采用 函数 体内 赋值 的 方式 初始
B::B(const A& a)
{
m_a=a; // 〈 调 了 A 的 默认 构造 函数 )， 再 调用类 A 的 赋值 函数 才 将 参数 a赋 m_a。
}
```

 对于 内 部 数据 类 型 的 数据 员 而 言 两 种 初始 化 方式 的 效率 几乎 没有 区别
 ```C++
 class F {
public:
F(int x, int y); / 构造 函数
Private:
int m_x;
int m_y;
}
CL) 采用初始 化 列表 的 方式初始
F::F(int x, int y) : m_x(x), m_y(y)
{
}
(2) 采用 函数 体内 赋值 的 方式初始
F::F(int x, int y)
{
m_x=x;
my=y;
}
 ```
初始化顺序序 并不 一 定 与 你在 初始 化 列表 中 为 它们 安排 的 顺序 一 致 
编 译 器 总 是按照 它们 在 类 中 **声明 的 次序** 来 初始 化 的 
所以最 好 是 按照 它们 的 声明 顺序 书写 员 初 始 化 列表 

如 果 各 个 员 的 初始 化 存在依赖 关系 
要 注意 顺序 问题 
可 以 调整 下 数据员 的 **声明 顺序** 避免 这 个 问题

构造 函数首先 调用每 一 个基类 的 构造 函数 
然 后 调用成 员 对 象 的 构造 函数 
而 每 一 基类 的构造 函数 又 将 首先 调用 它们 各 自 基类 的 构造 函数 直 到 最 根 。 

析 构 会 严格 按照 对 象 构造 相反 的 次 序 执行 该 次 序 是 唯一 的 否 则 编译 器 将 无 法 自动 执行 析 构 过 ;

数据 员 的 初始 化 次 序 完全 不 受 它们 在 初始 化 列表 中 出 现 次 序 的 影响 
只由 它们 在 类 中 声明 的 次 序 决 定 因 为 这 个 顺序 是 唯一 的

如 果 数 据 员 按照 初始 化 列表 的次序 进 行 构造 将 导致 析 构 函数 无法得到唯一的逆序。

拷贝 构造 函数 的 参数 必须 是 同类 对 象 的 引用 ， 而 不 能 是 对 象 值
```C++
class A
 拷贝构造 函数
public:
A(A copy){...} “#Q)陷入 不 地 分 配 堆栈 的 无 限 递归
A(const A& other) {...} (2)
MN...
上
Aa;
Ab=a;
```

不 主 动 编写 拷贝 构造 函数 和 拷贝 赋值 函数 编 译 器
将 “ 按 员 拷贝 ”的 方式 自动 生成 相应 的默认 函数
倘 类 中 含有 指针 员 或 引用 成员 ， 那 这 两 个默认 的 函数 可 能 隐 含错误

![](高质量C++读书笔记/Pasted%20image%2020230602112452.png)

拷贝 构造 函数 是在对 象 被 创建 并 用 另 一 个 已 经 存在 的 对 象 来 初始 化 它 时 调用 
赋值函数只能把一个对象 赋值 给 另 一 个 已 经 存在 的 对 象 
使 得 经 存在 的 对 象 有 和 源 对 象 相同 的 状态 。

![](高质量C++读书笔记/Pasted%20image%2020230602112740.png)

只 需 将 拷贝 构造 函数 和 拷贝 赋值 函数 声明 private， 并 且 不 实现 它们
显 式 声明 的 这两 个函数 会 阻止 编译 器 自动 生成 相应 的 默认 函数

甚至 可 以 把 类 的 有 构造 函数 和 赋值函数 都 声明 为 private， 
这 样 就 彻底 阻止 了类 的 实例 化 

或 者 把 默认 构造 函数 声明为 private， 
而 把 其 他 带 参 数 的 构造 函数明 public， 
这 样 强 用 户 使 用 带 参数 的 构造 函数 来 声明 和 定义 对象
```C++
class A {
fe.
const char * GetType(); / 未 实现
private:
A(const A& a); / 私有 的 拷贝 构造 函数
A& operator=(const A& a); / 私有 的 拷贝 赋值 函数
is
如 果 有 人 试图 编写 如下 程序 :
A ba); UBF T AAA See DFE
b=a; / 调用 了 私有 的 赋值 函数
```


派生 类 的 构造函数 应 在其 初始 化 列表 里显式 地 调用 基 类 的 构造 函数

如 果 基 类 是 多 态 ， 那 么 必须 把 基 类 的 析 构 函数 定义 为 虚 函 数
这 样 可以 其 他虚 函 数 一 实现 动态 绑 定 否则 有可 能 造成 内 存 泄漏
delete 基类指针 不会释放派生类

基关 的 构造函数 、 析 构 函 数 赋信函 数都 不 能被 派生 类 继承
![](高质量C++读书笔记/Pasted%20image%2020230602114921.png)

## 9. C++函数 高级特性

### 重载

编译 器 根据 参数列表 为 个 重 载 函 数 产生 不 同的 内 部 标识 符 

```C++
void _cdeci foo(intx, inty);
```

该 函数 被 C 编译 器 编译 后 在 库 中 的 名 字 为 foo， 
而 C++ 编译 器 则 会 产生 像 | `_foo_int_int` 之 类 的 内 部 名 字 用 来 支持 函数 重 载 
由 于 编译 后 的 名 字 不 同 ，C++ 程 序 | 不能 直接 调用 编译 后 C 函数 。
C++ 提 供 了 一 C 连接交换 指示 符 extern "C" 来 解决 这 个 问题

这就 是连接 规范 的 概念

上 述代码 是 在 告诉 C++ 编译 器 函 foo 是 C 连接
函数 应 该 为 生成 名 字 `_foo` 而 不 `_foo_int_int`， 
并 指示连接 器 到 C 程序 库 中 去 找该函数 的 定义

```C++

void output( intx ); / 函数 声明
void output( float x ); / 函数 声明
void output( int x )
{
cout <<" output int "<< x << endl ;
}
void output( float x )
{
cout <<" output float " << x << endl ;
}
void main(void)
{
int x=1;
float y = 1.0;
output(x); // output int1
output(y); // output float 1
output(1); // output int 1
output(0.5); / 错误 ! 不 明确 的 调用 ， 因 为 自动 类 型 转换
output(int(0.5)); // output int 0
output(float(0.5)); /f output float 0.5
}
```

###  成员函数的重载 覆盖与隐藏

成 员 函 数被**重载** 的 特征 ：
具 有 相同 作用域 〈 即 同一 个 类 定义 );
函数名字相同
参数 类 型 顺序 或数目 不 同〈 包 const 参数 和 非 const 参数 );
virtual 关键字 可有可无

**履 盖** 是 指派 生 类 重新 实现 〈 或 者 改写 了 基 类 的 员 函 数 其 特征 :
不 同 的作用域 〈 分 别 于派生 类 和 基 类 中 );
函数名称相同
参 数列 表完 全 相同 ;
基 类 函数 必须 是 虚 函 数 **只能覆盖 同名同参 虚函数**
```c++
#include <iostream.h>
class Base {
public:
void f(int x){ cout << "Base::f(int) "<< x << endl; }
void f(float x){ cout << "Base::f(float) "<< x << endl; }
virtual void g(void){ cout << "Base::g(void)" << endl; }
b
class Derived : public Base {
public:
virtual void g(void){ cout << “Derived::g(void)" << endl; }
上
void main(void)
{
Derived d;
Base *pb = &d;
| pb—>f(42); // Base::f(int) 42
pb 一 >f3.140; ——_‘// Base::f(float) 3.14
pb 一 >gO; / Derived::g(void)
}
```

函数 Base::fint) Base::f(float)构成重载
Base::g(void) 被 Derived::g(void) 覆盖

虚 函 数 的 覆盖 有 两 种 方式 : 完全 重 和 扩展 。 
扩展 是 指派 生 类 诬 数 首先 调用 基 类的 虚 函 数 然 后 再 增加 新 的 功能 。

“隐藏 ”是 指派 生 类 的 员 函 数 遮 了 与 其 同名 的 基 类 成 员 函 数：
1. 派 生 类 的 函数 基类 的 函数 同名 ， 但 是 **参数 列表 有所 差异** 
不轮 有无 virtual 关键 ， 基 类 的 函数 在 派生 类 中 将 隐藏
2. 派生类的函数与基类的函数同名，**参 数列表也相同** 但 基类函数 没有virtual 关键字。 
此 时 , 基类的函数在派生类中将隐藏 (注意 别 与 覆盖 混淆 )。

解释：
对于第一点 同名不同参 同作用域为重载 不同作用域为隐藏
对于第二点 同名同参 是虚函数 则覆盖 不是虚函数则隐藏


![](高质量C++读书笔记/Pasted%20image%2020230602133459.png)

```C++
class Base {
public:
void f(int x);
}
class Derived : public Base {
public:
void f(char *str);
上
void Test(void)
{
Derived *pd = new Derived;
pd—>f(10);—_// error!
}

class Derived : public Base {
public:
void f(char *str);
void f(int x) { Base::f(x); } / 调用传递
};
```

void Foo(int x = 0, int y = 0); / 正确 默 认值 出现在 函数 的 声明 中
如 果 函 有 多 个 参数 参 数 只 能 从 后 向 前 依次 默认 

![](高质量C++读书笔记/Pasted%20image%2020230602134849.png)

![](高质量C++读书笔记/Pasted%20image%2020230602135659.png)

![](高质量C++读书笔记/Pasted%20image%2020230602135956.png)


C++ 标准 规定 当 为 一 类 型 重 “++”/“--” 的 前 置 本 时 不需要 参数
当 为 一 个 类 型 重 “++”/“--” 的 后 置 本 时 ， 需 要 一 int 类 型 的 参数 作为标志 〈 即 哑 元 非 具名 参数 
```C++
class Integer {
public:
Integer(long data) : m_data(data){ }
Integer& operator++() { /W 前 置 版 : 返回 引用
cout<< “Integer::operator++() called!” << end];
m_datat++ ;
return *this;
}
Integer operator++(int) { // 后 置 版 : 返回 对 象 的
cout<< "Integer::operator++(int) called!" << end];
Integer temp = *this ;
m_data++ ; // BR: ++(*this) ;
return temp; / 返回 this 对 象 的 旧 值
}
/ 其 他 成 员
private:
long m_data; / 对 long 的 封装
};
void main(void)
{
Integer x = 1; // call Integer(long)
+4x ; // call operator++Q)
xtt+ 3 // call operator++(int)
}
输出 结果 :
Integer::operator++() called!
Integer::operatort++(int) called!
```

### 内联
```C++
void Foo(intx, int y);
inline void Foo(intx, int y) // inline 与 函数 定义 体 放 在 一
{
...
}

定义 在类 声明 之 中 的 员 函 数 将 自动地 成 内 联 函数
class A {
public:
void Foo(int x, int y) { ... } / 自动 地 成 内 联 函数
}
```

### 类型转换函数


```C++
class MyString {
public:
MyString(size_t size, char c =
fhe.
private:
char *m_data;
Point pl = 10.5;
pi = 20.5;
\0);
}

MyString strName = 20;
strName = 40; / 莫名 其妙 

void f(MyString str);
f(100);
如果 当 前程序 中 没有 其 他 重 载 恕 函数， 编译 器 就 将 它暗 中 转换 :
f(MyString(100));
```

当你的类定义中出现类似的情况时，可以在构造函数前面添加关键字 explicit 
将其 声明 为 显 式 的 意 为要 求 用 户 必须 显 式 地 调用 该 构造 函数 来 初始 对 象 
以 明确 表明 他 的 意图 

![](高质量C++读书笔记/Pasted%20image%2020230602141650.png)



1. `static_cast<desttype>(src_obj)`， 作 用 相当 于 C 风格 的强制 转换 , 
但 是 在 多 重继承 的情况 ， 它 会 正确 地 调整 指针 的 值 
而 C 风格 的强制 转换 则 不 会调整 
它 可 以 遍历继承 树 来 确定 src_obj 与 dest_type 的 关系 ,
但 是 只 在 编译 时进行 〈 此 所 谓 静 )， 
如 果使 用 它 来 downcast 操作 ， 则 会 存在 隐患 。

2. `const_cast<dest_type>(src_obj)`， 用 于去除 一 个对 象 const/volatile 属性 
3. `reinterpret_cast<desttype>(src_obj)`, 我 们 可 以借助 它 把 一 个 整数 转换 成 一地 址 或 者 在任何 两 种 类 型 的 指针 之闻 转 。 使 用 该 运算 符 的 结果 很 危险 ，请你 要 轻易使 
4. `dynamic_cast<dest_type>(src_obj)`， 在 运行 时 台历 继承 〈 类 层次 结构 ) 来确定 src_obj 与 desttype 的 关系

在 C++ 程序 中 尽量 不 要 再 使 C 风格 的 类 型 转换 
除 非 源 对 和 目标 类型 都 **基本 类 型** 的对 象 指针 ， 否 则 很 不 安全 。
C++ 的 类 型 转换 运算 符在 需要 的 时 候 会 进行**指针 调整** 因 此 结果 比较 安全 。

### const成员函数
任何 不 会修改 数据 员 的 员 函 数 都应 该 声明 为 const 类
```C++
class Stack {
public:
void Push(int elem);
int Pop(void);
int GetCount(void) const; // const A AB
private:
int m_num;
int m_data[ 100};
I
int Stack::GetCount(void) const
t
++m_num; / 编译 错误 企 图修改 数据 成 m_num
Pop(); / 编译 错误 企 图 调用非 const 成 员 函
return m_num;
}
```

Static KA BRA RANA const 的 这 是 因为 static 成 员 函 数 只 是 全 局 函 数 的 一 个 形式 上 的 封装 而 全 局 函数 不 存在 const 一 ; 何况 static 成 员 函数 不 能 访问 类 的 非 静态 成 (没有 this 指针 )， 修 改 非 静态 数据 员 又 从 何说 起 ? 

## 10. C++异常处理 和 RTTI

何 一 种 类 型 可 以 当做 异常 类 型 因 此 任何 个 对 象 可 以 当做异常 对 ，
包括 基本 数据 类 型 的 变量 常量、 任何 类 型 的 指针 、 引 用 、 结 构 等 甚 至 空 结构类 的 对象
这 是 因为 异常 仅仅通过 类 型 而不是 通过 值 来 匹配 的 否 则 就 又 退回 到
了 传统 的 错误 处 理 技术 上 

```C++
void test(int index)
{
}
double x = 100, y = 20.5;
int x[20] = { 0};
try {
cout << Devide(x, y) << endl; / 可 能 抛 出 异常 DevidedByZero
if(index >= 20) throw OverFlow(); 1) iE EGLF 4 Bil try RA
if(index < 0) throw UnderFlow(); / 抛 出 点 位 于 当前 try 块 内
}
catch(const DevidedByZero& ex) {
cerr << ex.description() << endl;
}
catch(const OverFlow&) { / 省 略 参数 名 称
cerr << "Overflow occurred!" << endl;
}
catch(const UnderFlow&) {
cerr << "Underflow occurred!" << endl;
}
catch(...) { / 捕获 他所 有 可 能 的 异常
cerr << "Unexpected exception occurred!" << endl;
}
```

由 于 异常 处理 机 制 采用 类 型 匹配 而 不 是 值 判 断 因 此 catch 块 参数 
可 以 没有 参数 名 称 只 需要 参数 类 型 除 非 确实 要 使 用 那个 异常 对 

虽然 异常 对 象 上 去 局 部 对 象 但 是 并 非 创建 在 函数 堆栈 上 ， 
而 是创建 在 专用 的 **异常 堆栈** 上 ， 因 此 它 才 可 以 跨接 多 个 函数 而 传递 上 层 ，
否则 在 堆栈 清 退 的 过 程 中 就 会 销毁 .
**不 要** 企图 把 **局 部 对 象 地 址** 作为异常 对 象 **抛 出** 
因 为 局 部 对 象 会 蜡 常 出 后 函数 堆栈 清 退 的 过 程 中销毁 


![](高质量C++读书笔记/Pasted%20image%2020230602161511.png)


```C++
double Devide(double x, double y) throw(DevidedByZero);  // 只 可 能 出 一种 异常
bool func(const char *) throw(T1, T2,T3); // 可 能 抛 3 种 异常
void g() throw(); // (3) 不抛出任何异常
void k();  // 可 能 出 任何 异常 也 可能 不 出 任何异常
```

所 有从try 到 throw 语句 之间 构造 起 来 的 局 部对 象 的析 构 函数将被 自动 调用
(以 与构造相 反 的 顺序 )， 然 后 清 退 堆栈 (就像 函 数 正 退出 那样 )。

如 果 一 个 函数 在 运行 时 抛 了 异常 
于 是 异常 处 理 机制 调用 局 部 对 象 析构 函数 〈 清退 扒 )， 
而如 果 此 时 某 一 析 构 函 数 恰 也 要 抛 出一 个 异常 那 这个 异常 由 谁 处 理 ? 
没有 办 法 异 常 处 理 机 制 只 好 调用;terminate0。

如 果 你 真 的 不 得 不 从 析 构 函数 内 抛 出 异常 的 话 你应 该 
首先 检查一 下 看 当前 是 有 一 个 未 捕获 的 异常 要 被 处 理 如 果 没有 ， 
说 明 该 析 构 函数 的用 并 非 由 一 外 部 异常 引起 而 是 正常 的 销毁 
于是 你 可 以 出 一 个 异常 上 层:程序 来 捕获


一 些 编译 器可 以 设 异 常 处 理 支持 开关 ， 当 关闭 异常 理 支持 后 附加 的 数据 结构 查 找表 、 额 外 的 代码 都 不 生成

要 合理 安排 异常 处 理 的 层次 : 一 要 把 派生 类 的 异常 .
获 放 在 基 类 异常 捕获 的 前 面 否 则派生 类 异常 匹配 永远 也 不 会执行到 


如 果实 在 无 法判断 到 底 有什么 异常 抛 出 那 就 使 “一 打 尽”的 策
略 了 : catch(void*) 和 catch(...)。 但 要 记 住 catch(void*) 和 catch(..)
必须 放 在 异常 组 合 的 最 后 面 并 且 cateh(void*) 放 在 catch(...) 的 前 

![](高质量C++读书笔记/Pasted%20image%2020230602165212.png)

为 了 支持 dynamic_cast<> 运 算 符 ，RTTI 机 制 必须 维 护 一 棵 继承 ，
即 base class table 模型 ( 或 类 似 的 索引 表格 )

```C++
void DeviceControllor::ControlThem(HomeElectricDevice& device)
{
}
Command cmd = GetCommand();
switch(cmd)
{
case OPEN:
case PLAY VCD:
try {
Television tv = dynamic_cast<Television &>(device);
tv.PlayVCDQ; // PRCERESE ADEE Television WRAEAIE BCH!
}
catch(std::bad_cast&) {
MsgBox("This device cannot play VCD!");
}
break;
```

## 11. 16章 内存管理
 野 指 针 
 free 完 delete完 置NULL
 
对 于 内 部 数据 类 (如 ADT/UDT) 的 对 象 而 言 光 用 mallocO/free0 无 法 满足 动态 对 象 的 要 求 
对象 在 创建 的 同时 要 **自动 调用 构造 函数** 对象在 销毁 的 时 候要 **自动 调用 析 构 函数**
由 mallocO/free0 是 库 函 数而 不 是 运算 符 不 在 编译 器 控制 权限
不 能 把调用 构造函数 和 析 构 函数 的任务强加 给 它们 。 
因 此 C++ 语言 需要 一能 完成 动态 内 存 分 和 初始 化 工作 的运算 new, 
以 及一 个 能 够 完成清 理与 释放 内存工作 的运算符 delete。

plain new、nothrow new 及 placement new.
```C++
new 的 使 用 方 法 见 示 例 16-10。
// plain new 定义
void * operator new(std::size_t) throw(std::bad_alloc);
| void operator delete(void *) throw();

Char * GetMemory(unsigned long size)
《
char *p = new char[size];
return p;
}
void main(void)
{
try {
char *p = GetMemory(1000000); / 可 能 抛 出 std::bad_alloc 异 常

delete []p;
}
catch(const std::bad_alloc& ex) {
cout << ex.what() << endl;
}

/// nothrow new 定义
void * operator new(std::size_t, const std::nothrow_t&) throw();
void operator delete(void *) throw();

Void func(unsigned long length)
{
unsigned char *p = new(nothrow) unsigned char[length];
if(p == NULL) cout << "allocate failed!"<< endl;
Hf...
delete []p;
}
```

placement new 不 用 担心 内 存 分配 失败 因 为 它 根本 就 不 会 分配 内 存 
它所 做 的 唯一 一件 事情 就 是 调用 对 象 的 构造 函数 

```C++
include <new>
#include <iostream>
void main(void)
{
using namespace std;
char *p = new(nothrow) char[4]; // nothrow new
if(p == NULL) {
cout << "allocate failed!"<< endl;
exit(-1);
}
仁 …
long *q = new(p) long(1000); // placement new
Ie.
delete [ip; / 释放 内 存
```

Placementnew 的 主 要 用 途 就 是 : 反 复 使 用 一 块 较 大 的 动 态 分 配 成 功 的 内 存 来 构 造 不 同 类 型 的 对 象 或 者 它 们 的 数 组 。 比 如 , 可 以 先 申 请 一 个 尸 够 大 的 字 符 数 组 , 然 后 当 需 要 时 在 它 上 面 构 造 不 同 类 型 对 象 或 其 数组

```C++
 “ 使 用 placement new 构 造 起 来 的 对 象 或 其 数 组 , 要 显 式 地 调 用 它 们 的 析 构 函 数 来 销 毁 ( 析 构 函 数 并 不 释 放 对 象 的 内 存 , 千 万 不 要 使 用 delete。 这 是 因 为 ,placement new 构 造 起 来 的 对 象 或 其 数 组 的 大 小 并 不 一 定 等 于 原 来 分 配 的 内 存 大 小 , 因 此 使 用 delete 会 造 成 内 存 泄 漪 , 或 者 在 之 后 释 放 内 存 时 出 现 运 行 时 错 误 。 见 示 例 16-14 ( 假 设 ADT 表 示 任 意 复 合 数 据 类 # ).
示 例 16-14
#include <new>
#include <iostrem>
void main(void)
《
using namespace std:
char *p = new(nothrow) char[sizeof(ADT) + 2]:; / nothrow new
if(p == NULL) {
cout << "allocate failed!"<< endl;
exit(-1);
}
ADT *q = new(p) ADT; // placement new: 不 必 担 心 失 败
历 .
// deleteq; / 错 误 ! 不 能 在 此 处 调 用 deleteq;
q->ADT::-ADTO; / 显 示 调 用 析 构 函 数
delete []p; / 再 释 放 内 存
}
```


```C++
cout << sizeof(char) << endl;
cout << sizeof(int) << endl;
cout << sizeof(unsigned int) << endl;
cout << sizeof(long) << endl;
cout << sizeof(unsigned long) << endl;
cout << sizeof(float) << endl;
cout << sizeof(double) << endl;
cout << sizeof(void *) << endl;
```

## 12. 用对象模拟指针

 些 软 件 库 使 用 这 种 技 术 来 封 装 指 针 , 基 本 上 分 为 如 下 几 种 情 况 。
1. 采 用 拷 贝 方 式 。 这 样 的 指 针 对 象 既 负 责 创 建 数 据 对 象 , 又 负 责 删 除 数 据 对
象 ,STL 容 器 对 象 采 用 的 就 是 这 种 方 式 。显 然 , 采 用 这 种 方 式 的 指 针 对 象
责任 最 清 晰 。
2. 采 用 完 全 接 管 方 式 , 指 针 对 象 不 负 责 创 建 数 据 对 象 , 但是负责删除数据对
象 , 即 不 仅 接 管 了 源 指 针 指 向 的 对 象 , 而 且 接 管 了 它 的 所 有 权 。auto ptr<>
类 就 是 采 用 这 种 方 式 实 现 的 。
3. 采 用 接 管 方 式 , 是 既 不 负 责 创 建 数 据 对 象 也 不 负 责 删 除 数 据 对 象 , 这 就 是
我 们 的 模 拟 指 针 。STL 中 的 迭 代 器 (iterator》 采 用 的 就 是 这 种 方 式 , 它们 在
行 为 上 与 底 层 的 指 针 变 量 没 有 什 么 太 大 区 别 , 只 是 在 使 用 方 式 上 统 一 了 起
来 。
4. 完 全 接 管 方 式 和 深 拷 贝 方 式 结 合 。 一 般 情 况 是 : 拷 贝 构 造 和 拷 贝 赋 值 采 用
深拷 贝 方 式 , 而 指 针 构 造 和 指 针 赋 值 采 用 接 管 方 式 。 这 种 方 式 最 容 易 产 生
运 行 时 内 存 访 问 冲 突 和 内 存 泄 漏 问 题 , 因 此 建 议 不 要 使 用 。

### 泛型指针auto_ptr

```C++
template<typename T>
class auto_ptr 【
public:
explicit autp_ptr(T *p = 0) : m_ptr(p){}
auto_ptr(const auto_ptr<T>& copy) : m_ptr(copy.release()){}
auto_ptr<T>& operator=(const auto_ptr<T>& assign) {
if (this != &assign) {
delete m_ptr;
m_ptr = assign.release(); H 释 放 并 移 交 拥 有 权
多
}
return *this;
hs
~auto_ptr(){ delete m_ptr; } / 负 责 释 放 存 储
T& operator*(){ return.*m_p; } / 重 载 “*2
T* operator->(){ return m_p; } / 重 载 “->“
T* release() const {
T *temp = m_ptr;
(const_cast<auto_ptr<T> *>(this))->m_ptr = 0;
return temp;
}
private:
工 * m_ptr;
}
```

使 用 auto_ptr<> 这 样 的 灵 巧 指 针 有 一 个 好 处 : 当 函 数 即 将 退 出 或 有 异 常 抛 出 的 时 候 , 
不 再 需 要 我 们 显 式 地 用 delete 来 删 除 每 一 个 动 态 创 建 起 来 的 对 象

### 带引用技术的智能指针

带 有 引 用 计 数 功 能 的 智 能 指 针 兼 有, 智 通 指 针 共 享 实 值 对 象 和 auto ptr 自 动 释 放 实 值 对 象 的 双 重 功 能

![](高质量C++读书笔记/Pasted%20image%2020230603113212.png)

“auto_ptr不 满 足 STL 标 准 容 器 对 元 素 的 最 基 本 要 求 
,auto ptr 对 象 和 它 的 拷 贝 不 会 共 享 实 值 | 对 象 , 任 何 两 个 auto_ptr 也 不 应 该 共 享 同 一 个 实 值 对 象 。 这 就 是 说 ,auto ptr 对 象 和 | 它 的 拷 贝 并 不 相 同 。 然 而 根 据 STL 容 器 “ 值 “ 语 义 的 要 求 , 可 拷 贝 构 造 意 味 着 一 个 || 对 象 必 须 和 它 的 拷 贝 相 同 ( 标 准 中 的 正 式 定 义 比 这 稍 复 杂 一 些 。 同 样 , 可 赋 值 意 味 耒萱把 个 对 象 赋 值 给 另 一 个 同 类 型 对 象 将 产 生 两 个 相 同 的 对 象 。 显 然 ,auto ptr 不 能 LK 要 求 , 它 与 上 面 的 结 论 矛 盾

```C++
示例16-23
std::list< std::auto_ptr<int> > la; / auto_ptr 列 表
std::auto_ptr<int> p1(new int(1));
std::auto_ptr<int> p2(new int(2));
std::auto_ptr<int> p3(new int(3));
la.push_back(p1); // compiling-error!
la.push_back(p2); // compiling-error!
la.push_back(p3); // compiling-error!
set<auto_ptr<int> > sa; / auto_ptr RA: 假 设 为 auto ptr 定 义 了 operator<
sa.insert(p1); // compiling-error!
sa.insert(p2); // compiling-error!
sa.insert(p3); // compiling-error!

template<typename T>
void list<T>::push_back(const T& x)
{
T *p = operator new(sizeof(T)); / 分 配 内 存空 间
new (p) T(x); // placement new, 调 用 T 的 copy constructor
------ / 将 p 交 给 容 器 管 理 , 调 整 容 器 大 小
}

int main()
{
typedef std::vector<std::auto_ptr<int> > IntPtrVector;
IntPtrVector va;
std::auto_ptr<int> p1(new int(1));
std::auto_ptr<int> p2(new int(2));
std::auto_ptr<int> p3(new int(3));
std::auto_ptr<int> p4(new int(4));
std::auto_ptr<int> p5(new int(5));
va.push_back(p1);
va.push_back(p2);
va.push_back(p3);
va.push_back(p4);
va.push_back(p5);
1 ( 注意 : 以 下 操 作 并 非 放 在 一 起 进 行 , 仅 是 示 范 )
IntPtrVector vb = va; va 丧 失 对 所 有 实 值 对 象 的 拥 有 权 ,
/ 元 素 成 为 NULL 指 针
vb.resize(10); / 新 增 的 元 素 都 为 NULL指 针
std::sort(vb.begin(),
vb.end0);

std::auto_ptr<int>
t
=
vb.front();
//
改
变
了
容
器
元
素
std::auto_ptr<int> r = vb[3];

std::list<std::auto_ptr<int> > la;
std::copy(vb.begin(), vb.end(), std::back_inserter(la));

return 0;
```

Scott Meyers 在 《Effective STL) Item 8 中 详 细 地 分 析 了 对 auto ptr 容 器 进 行 排 序
| 时 可 能 会 导 致 的 问 题 。 但 是 在 MS VC++ 环 境 下 经 测 试 , 并 没 有 出 现 书 中 所 描 述 的 悲
| 惨 结 局 , 而 是 结 果 正 确 。 主 要 原 因 在 于 C++ 标 准 并 没 有 要 求 std::sort等 泛 型 算 法 的 实
现 必 须 采 用 某 一 种 方 法 , 而 是 只 规 定 了 它 们 的 接 口 、 功 能 和 应 该 达 到 的 性 能 要 求 ( 容
器也 是 如 此 )。 因 此 , 不 同 的 STL 实 现 可 能 采 取 不 同 的 方 法 , 比 如 有 的 sort 实 现 采 用
快速 排 序 法 , 而 有 的 采 用 插 入 式 排 序 法 等 。 不 同 的 排 序 方 法 在 遮遇 auto_ptr这 样 的 容
| 器 时 可 能 就 会 产 生 不 同 的 结 果 


可 见 , 督 能 指 针 “ 可 以 “ 还 是 “ 不 可 以 “ 作 为 容 器 的 元 素 并 非 绝 对 的 , 不 仅 与 ,STL 的 实 现 有 关 , 而 且 与 STL 宪 器 的 需 求 和 安 全 性 及 容 器 的 语 义 有 关


```C++
由于 auto_ptr 是 对 象 化 的 智 能 指 针 , 具 有 自 动 释 放 资 源 的 能 力 , 因 此 它 真 正 有 价
, 值 的 用 途 是 在 发 生 异 常 时 避 免 资 源 泄 濡 。 比 如 , 如 果 不 使 用 auto ptr, 则 下 列 代 码 在
发 生 异 常 的 情 况 下 不 得 不 多 次 手 工 释 放 资 源 ( 见 示 例 16-28)。
示 例 16-28
class A{ ... }5
void func()
《
A *pA= 一 new A;
try {
wee // using *pA
}
catch(...) {
delete pA; MBERTAIN BE LAE IK
throw;
}
delete pA; / 函 数 退 出 时 还 要 显 式 释 放
}

现 在 有 了 auto ptr, 我 们 就 可 以 这 么 做 ( 见 示 例 16-29).
示 例 16-29 |
classA{ ... }5
void func()
{
auto_ptr<A> pA(new A);
// using *pA



如果 想 防 止 无 意 中 修 改 auto ptr 对 实 值 对 象 的 拥 有 权 , 可以 使 用 const auto_ptr,
! 这 样 的 auto_ptr 只 能 使 用 引 用 或 指 针 传 递 , 不 能 使 用 值 传 递 , 也 不 能 赋 值 和 拷 贝 构 造 

classA{ ... };
void func()
{
const auto_ptr<A> pl(new A);
we // using *pA
auto_ptr<A> p2(p1); // error!
auto_ptr<A> p3;
p3=pl; // error!


// smart pointer stl

示 例 16-31
) typedef SmartPtr<Shape> ShapeSmartPtr;
typedef std::list<ShapeSmartPt> ShapeList;
bool operator<(const Point& left, const Point& right)
{ return ((left.m_x < right.m_x) && (left.m_y <right.m_y); }
bool operator—=(const Point& left, const Point& right)
{ return ((left.m_x == right.m_x) && (left.m_y == right.m_y); )
| bool operator<(const ShapeSmartPtr left, const ShapeSmartPtr right)
{ return (left->GetOriginQ < right->GetOrigin()); 》
1 bool operator==(const ShapeSmartPtr left, const ShapeSmartPtr right)
{ return (left->GetOriginQ == right->GetOrigin()); }
ShapeList shapes;
ShapeSmartPtr p(new Shape(Point(1, 1)));
ShapeSmartPtr q(new Circle(Point(2, 2), 5));
ShapeSmartPtr r(new Rectangle(Point(3, 3), Point(4, 4)));
shapes.push_back(p);
shapes.push_back(q);
shapes.push_back(r);
|
std::sort(shapes.begin(), shapes.end());

for(ShapeList::const_iterator first=shapes.begin();first !=shapes.end();++first)
(*first)->DrawQ);
```


## 13. 17章 STL

STL 主 要 包 括 下 面 这 些 组 件 
IO流、 string 类 、 容 器 类(Container)、 迭 代 器 熹 (Iterator)、 存 储 分 配 器 (Allocator)、 
适 配 器 (Adapter)、 函 数 对 象 (Functor)、 泛 | 型 算 法 (Algorithm)、 
数 值 运 算 、 国 际 化 和 本 地 化 支 持 , 以 及 标 准 异 常 类 等 。

C++标 准 规 定 , STL 的 头 文 件 都 不 使 用 扩 展名 
过 去 的 C 程 序 库 头 文 件 在, 并 入 C++ 标 准 库 时 也 都 去 掉 了 .h 扩 展 名 , 同 时 增 加 了 前 缀 “c“。
STL 组 件 都 被 纳 入 了 名 字 空 间 std::, 所 以 在 使 用 其 中 的 组 件 之 前 需 使 用 using 声 明 或 using 指 令 , 
或 者 也 可 以 在 每 一 处 都 直 接 使 用 完 全 限 定 名 std::。 

![](高质量C++读书笔记/Pasted%20image%2020230603051215.png)

### STL头文件分布

### 容器类
![](高质量C++读书笔记/Pasted%20image%2020230603050007.png)

	关 联 式 容 器 multimap 和 multiset 也 都 分 别 定 义 在 <map> 和 <set> 中 ,
	hash_multimap 和 hash_multiset 定 义 在 <hash_map>和<hash_set>中
	
### 泛型算法

	像 C++VC 数 组 、 字 符 留 、UO 流 等 特 殊 的 容 器 也 可 以 使 用 标 些 泛 型 算 法 一 一
	它 们 定 义 在 头 文 件 <algorithm>和 <utility> 中 
	
### 迭代器
	如 输 入 / 输 出 迭 代 器 、 插 入 迭 代 器 、 反 向 迭 代 器 等 都 是 迢 代 器 适 配 器 , 
	定 义 在 头 文 件 <iterator>中 

### 数学运算库
![](高质量C++读书笔记/Pasted%20image%2020230603050515.png)

### 通用工具
![](高质量C++读书笔记/Pasted%20image%2020230603050604.png)

### 其他头文件
	<typeinfo> 、<stdexcept>、<strsteam>、<string>、<istream>、
	<ostream>、<iostream>、<new>、<iomanip>、<fstream> 
	
### 容器设计原理

容 器 在 概 念 上 是 一 种 可 以 动 态 增 大 和 减 小 的 模 型
其**元 素 对 象** 在 实 现 上 不 可 能 直 接 保 存 在 容 器 对 象 里 面
应 该 保 存 在 自 由 内 存 (Free Memory) 或 堆 , CHeap)》 上

### 内存映像
	
![](高质量C++读书笔记/Pasted%20image%2020230603051108.png)

![](高质量C++读书笔记/Pasted%20image%2020230603052000.png)

### 存储方式 访问方式

向 量 Cvector) 和 链 表 (linked list) 是 两 种 最 基 本 的 动 态 结 构 , 
也是 STL 中 两 种 最 基 本 的 容 器 , 
分 别 对 应 动 态 数 组 和 链 接 表 结 构
同 时 它 们 分 别 代 表 了 内 存 中 同 类型 批 量 数 据 存 放 的 两 种 基 本 方 式 , 连 续 存 储 和 随 机 存 储 ( 不 连 续 存 储 )。 

随 机 访 问 就 是 指 可 以 直 接 通 过 开 销 恒 定 的 算 术 运 算 来 得 到 任 一 元 素 的 内 存 地 址 的 访 问 方 法

顺 序 访 问 则 是 指 必 须 从 第 一 个 元 素 开 始 遍 历 , 直 到 找 到 所 需 的 元 素 对 象 为止 ,
而 无 法 直 接 得 到 任 一 中 间 元 素 对 象 的 地址

![](高质量C++读书笔记/Pasted%20image%2020230603052600.png)

stack、queue 及 priorityqueue 在 概 念 和 接 口 上 都 不 支 持 随 机 访 问和 遍历 , 
这 是 由 它 们 的 语 义 决 定 的 , 而 不 是 由 底 层 存 储 方 式 决 定 的 , 因此 没 有 选 代 器
( 所 以 它 们 才 被 叫 做 容 器 近 配 器 而 不 是 归 为 容 器 类 ), 

这 两 种 基 本 的 存 储 方 式 可 以 演 变 出 各 种 不 同 的 存 储 方 式 , 
比 如 分 层 连 续 存 储 、 树 (Tree)、 邻 接 表 、 图 等 , 甚 至 可 以 把 二 者 组 合 起 来 

就 拿 “ 树 “ 来 说 , 它 在 本 质 上 就 是 一 种 特 殊 的 链 表 结 构 , 因 此 只 能 顺 序 访 问 , 
即 从 某 个 节 点 开 始 搜 索 直 至 到 达 所 要 访 问 的 元 素 对 象 , 
或 者 采 用 深 度 优 先 、 广 度 优 先 或 者 前 序 、 中 序 、 后 序 等 方 法 遍 历 整 棵 树 , 
但 是 不 可 能 直 接 定 位 到 树 上 的 任 一 个 结 点 对 象 。

主 要 有 一 叉 搜 索 树 (binary-search)、 平 衡 二 叉 树 (balanced binary search). 红 黑 树 (red-black) 等 

由于 红 黑 树 ( 平 衡 二 又 搜 索 树 的 一 种 ) 在 元素 定 位 上 的 优 异 性 能 (CO(logyW),STL 通 常 使 用 它 来 实 现 关 联 式 容 器 

顺序 容 器 主 要 采 用 向 量 和 链 表 及 其 组 合 作 为 基 本 存 储 结 构 , 如 堆 栈 和 各 种 队 列
而 关 联 式 容 器 采 用 平 衡 二 叉 搜 索 树 作 为 底 层 存 储 绪 构


由 于 顺 序 容 器 本 来 就 有 “ 序 “, 所 以 它 是 通 过 元 素 对 象 在 容 噬 中 的位 置 来标 识 一 个 元 素 的 , 
而 不 是 通 过 元 素 的 值 ( 因 为 它 可 以 存 储 值 相 等 的 多 元 素 对 象 , 而 且 它 们 的 位 置 不 一 定 相 邻 ),
这 也 就 是 调 用 顺 序 容 器 的 insert0 函 数 和 erase0 函 数 时 必 须 指 定 插 入 位 置 和 删 除 位 置
而 不 能 仅 指 定 元 素 值 的 原 因 . 
当 然 , 关 联 式 容 噩 也 能 存 储 值 相 等 的 元 素 , 比 如 multimap 和 multiset 等 , 
但 是 它 们 在 容 器 中 的 位 置 肯 定 是 相 邻 的


![](高质量C++读书笔记/Pasted%20image%2020230603055357.png)


```C++
for (Container:iiterator first = theContainerObj.begin(),
fast = theContainerObj.end(Q);
first != last;
++first) 
{
cout << first->.,. << endl;
cout << (*first)... << endl;
/
template<typename Inputlterator, typename T>

template<typename Inputlterator, typename T>
Inputlterator find(Inputlterator first, Inputfterator last, const T& value)
{
while(first != last && *first != value) ++first;
return first;
}

```

hash table 及 由 它 演 化 出 来 的 hash_set/ hash_map/hash_ multiset/hash_multimap 
作 为 关 联 式 容 磐 加 进来

STL 容 器 采 用 拷 贝 方 式 来 接 收 待 插 入 的 元 素 对 象 一 一 在 插入 的 时 候 容 器
自 动 新 建 等 量 的 元 素 对 象 , 并 用 待 插 入 对 象 依 次 初 始 化 它 们 ( 调 用 拷 贝 构 造 函 数 

在 删 除 元 素 时 , 容 器 负 责 释 放 其 内 存 资 源 ( 对 札 用 随 机 存 储 策 略 的 容 器 ) 
或者 仅 仅调 用 元 素 的 析 构 函 数 ( 对 采 用 连 续 存 储 策 略 的 容 器 


对 象 类 型 一 般 需 要 符 合 下 述 要 求 , 才 能 够 作 为 STL 容 器 的 元 素 。
(1) 可 默 认 构 造 的 。 但 不 是 在 任 何 情 况 下 都 需 要 满 足 这 一 条 , 比 如 关 联 式 容 器 ,
i 对 于 顺 序 容 器 , 除 非 在 初 始 化 的 时 候 需 要 插 入 默 认 构 造 的 若 干 个 对 象 , 或 者 调 用 容
| 器 的 resize0、assign0、insert0等 函 数 的 菜 些 版 本 , 否 则 也 不 需 要 满 足 这 -- 条 。
(2)) 可 拷 贝 构 造 的 。
(3) 可 拷 贝 赋 值 的 ( 但 也 不 是 在 任 何 情 况 下 都 需 要 )。
这 几 条 条 对 基 本 数 据 类 型 及 不 含 指 针 成 员 和 引 用 成 员 的 类 型 都 是 适 用 的 。
(4) 或 者 , 具 有 public的 、 采 用 拷 贝 的 方 式 显 式 定 义 的 拷 贝 构 造 函 数 、 拷 贝 赋
, 值 函 数 和 析 构 函 数 。 这 一 条 适 用 于 含 有 指 针 成 员 或 引 用 成 员 的 对 象 , 
但 模 拟 指 针 ( 例如 迭 代 器 ) 应 该 归 入 前 面 几 条 中 

引 用 不 能 作 为 STL 容 噩 的 元 素 类 型 : 
第 一 , 引用 在创 建 时 必 须 初 始 化 为 一 个 具 体 的 对 氢 , 而 STL 容 器 不 能 满 尸 这 一 要 求 ;
第 二 , 引 用 没 有 构 造 函 数 和 析 构 函 数 , 更 没 有 赋 值 语 义 .

###  迭代器

迭 代 器 是 为 了 降 低 容 器 和 泛 型 算 法 之 间 的 糖 合 性 而 设 计 
指 针 代 表 眠 正 的 内 存 地 址 , 即 对 象 在 内 存 中 的 存 储 位 置 ; 
而 迭 代 器 则 代 表 元 素 在 容 器 中 的 相 对 位 置 
( 当 道 历 容 命 的 时 候 , 关 联 式 容 器 的 元 素 也 就 具 有 了 “ 相 对 位 置 “)。

vector, 没 有 必 要 重 新 定 义 迭 代 器 类 型 , 其 元 素 ! 的 指 针 就 可 以 直 接 充 当 迭 代 器
```c++
typedef T* iterator;
typedef const T* iterator;
```

采 用 不 连 续 存 储 或 其 他 存 储 方 式 的 容 器 , 例 如 ist. deque. set. map 等 , 
则 | 需 定 义 自 己 的 迭 代 器 类 (class), 一 般 情 况 下 它 们 是 对 元 素 指 针 的 封 装 , 即 模 拟 指 针
 
一 些 特 殊 容 器 如 `vector<bool>` 和 `bitset<N>` 等 ,

较 典 型 的 算 法 就 是 distance 和 advance。 这 方 面 的 知 识 涉 及 到 traits 技术
使 用 `vector<int>::iterator`, 而不是`int*` 虽然它们是等价的',

#### 迭代器失效

迭 代 器 失 效 是 指 当 **容 器 底 层 存 储 发 生 变 动** 时 , 原 来 指 向 容 器 中 某 个 或 某 些 元 素 的 迭 代 器 
由 于 元 素 的 存 储 位 置 发 生 了 改 变 而 不 再 指 向 它 们 , 从 而 成 为 无 效 的 迭 代 器 。 |
使 用 无 效 的 迭 代 器 就 像 使 用 无 效 的 指 针 ( 野 指 针 ) 一 样 危 险 。

引 起 容 器 存 储 的 变 动 呢  
主 要 有 : reserve(0、 resize(). push_back(), pop_back(). insert(). erase(). clear() 等 容 器 方 法 
和 一 些 泛 型 算 法 , 如 sort()、copy()、 replace()、remove()、unique(), 
以 及 集 合 操 作 ( 并 、 交 、 差 ) 算 法 等

```C++
#include <iostream>
#include <vector>
using namespace std;
void main()
《
vector<int> ages; / 未 预 留 空 间
ages.push_back(2); / 引 起 内 存 重 分 配
vector<int>::const_iterator p = ages.begin();
for Ginti=0;i< 10; i++) {
ages.push_back(5); / 会 引 起 若 干 次 内 存 重 分 配 操 作
}
. cout << “The first age :“<<*p << endl: /p 已 经 失 效 , 危 险 !
}

void main()
{
//...
vector<int>::const_iterator p = ages.begin();
for (int i= 0; i< 10; i++) {
ages.push_back(5); / 会 引 起 若 干 次 内 存 重 分 配 操 作
}
p = ages.begin(); / 重 新 获 取 迭 代 器
cout << "The first age : “<< *p << endl; 八 ok
}

```

容 量 是 为 了 减 少 那 些 使 用 连 续 空 间 ( 线 性 空 间 》 存 储 元 素 的 容 器 在 增 加 元 素 时
重 新 分 配 内 存 的 次 数 的 一 种 机 制 , 即 当 增 加 元 素 且 剩 余 空 闲 空 间 不 足 时 ,
按照 一 定 比 例 ( 通 常 是 原 来 容 量 的 2 或 1.5 倍 ) 
![](高质量C++读书笔记/Pasted%20image%2020230603075716.png)

```C++
我 们 可 以 从 std::vector<T>的 size0和 capacity0这 两 个
成 员 函 数 的 实 现 上 看 出 容 器 所 辖 元 素 空 间 和 容 量 的 区 别 :
size_type capacity() const{ return (start == 0 ? 0 : end_of_storage - start); }
size_type size() const { return (start == (70: finish - start); }

void reserve(size_type n);
```

(1)) 如 果 n 大 于 容 器 现 有 的 容 量 ( 即 capacity0), 
则 需 要 在 自 由 内 存 区 为 整 个 容 器 重 新 分 配 一 块 新 的 更 大 的 连 续 空 间 , 其 大 小 为 n * sizeof (T), 
然 后 将 容 器 内 所 有有 效 元 素 从 旧 位 置 全 部 拷 贝 到 新 位 置 ( 调 用 拷 贝 构 造 函 数 ), 
最 后 释 放 旧 位 置 的 所 有存 储 空 间 并 调 整 容 器 对 象 的 元 素 位 置 指 示 器 
( 就 是 让 那 三 个 指 针 指 向 新 内 存 区 的 相 应 位 置 )。 
也 就 是 说 , 如 果 请 求 容 量 比 原 有 容 量 大 的 话 , 结 果 是 容 器 的 冗 余 容 量 加 大
(2) <=n 什么都不做

```C++
std::list<int> li;
std::vector<int> vi;
for (int ¢ = 0; ¢ < 10; c++)
lipush_back (c);
vi.reserve(li.size());// 预 留 空 间 , 但 是 并 没 有 改 变 容 器 的 大 小 , 预 留 空 间 未 初 始 化
std::copy (libegin0, liendO vi.begin()); / 拷 贝赋 值
std::copy (vi-begin(), vi.end(), std::ostream_iterator<int>(std::cerr, "\t"));

// 正确使用方法
vi.reserve (li.size()); // 预留 空 间 , 但 是 并 没 有 改 
std::copy(t.beginO, liend(), std::back_inserter(vi));
```

![](高质量C++读书笔记/Pasted%20image%2020230603081048.png)

```C++
void resize(size type n, const T& c = T());
```

其 中 n 就 是 最 后 要 保 持 的 元 素 个 数 , 如 果 需 要 新 增 元 素 的 话 ,c 则 是 新 增 元 素 的 默 认
壹 初 始 值 。 下 面 是 resize()的 实 现 策 略 。
(1) 如 果 n 大 于 容 器 当 前 的 大 小 ( 即 size0), 则 在 容 器 的 末 尾 插 入 ( 追加 ) n 
size0个 初 值 为 c 的 元 素 , 如 果 不 指 定 初 值 , 则 用 元 素 类 型 的 默 认 构 造 函 数 来 初 始 化
每一 个 新 元 素 〔 这 可 能 引 起 内 存 重 分 配 以 及 容 器 容 量 的 扩 张 )。
(2) 如 果 n 小 于 容 器 当 前 的 大 小 , 则 从 容 器 的 末 尾 删 除 size 0 ~ n 个 元 素 , 
但i **不 释 放 元 素 本 身 的 内 存 空 间**, 因 此 容 量 不 变 。
(3) 否 则 , 什 么 也 不 做 

```C++
td::list<int> 。 止 ;
std::vector<int> vi;
for (int c = 0; c < 10; c++)
li.push_back (c);
vi-resize(li.size(); / 调 整 容 器 大 小
std::copy (li-begin(), li.end(), vi-begin()); UMA
std::copy (vi-begin(), vi.end(), std::ostream_iterator<int>(std::cerr, "\t"));
```

![](高质量C++读书笔记/Pasted%20image%2020230603081829.png)

 压 缩 容 器的 多 余 容 量 从 而 节 省 存 储 空 间
 ```C++
 std::vector<int> — vi;
for (int c = 0; c < 10; c++)
vi.push_back (c);
std::vector<int>(vi).swap(vi); W 构 造 一 个 临 时 对 象 , 然 后 与 之 交 换 元 素
```


```C++
示例 17-7 一 个 固 定 容 量 的 循 环 队 
template<typename T /* 元 素 类 型 */ , unsigned int N /* 容 量 */ >
class CyclicQueue {
public:
typedef T value_type;
typedef size_t size_type;
typedef T& reference;
typedef const T& const_reference;
CyclicQueue() : m_popPos(0), m_count(0) {
assert(N > 0);
m_beginPtr = (T*)(::operator new(sizeof(T) * N)); / 分 配 原 始 空 间
} 。
~CyclicQueue() {
_Clear(); // this->_Clear(Q);
operator delete((void*)m_beginPtr);
}
CyclicQueue(const CyclicQueue<T, N>& copy) : m_popPos(0), m_count(0) {
assert(N > 0);
m_beginPtr = (T*)(operator new(sizeof(T) * N)); / 分 配 原 始 空 间
size_t copyPos = copy.m_popPos;
for (size_type idx = 0; idx < copy.m_count; ++idx) 【
_Copy(idx, copy.m_beginPtr[copyPos}); // this->_Copy0Q;
++copyPos; copyPos %= N; ++m_count;
}
}
CyclicQueue& operator=(const CyclicQueue<T, N>& other) {
CyclicQueue<T, N> temp(other); W 调 用 拷 贝 构 造 函 数
swap(temp); [ff this->swapQ);
return (*this);
}
bool is_empty() const { return (m_count == 0); }
bool isfulO const { return (m_count == N); }
value_type front() {
assert(m_count != 0);
return (m_beginPtr[m_popPos});
} .
value_type front() const {
assert(m_count != 0);
return (m_beginPtr[m_popPos]);
}
value_type back() {
assert(m_count != 0);

size_type pushPos = (m_popPos + m_count) % N;
if (pushPos== 0)
return (*(m_beginPtr + N - 1));
return (m_beginPtr[pushPos - 1]);
}
) value_type back() const {
assert(m_count {= 0);
size_type pushPos = (m_popPos + m_count) % N;
if (pushPos == 0)
return (*(m_beginPtr + N - 1);
return (m_beginPtr[pushPos - 1});
}
; bool push(const_reference data = TO) {
| if (m_count <N) t / 不 满 !
size_type pushPos = (m_popPos + m_count) % N;
i _Copy(pushPos, data); /! this->_CopyQ;
++m_count;
return true;
)
) return false;
}
) bool pop(reference data) {
| if (m_count > 0) { 丫 不 宇 !
data = m_beginPtr[m_popPos]; /f operator=
彗 _Destroy(m_popPos); // this->_Destroy(Q);
--m_count; ++m_popPos; m_popPos %= N; / 新 的 pop 位 置
return true;
}
) return false;
}
size_type size() const { return m_count; }
size_type capacity() const { return N; }
void clear() {_Clear(); } / this->_Clear();
void swap(CyclicQueue<T, N>& other) {
std::swap(m_beginPtr, other.m_beginPtr);
std::swap(m_popPos, other.m_popPos);
std::swap(m_count, other.m_count);i|
i
' }
i
ii private:
void Clear() {
for (; m_count > 0; --m_count) {
_Destroy(m_popPos); H this->_Destroy();
| ++m_popPos; m_popPos %= N;
}

m_popPos = 0;
}
void _Destroy(sizetype idx) {
assert(idx < NJ;
T *pTemp = (m_beginPtr + idx);
pTemp->~TQ; W 调 用 析 构 函 数 销 毁 元 素 对 象
}
void _Copy(size_type idx, const_reference data) {
assert(idx < N);
T *pTemp = (m_beginPtr + idx);
new ((void*)pTemp) T(data); // 调 用 placement new 和 拷 贝 构 造 函 数 拷 贝 对 象
| }|
| value_type *m_beginPtr; / 队 列 存 储 空 间 起 始 位 置
size_type m_popPos; / 下 次 pop 位 置
| size_type m_count; / 有 效 元 紫 个 数
};
```

**尽 量 不 要 在 道 历 容 器 的 过 程 中 义 容 器 进 行 插 入 元 素 、 删除 元 素 等 修 改 擎作 ,**
这 和 不 要 在 for 循 环 中 修 改 计 数 器 是 一 个 道 理 , 特 别 是 连 续 存 储 的 容器 中 。 
因 为 这 些 操 作 会 使 一 些 追 代 器 失 效 , 特 别 是 当 前 选 代 器 , 这 在 效果 上 等 价 于 修 改 了 循 环 计 数 器 。 
更 进 一 步 的 原 因 是 下 一 次 迭 代 操 作 , 即++iterator 会 使 用 本 次 选 代 操 作 的 选 代 器 ,
而 当 前 迭 代 器 可 能 已 经 失 效 

虽然 有 些 容 器 如 list, 修 改 操 作 只 会 使 当 前 选 代 器 失 效 , 即 并 不 会 引 起 存储 空 间 重 分 配 , 
所 以 可 以 在 邋 历 的 过 程 中 正 确 地 删 除 当 前 元 素 ( 这 里 面有一 个 技 巧 ), 
但 是 也 最 好 不 要 这 样 做 , 否 则 可 能 存 在 重 大 隐 惠 . 参 见 list等 的 remove(). remove_if() 成 员 函 数 的 实 现 

### 存储分配器

allocator类 是 一 个 模 板 , 作 为 容 器 类 模 板 的 一 个 policy 参 数 , 它 不 仅 与 将 要 为 之
分 配 空 间 的 数 据 对 象 的 类 型 无 关 , 并 且 为 动 态 内 存 的 分 配 和 释 放 提 供 了 面 向 对 象 的
接 口 。 它 是 对 new 运 算 符 的 更 高 层 次 的 抽 象 , 即 隐 藏 了 底 层 的 内 存 模 式 〔 段 内 存 、
! 共享 内 存 、 分 布 式 内 存 等 ), 封 装 了 动 态 内 存 分 配 和 释 放 操 作 , 隐 藏 了 指 针 本 身 的 大
| 小 、 存 储 空 间 重 分 配 模 型 及 内 存 页 大 小 等 细 节 , 提 供 了 更 好 的 可 移 植 性 。

```C++
示例17-8  COM 环 境 下 STL 容 器 的 allocator 实 
template<typename _Ty>
class STLCOMAllocator {
public:
typedef size_t size_type;
typedef ptrdiff_t difference_type;
typedef _Ty value_type;
typedef _Ty* pointer;
typedef const _Ty* const_pointer;
typedef _Ty& reference;
typedef const _Ty& const_reference;
pointer address(reference ref) const { return (&ref); }
const_pointer address(const_reference ref) const { return (&ref); }
_Ty* allocate(size_type n, const void* /* no use */ )
皇 { return (pointer)(::CoTaskMemAlloc(n * sizeof(_Ty))); } // aligned!
void deallocate(void *p, size_type /* no use */ )
{ ::CoTaskMemFree(p); 

void construct(pointer p, const Ty& v) {
if (p != NULL) new ((void*)p)_Ty(v); // placement new & copy constructor
}
void destroy(pointer p) {
if (p != NULL) p->~_TyQ; // destructor
}
size_type max_size() const {
size_type sz = (size_type)(-1) / sizeof(_ Ty);
return (0 < sz? sz: 1);
}
]
template<typename Ty, typename _U> inline
bool __stdcall operator ==(const STLCOMAllocator<_Ty>&,
const STLCOMAllocator<_U>&)
{ return (true); }
template<typename Ty, typename _U> inline
bool __stdcall operator !=(const STLCOMAIlocator<_Ty>&,
const STLCOMAIlocator<_U>&)
{ return (false); 
 ```

### 适配器 容器适配器 stack deque list
```C++
#include <list>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;
void main()
{
list<int> li;
for (int k = 0; k < 10; k+4) {
li.push_back(k);
}
copy(li.begin(), li.endQ, ostream _iterator<int>(cout, " "));
}


list<int> list ;
istream_iterator<int> eos, isiter(cin);
copy(isiter, eos, back_inserter(li));
copy(li.begin(),li.end(), ostream_iterator<int>(cout,""));
```


### 泛型算法

STL 提 供 的 泛 型 算 法 主 要 有 如 下 几 种 ;
> “ 查 找 算 法 , 如 find0、search0、binary search()、find_if0等 。
今 “ 排 序 算 法 , 如 sort)、merge0等 。
从 “ 数 学 计 算 , 如 accumulate0、inner product0、partial sum0 等 。
+ “ 集合 运 算 , 如 set_union0、set_intersection0、includes0等 。
+ “容器 管 理 , 如 copy0、replace0、transform0、remove()、for each() 等 。
> “ 统 计 运 算 , 如 max(0O、min0、count0、max_element0等 。
今 “ 堆 管 理 , 如 make_heap()、push_heap0、pop_ heap(). sort_heap().
> “ 比 较 运 算 , 如 equal0等 

 泛 型 算 法 一 般 接 受 下 列 参 数 类 型 的 一 种 或 几 种 
“ 迭 代 器 , 标 示 容 器 或 区 间 的 范 围 , 以值 传 递 。
从 “ 谓 词 , 返 回 bool 值 的 函 数 对 象 , 指 定 算 法 的 操 作 方 式 , 例 如 find_if0的 第 三
个 参 数 。
从 函 数 对 象 , 用 户 指 定 要 做 的 操 作 , 例 如 for_ each(0)的 第 三 个 参 数 。
今 “ 容器 元 素 , 用 户 指 定 的 基 准 对 象 , 例 如 find0的 第 三 个 参 数 


```C++
Template<typename Inputlterator, typename Outputlterator> inline
Outputlterator copy(Inputlterator first, InputIterator last, Outputlterator oi)
{
for(; first != last; ++first, ++oi)
	*oi = *first;
	return Oi;
}

void main()
{
list<int> i; vector<int> vi;
for (int c = 0; c< 10; c++) li.push_back(c);
vi.resize(li.size());
copy(libeginO, liendO, vi.begin()); 
}

void main()
{
list<int> i; vector<int> vi;
for (int c = 0; c< 10; c++) li.push_back(c);
/ 拷 贝 链 表 元 素 到 vi 中 , 使 用 插 入 迭 代 器
copy(libegin(, li.endQ), back_inserter(vi)); / 调 用 vi.push_back()
}
```

侃 多 泛 型 算 法 总 是 假 定 容 器 的 元 素 类 型 定 义 了 operator-(、operator--0、 operatorl=()、operator<() 或 operator>() 等 函 数 , 因 此 你 有 义 务 为 你 的 家 器 元 素 类 型 定 义 它 们 , 否 则 泛 型 算 法 将 采 用 元 素 类 型 的 默 认 语 义 或 者 报 错。

在 应 用 编 程 时 要 选 用 最 合 适 的 算法
,find0 算 法 的 复 杂 度 为 O(, 而 binary seareh0 算 法 的 复 杂 度 为$O(log{2}N)$当 容 器 中 的 元 素 有 序 时 , 当 然 应 选 用 binary seareh0。

```C++
示例 17-15 基 于 STL 框 架 实 现 的 “ 折 半 “ 查找 算法
template<typename RandomAccessIterator, typename T>
RandomAccessIterator binary_search(RandomAccesslterator first,
RandomAccesslterator last,
const T& value)
RandomAccesslterator mid, not_found = last;
while (first != last) {
mid = first + (last - frst) / 2:; / 注 意 : 不是 (first + last) / 2
if(L(value < *mid) && !(*mid < value))
return mid; /! 调 用 T::operator<()
if (value < *mid)
last = mid; // 调 用 T::operator<()
else first = mid + 1;
}
return not_found;
}
```

### 一些特殊容器
### string 类
### bitset 并非set
```C++
template<size_t Length>
class bitset {
public:
class reference{ ... }; / 可 以 自 动 转 换 为 bool 变 量
// constructors...
Hf accessors...
// rautators...
// convertors...
// statistics...
// test...
I &=, |=, *=, <<=, >>=, >>, <<, ==, !=, ~ overloading...
/ <<(output), >>(input), &, |, ^ friend overloading...
private:
enum {
WORD BITS = sizeof(unsigned long) * 8, / WORD 的 bit 数
N_WORD= ( Length== 0 ? 0 : (Length - 1) /,WORD_BITS )
// Length 个 bit 折 合 多 少 个 WORD
出
unsigned long array[N_WORD + 1]; ABBA ( 不 能 定 义 长 度 为 0 的 数 组 )
};
```

![](高质量C++读书笔记/Pasted%20image%2020230603090456.png)

### `vector<bool>`

![](高质量C++读书笔记/Pasted%20image%2020230603091045.png)

```C++
#include <vector>
#include <iostream>
using namespace std;
void main()
{
typedef vector<bool> BoolVector; // vector<bool>的 具 体 名 字 与 STL 实现 有 关
BoolVector bvect;
bvect.reserve(100);
for (inti = 0; i < 100; i += 2) {
bvect.insert(bvect.end(), true);
bvect.insert(bvect.end(), false);
}
for (BoolVector::iterator first = bvect.begin(),
last = bvect.end(); first != last; ++first) {
cout << *first << endl;
}
cout << bvect.size() << endl;
bvect.front(). flipQ;
bvect.back().flipQ;
bvect.flipQ;
bvect.clear();
}
```

### 空容器

```C++
#include <list>
using namespace std;
void main()
{
list<int> li; / 空 容 器
li.pop_backQ; // runtime error!
if
(!li.empty()) li.erase(li.begin());
}

 例 17-20。 示 例 17-20 
 #include <list> / 顺 序 容 器 
 #include <set> / 联 合 容 器
 #include <iostream> using namespace std; 
 void main() 
 { int *p = new int(100);
  list<int> temp1;
   temp] .insert(temp1.end(), 10);
    list<int> li; W 空 容 器
liinsertdisend0, 10); / 创 建 新 对 象
li.insert(li.endQ, 10); / 创 建 新 对 象
Iierase(temp1l.begin0O); “ 丫 runtime error! 虫 然 li 中 也 有 值 为 10 的 int 元 素
/ 但 是 temp1.begin0指 向 的 元 素 对 象 并 不 属 于 li 所 有
lierase(li-begin(Q)); ok! libegin0指 向 的 对 象 为 ii 所 有
| lierase(*p); W compiling error! 没 有 定 义 这 样 的 方 法 , 对象 *p 不
W 属 于 i 所 有 。 不 能 以 元 素 的 值 来 判 断 其 是 否 属 于 一
W 个 顺 序 容 器 , 因 为 顺 序 容 器 可 以 同 时 存 储 值 完 全 相 同
W 的 多 个 对 象 ; 否 则 容 器 将 不 知 道 该 删 除 哪 一 个 元 素 了
1 set<int> temp2;
| temp2.insert(10);
set<int> si; / 空容 器
siinsert(10); / si 包 含 一 个 值 为 10 的元 素
siinsert(si.end(0), 20);
siinsert(sisend(0), 20); 丫 忽 略 !
si.erase(temp2.beginQ); —_// runtime error! temp2.begin0指 向 的 对 参 不 属 于 si 所 有
si.erase(*p); (OK! 从 si 中 删 除 值 等 于 *p 的元 素
si.erase(si.begin()); OK! sitbegin0 指 向 的 对 象 为 si 所 有
delete p;
}
```

除 了 上 面 这 些 特 殊 容 器 外 , 还 有 一 些 专 用 于 数 学 运 算 的 容 器 、 算 法 和 类 型 , 
如 valarray、 complex 等 , 分 别 定 义 在 头 文 件 `<valarray>和 <complex>` 中 , 
头 文 件 `<numeric>` 中 定 义 了 用 于 向 量 计 算 的 算 法 


往 容 器 中 插 入 元 素 时 , 若 元 素 在 容 器 中 的 顺 序 无 关 紧 要 , 请 尽 量 加 在 最
后 面 。 若 经 常 需 要 在 序 列 容 器 的 开 头 或 中 间 增 加 或 删 除 元 素 时 , 应选 用
list.

当 容 器 作 为 参 数 被 传 递 时 , 请 采 用 引 用 传 递 方 式 。 否 则 将 调 用 容 器 的 拷
贝构 造 函 数 , 其 开 销 是 难 以 想 象 的 

当 元 素 的 有 序 比 搜 索 速 度 更 重 要 时 , 应 选 用 set. multiset.map或 multimap。
否 则 , 选 用 hash_set、hash_multiset、hash_map 或 hash_multimap。


![](高质量C++读书笔记/Pasted%20image%2020230603100246.png)

```C++
char* strcpy(char* strDest, char* strSrc)
{
	assert( (strDest != NULL) && (srtSrc != NULL) );
	char* address = strDest;
	while ( (*strDest++ = *strSrc++) != '\0' );
	return address;
}


class String {
public:
	String(const char *str = ""); //智 通 构 造 函 数
	String(const String &other);  // 拷 贝 构 造 函 数
	~ String(void);
	String& operate =(const String &other);
private:
	char *m_data;
}


 //String 的 普 通 构 造 函 数
String::String(const char *str) / 6 分
{
	if(str == NULL)
	{ 
		m_data = new char[1];
		*m_data = '\0';
	}
	else
	{
		int length = strlen(str);
		m_data = new char[length+1];
		strcpy(m_data, str);
	}
}

// String 的 析 构 函 数
String::~String(void)
{
	delete [] m_data;
}

// 拷 贝 构 造 函 数
String::String(const String &other)
{
	int length = strlen(other.m_data);
	m_data = new char[length+1];
	strcpy(m_data, other.m_data);
}

// 赋值 函 数
String & String::operate =(const String &other)
{

	if (this != &other)
	{
		char *temp = new char[strlen(other.m_data) + 1];
		strcpy(temp, other.m_data);
		
		delete [] m_data;
		m_data = temp;
	}
	return *this;
}

```