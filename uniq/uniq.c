#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Let argv[2] be OUTPUT */
int main(int argc, char **argv) {
    if (argc > 3) {
        fprintf(stderr, "uniq: extra operand ‘%s’\n", argv[2]);
        return 1;
    }

    FILE *file;
    char *line = NULL;
    char *prev_line = NULL;
    size_t len = 0;
    ssize_t nread;

    if (argc == 1 || !strcmp(argv[1], "-")) {
        file = stdin;
    } else {
        file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "uniq: %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
    }

    int first = 1;
    while ((nread = getline(&line, &len, file)) != -1) {
        if (first || strcmp(prev_line, line)) {
            printf("%s", line);
            first = 0;
        }
        /* memory leak */
        // prev_line = malloc((strlen(line) + 1) * sizeof(char));
        char *tmp = realloc(prev_line, (strlen(line) + 1) * sizeof(char));
        if (!tmp) {
            fprintf(stderr, "uniq: realloc: %s\n", strerror(errno));
            free(prev_line);
            return 1;
        } else {
            prev_line = tmp;
            tmp = NULL;
        }
        strncpy(prev_line, line, strlen(line));
    }

    free(prev_line);
    free(line);
    if (argc == 1 || !strcmp(argv[1], "-")) {
        ;
    } else {
        fclose(file);
    }

    return 0;
}