import heapq

def swap(h, pos, x, y):
    h[x], h[y] = h[y], h[x]
    pos[h[x]], pos[h[y]] = pos[h[y]], pos[h[x]]

def siftdown(h, dis, pos, size, i):
    flag = 0
    while i * 2 <= size and flag == 0:
        if dis[h[i]] > dis[h[i * 2]]:
            t = i * 2
        else:
            t = i
        if i * 2 + 1 <= size and dis[h[t]] > dis[h[i * 2 + 1]]:
            t = i * 2 + 1
        if t != i:
            swap(h, pos, t, i)
            i = t
        else:
            flag = 1

def siftup(h, dis, pos, i):
    flag = 0
    while i != 1 and flag == 0:
        if dis[h[i]] < dis[h[i // 2]]:
            swap(h, pos, i, i // 2)
        else:
            flag = 1
        i = i // 2

def pop(h, dis, pos, size):
    t = h[1]
    pos[t] = 0
    h[1] = h[size]
    h[size] = -1
    pos[h[1]] = 1
    size -= 1
    siftdown(h, dis, pos, size, 1)
    return t, size

data = [[2,4,11],[3,5,13],[4,6,3],[5,6,4],[2,3,6],[4,5,7],[1,2,1],[3,4,9],[1,3,2]]

def main():
    inf = float('inf')
    n, m = 6,9
    u, v, w = [0] * (2 * m + 1), [0] * (2 * m + 1), [0] * (2 * m + 1)
    first, next = [-1] * (n + 1), [0] * (2 * m + 1)
    dis, book = [inf] * (n + 1), [0] * (n + 1)
    h, pos = [0] * (n + 1), [0] * (n + 1)

    for i in range(1, m + 1):
        u[i], v[i], w[i] = data[i - 1]
    for i in range(m + 1, 2 * m + 1):
        u[i], v[i], w[i] = v[i - m], u[i - m], w[i - m]
    for i in range(1, n + 1):
        first[i] = -1
    for i in range(1, 2 * m + 1):
        next[i] = first[u[i]]
        first[u[i]] = i

    book[1] = 1
    count, sum = 1, 0
    dis[1] = 0
    for i in range(2, n + 1):
        dis[i] = inf
    k = first[1]
    while k != -1:
        dis[v[k]] = w[k]
        k = next[k]

    print(dis)

    size = n
    for i in range(1, size + 1):
        h[i] = i
        pos[i] = i
    for i in range(size // 2, 0, -1):
        siftdown(h, dis, pos, size, i)
        print(size, i,h, pos)

    _, size = pop(h, dis, pos, size)
    print(h, pos)
    print('========================================')
    while count < n:
        j, size = pop(h, dis, pos, size)
        print('while pop',h[1:], pos[1:], dis[1:], j, size)

        book[j] = 1
        count += 1
        sum += dis[j]
        k = first[j]
        while k != -1:
            if book[v[k]] == 0 and dis[v[k]] > w[k]:
                dis[v[k]] = w[k]
                siftup(h, dis, pos, pos[v[k]])
                print('while siftup', h[1:], pos[1:], dis[1:], j, size)
            
            k = next[k]

    print(sum)

if __name__ == "__main__":
    main()