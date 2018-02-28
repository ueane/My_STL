#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#define ERROR 0
#define OK 1

#define swap(a, b) { \
    __typeof(a) __tmpe = (a); \
    (a) = (b); (b) = __tmpe; \
}

typedef struct pQueue {
    int *data;
    int size, length;
}pQueue;

int cmp_value(int a, int b) {
    if (a > b) return 1;
    return 0;
}

void init(pQueue *pq, int n) {
    pq->data = (int *)malloc(sizeof(int) * n);
    pq->size = 0;
    pq->length = n;
}

int empty(pQueue *pq) {
    return pq->size == 0;
}

int frount(pQueue *pq) {
    return pq->data[0];
}

int push(pQueue *pq, int value) {
    if (pq->size >= pq->length) {
        return ERROR;
    }
    pq->data[pq->size] = value;
    int c = pq->size, f;
    f = (c - 1) / 2;
    while (cmp_value(pq->data[c], pq->data[f])) {
        swap(pq->data[c], pq->data[f]);
        c = f;
        f = (c - 1) / 2;
    }
    pq->size++;
    return OK;
}

int pop(pQueue *pq) {
    if (empty(pq)) {
        return ERROR;
    }
    swap(pq->data[pq->size - 1], pq->data[0]);
    pq->size--;
    int i = -1, l, r, max_index = 0;
    while (max_index != i) {
        i = max_index;
        l = 2 * i + 1;
        r = 2 * i + 2;
        if (l < pq->size && cmp_value(pq->data[l], pq->data[max_index])) {
            max_index = l;
        }
        if (r < pq->size && cmp_value(pq->data[r], pq->data[max_index])) {
            max_index = r;
        }
        if (max_index != i) {
            swap(pq->data[i], pq->data[max_index]);
        }
    }
    return OK;
}


void clear(pQueue *pq) {
    free(pq->data);
    free(pq);
}

int main () {
    int n, a;
    scanf("%d", &n);
    pQueue *pq = (pQueue *)malloc(sizeof(pQueue));
    init(pq, n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a);
        push(pq, a);
    }
   // printf("pq->size = %d\n", pq->size);
    while (!empty(pq)) {
        printf("%d ", frount(pq));
        pop(pq);
    }
    printf("\n");
    clear(pq);
    return 0;
}
