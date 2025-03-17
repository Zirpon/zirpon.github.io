#include <stdio.h>

int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
void swap (int i, int j) {
    int temp = arr[i];
    arr[j] = arr[i];
    arr[i] = temp;
}

void shiftdown(int i, int n) {
    int t = i;
    if (i * 2 <= n && arr[i] < arr[i * 2]) {
        t = i * 2;
    }
    if (i * 2 + 1 <= n && arr[t] < arr[i * 2 + 1]) {
        t = i * 2 + 1;
    }
    if (t != i) {
        swap(i, t);
        shiftdown(t, n);
    }
}

void create_heap(int n) {
    for (int i = n / 2; i >= 1; i--) {
        shiftdown(i, n);
    }
}

void heap_sort(int n) {
    for (int i = n; i > 1; i--) {
        swap(1, i);
        shiftdown(1, i - 1);
    }
}

int main() {
    printf("Hello, World!\n");
    return 0;
}