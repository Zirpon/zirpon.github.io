#include <stdio.h>
int dis[7], book[7] = {0}; // bookB A aR MET BA BOA RAE
int h[7], pos[7], size;    // b 用 晥 保 存 堆 ,pos 用 晥 存 傅 每 个 顶 点 在 堆 中 的 位 繮 ,size 为 堆 的 大 小
// 交 换函数 , 用 米 交 换 堆 中 的 两 个 元 素 的 值
void swap(int x, int y)
{
    int t;
    t = h[x];
    h[x] = h[y];
    h[y] = t;
    // 同 步 更 新 pos
    t = pos[h[x]];
    pos[h[x]] = pos[h[y]];
    pos[h[y]] = t;
}
// 向 下 调 整 函 数
void siftdown(int i) // 优 入 一 个 霁 要 向 下 调 整 的 结 点 縖 号
{
    int t, flag = 0; // 5lag 用 晥 朇 记 是 否 需 要 继 续 向 下 调 整
    while (i * 2 <= size && flag == 0)
    {
        // 比 较 i 和 它 左 儿 于 i42 在 ais 中 的 值 , 干 用 t 记 录 值 轼 小 的 结 点 縖 号
        if (dis[h[i]] > dis[h[i * 2]])
            t = i * 2;
        else
            t = i;
        // 加 暜 它 昉 右 儿 子 , 再 对 昉 儿 二 进 行 讨 论
        if (i * 2 + 1 <= size)
        {
            // 加 昨 昉 儿 子 的 值 更 小 , 更 新 较 小 的 结 点 縖号
            if (dis[h[t]] > dis[h[i * 2 + 1]])
                t = i * 2 + 1;
        }
        // 如 概 发 现 昀 小 的 结 点 縖 号 不 是 自 己 , 说 明 子 结 点 中 昉 比 父 结 点 更 尔 的
        if (t != i)
        {
            swap(t, i); // 交 换 它 们
                i = t; // 更 新 ; 为 刚 才 与 它 交 换 的 儿 孔 结 点 的 縖 号 , 便 于 接 下 米 继 续 向 下 调 整
        }
        else
            flag = 1; // 则 说 明 当 前 的 文 结 点 已 经 比 两 个 孔 结 点 都 要 小 了 , 不 霜 要 彷 进 行 调 孤 了
    }
}
void siftup(int i) // 传 入 一 个 需 要 向 上 调 整 的 结 点 縖 号
{
    int flag = 0; // 用 江朇 记 是 否 需 要 继 续 吊 上 调 整
    if (i == 1)
        return; // 如 暜 是 堆 顶 , 就 返 回 , 不 霜 要 调 驹 了
    // 不 在 堆 顶 , 并 东 当 前 结 点 3 的 值 比 文 结 点 小 的 时 候 继 续 向 上 调 整
    while (i != 1 && flag == 0)
    {

        // 判 断 是 否 比 父 结 点 的 小
        if (dis[h[i]] < dis[h[i / 2]])
            swap(i, i / 2); // RREME EEE
        else
            flag = 1; // RAGA Hi SRE TAYE SC EEK
        i = i / 2;    // 这 句 话 很 重 要 , 更 新 縖 号 { 为 它 父 结 点 的 縖 号 , 从 而 便 于 下 一 次 继 续 吊 上 调 整
    }
}
// 从 堆 顶 取 出 一 个 元 素
int pop()
{
    int t;
    t = h[1];       // 用 一 个 临 时 变 量 记 录 堆 顶 点 的 值
    pos[t] = 0;     // 其 实 这 句 要 不 要 无 所 谓
    h[1] = h[size]; // 将 堆 的 昀 后 一 个 点 赔 值 到 绳 项
    pos[h[1]] = 1;
    size--;      // 堆 的 元 素 凑 少 1
    siftdown(1); // 向 下 调 整
    return t;    // 返 回 之 前 记 录 的 蜡 项 点
}

int main()
{
    int n, m, i, j, k;
    // Vs w 和 next 的 数 组 大 小 要 朹 据 实 际 情 况 晥 设 繮 , 此 图 是 无 向 图 , 要 比 2 *m 的 昀 大 值 要 大 1
    // #izst 要 比 n 的 昀 大 值 要 大 1, 要 比 24m 的 昀 大 值 要 大 1
    int u[19], v[19], w[19], first[7], next[19];
    int inf = 99999999;     // 用 inf 《infini y 的 縩 冗 》 存 傅 一 个 我 们 认 为 的 正 无 穷 值
    int count = 0, sum = 0; // count 用 朗 记 录 生 成 朑 中 顶 点 的 个 数 ,sum 用 晥 存 借 路 往 之 和
    // 读 入 nm,n 表 示 顶 店 个 数 ,m 表 示 边 的 晡 数
    scanf("%d %d", &n, &m);

    // 读 入 边
    for (i = 1; i <= m; i++)
        scanf("%d %d %d", &u[i], &v[i], &w[i]);
    // 这 里 是 无 向 图 , 所 以 需 要 将 所 昉 的 边 再 反 向 存 倩 一 次
    for (i = m + 1; i <= 2 * m; i++)
    {
        u[i] = v[i - m];
        v[i] = u[i - m];
        w[i] = w[i - m];
    }
    // 开 始使 用 邻 接 表 存 傅 边
    for (i = 1; i <= n; i++)
        first[i] = -1;
    for (i = 1; i <= 2 * m; i++)
    {
        next[i] = first[u[i]];
        first[u[i]] = i;
    }
    // 2rim 朸 心部分 开 始
    //  将 2 号 顶 点 加 入 生 或 朑
    book[1] = 1; // 这 里 用 book 暗 朇 记 一 个 项 点 已 经 加 入 生 成 朑
    count++;
    // 初 始 化 ais 数 组 , 这 里 是 1 号 顶 点 到 其 余 各 个 顶 点 的 初 始 跌 高
    dis[1] = 0;
    for (i = 2; i <= n; i++)
        dis[i] = inf;
    k = first[1];
    while (k != -1)
    {
        dis[v[k]] = w[k];
        k = next[k];
    }
    // 初 始 化 堆
    size = n;
    for (i = 1; i <= size; i++)
    {
        h[i] = i;
        pos[i] = i;
    }
    for (i = size / 2; i >= 1; i--)
        siftdown(i);

    pop(); // 先 弹 出 一 个 堆 顶 元 素 , 因 为 此 时 堆 顶 是 1 号 顶 点
    while (count < n)
    {
        j = pop();
        book[j] = 1;
        count++;
        sum = sum + dis[j];
        k = first[j];
        while (k != -1)
        {
            if (book[v[k]] == 0 && dis[v[k]] > w[k])
            {
                dis[v[k]] = w[k];
                siftup(pos[v[k]]);
            }
            k = next[k];
        }
    }
    printf("%d", sum);
    getchar();
    getchar();
    return 0;
}