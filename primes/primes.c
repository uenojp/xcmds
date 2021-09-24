#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ref. https://github.com/torvalds/linux/blob/master/scripts/kconfig/util.c */
void* xrealloc(void* p, size_t size) {
    p = realloc(p, size);
    if (p)
        return p;
    fprintf(stderr, "realloc: %s\n", strerror(errno));
    exit(1);
}

/* primes stores primes into *primes_ptr(primes)
   and returns the array size
   By Eratosthenes sieve: O(nloglogn) */
int primes(int** primes_ptr, int n) {
    if (n < 0) {
        fprintf(stderr, "primes: negative numbers aren't permitted\n");
        return -1;
    }
    if (n >= INT_MAX) {
        fprintf(stderr, "primes: out of range\n");
        return -1;
    }
    int* table = (int*)malloc((n + 1) * sizeof(int));
    if (!table) {
        perror("primes");
        return -1;
    }
    for (int i = 0; i < n + 1; i++)
        table[i] = 1;

    unsigned int len = 0;
    unsigned int cap = 10;
    *primes_ptr = (int*)malloc(cap * sizeof(int));
    if (!*primes_ptr) {
        perror("primes");
        return -1;
    }
    int* primes = *primes_ptr;

    for (int i = 2; i <= n; i++) {
        if (!table[i])
            continue;
        if (cap <= len + 1) {
            cap *= 2;
            primes = (int*)xrealloc(primes, cap * sizeof(int));
        }
        primes[len++] = i;
        for (int j = i * 2; j <= n; j += i) {
            table[j] = 0;
        }
    }

    free(table);
    return len;
}

int main(int argc, char** argv) {
    int* primes_list = NULL;
    int nprimes = primes(&primes_list, 1000);
    if (nprimes == -1 || primes_list == NULL)
        return 1;

    assert(nprimes == 168);

    free(primes_list);
    return 0;
}