#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc, char** argv) {
    char buf[BUF_SIZE];
    FILE* file = NULL;
    size_t nread;

    int iargs = 1;
    do {
        if (argc == 1 || !strcmp(argv[iargs], "-")) {
            file = stdin;
        } else {
            file = fopen(argv[iargs], "r");
        }
        if (file == NULL) {
            fprintf(stderr, "cat: %s: %s\n", argv[iargs], strerror(errno));
            continue;
        }

        while ((nread = fread(buf, 1, BUF_SIZE, file)) > 0) {
            fwrite(buf, 1, nread, stdout);
        }

        fclose(file);
    } while (++iargs < argc);

    return 0;
}
