# Day1_part1.c
c programming assignment 1
```c
#include <stdio.h>
#include <stdlib.h>

#define MAXN 10000  // adjust based on input size

// comparison function for qsort
int cmp(const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    int left[MAXN], right[MAXN];
    int n = 0;

    // Read pairs of integers until EOF
    while (scanf("%d %d", &left[n], &right[n]) == 2) {
        n++;
    }

    // Sort both lists
    qsort(left, n, sizeof(int), cmp);
    qsort(right, n, sizeof(int), cmp);

    long long total = 0;
    for (int i = 0; i < n; i++) {
        int diff = left[i] - right[i];
        if (diff < 0) diff = -diff;
        total += diff;
    }

    printf("%lld\n", total);
    return 0;
}
```
