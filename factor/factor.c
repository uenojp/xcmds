#include <errno.h>
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

/* factorize stores prime factors of n into *factors_ptr(factors)
   and returns the array size */
long factorize(long** factors_ptr, size_t n) {
    if ((long long)n < 0) {
        fprintf(stderr, "factor: ‘%ld’ is not a valid positive integer\n", n);
        return -1;
    }

    long* factors;
    size_t num = n;
    size_t len = 0;
    size_t cap = 10;

    *factors_ptr = (long*)malloc(cap * sizeof(long));
    if (!*factors_ptr) {
        return -1;
    }
    factors = *factors_ptr;

    for (long i = 2; i * i <= n; i++) {
        while (num % i == 0) {
            if (cap <= len + 1) {
                /* +1 for factors[len++] = num */
                cap = cap * 2 + 1;
                factors = (long*)xrealloc(factors, cap * sizeof(long));
            }
            factors[len++] = i;
            num /= i;
        }
    }
    if (num != 1 && num != 0)
        factors[len++] = num;

    return len;
}

static int cmpl(const void* p1, const void* p2) {
    if (*(const long*)p1 < *(const long*)p2)
        return -1;
    if (*(const long*)p1 > *(const long*)p2)
        return 1;
    return 0;
}

int main(int argc, char** argv) {
    long n;
    long* factors = NULL;
    size_t size; /* factors's size */

    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    char* nptr;

    int iargs = 0;
    while (1) {
        /* TODO: allow numbers separated by spaces or tabs
                 (trim spaces, tabs and newlines)
           e.g.
           echo -e '57 99\t33\t\t99\n88' | ./factor
           echo -e '     59   \t  ' | ./factor

           TODO: handle big numbers */
        if (argc == 1) {
            nread = getline(&line, &len, stdin);
            if (nread == -1)
                break;
            if (line[strlen(line) - 1] == '\n')
                line[strlen(line) - 1] = '\0';
            nptr = line;
        } else {
            nptr = argv[++iargs];
            if (iargs >= argc)
                break;
        }

        char* endptr;
        int base = 10;
        n = strtol(nptr, &endptr, base);
        if (errno != 0 || *endptr != '\0') {
            fprintf(stderr, "factor: ‘%s’ is not a valid positive integer\n", nptr);
            continue;
        }
        nptr = NULL;

        if ((size = factorize(&factors, n)) == -1) {
            return -1;
        }

        qsort(factors, size, sizeof(long), cmpl);

        printf("%ld:", n);
        for (int i = 0; i < size; i++) {
            printf("% ld", factors[i]);
        }
        puts("");
    }

    free(line);
    free(factors);
    return 0;
}
