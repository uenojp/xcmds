#define _GNU_SOURCE
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    FILE* file = fopen("/proc/cpuinfo", "r");
    if (!file) {
        fprintf(stderr, "nproc: fopen: %s\n", strerror(errno));
        return 1;
    }

    long nproc;
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    while ((nread = getline(&line, &len, file)) != -1) {
        if (strstr(line, "processor")) {
            /* In /proc/cpuinfo, CPU information is stored as following
                processor : <n>
               then, len("processor : ") == 12 */
            char* proc = line + 12;

            int base = 10;
            char* endptr;
            nproc = strtol(proc, &endptr, base);
            if (errno != 0) {
                perror("nproc");
                return 1;
            }
        }
    }

    printf("%ld\n", nproc + 1);

    return 0;
}