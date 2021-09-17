#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* substr(char* src, size_t offset, size_t len) {
    char* res = malloc((len + 1) * sizeof(char));
    if (substr == NULL)
        return NULL;
    strncpy(res, src + offset, len);
    res[len] = '\0';
    return res;
}

/* TODO: concatenates strings between buffers (perhaps using strncat) */
/*
    if buf[5],
    $ echo '12345678' | ./nl
    > 1 12345
    > 2 678
*/

/*
    0 1 2 3  4 5 6 7  8
    a b c \n d e f \n g
    |      | |      |
    off    i |      |
             |      |
            off     i
            <-len->
    len = i - off
*/

int main(int argc, char** argv) {
    char buf[4096];
    memset(buf, '\0', sizeof(buf));
    size_t nread;
    size_t offset;
    FILE* file;
    char* line;
    int lineno = 1;
    int iargs = 1;

    do {
        if (argc == 1 || !strcmp(argv[iargs], "-"))
            file = stdin;
        else {
            if ((file = fopen(argv[iargs], "r")) == NULL)
                return 1;
        }
        while ((nread = fread(buf, sizeof(char), sizeof(buf), file))) {
            offset = 0;
            for (int i = 0; i <= nread; i++) {
                if (buf[i] == '\n' || buf[i] == '\0') {
                    line = substr(buf, offset, i - offset);
                    if (line == NULL)
                        perror("nl");
                    /* if the line is only a newline,
                        substr sets line[0] to \0 due to len = off-i = 0
                        /usr/bin/nl print
                            7 spaces                    if only newline
                            <lineno(6char)><tab><line>  otherwise */
                    if (line[0] == '\0') {
                        /* no print newline if EOF */
                        if (buf[i] != '\0')
                            printf("%*s\n", 7, "");
                    } else {
                        printf("%6d\t%s\n", lineno++, line);
                    }

                    offset = i + 1;
                    free(line);
                }
            }
        }
    } while (++iargs < argc);

    return 0;
}