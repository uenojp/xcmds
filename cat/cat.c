#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 4096

bool number = false;

void usage() {
    printf(
        "Usage: cat [OPTION] [FILE]...\n"
        "\n"
        "    -n, --number    number all output lines\n"
        "    -h, --help      display this help and exit\n");
}

struct option const long_options[] = {
    {"number", no_argument, NULL, 'n'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
};

int run(int argc, char** argv) {
    FILE* file = NULL;
    ssize_t nread;
    int lineno = 1;
    int status = 0;

    int iargs = optind;
    do {
        if (argc == 1 || !strcmp(argv[iargs], "-")) {
            file = stdin;
        } else {
            file = fopen(argv[iargs], "r");
        }
        if (file == NULL) {
            fprintf(stderr, "cat: %s: %s\n", argv[iargs], strerror(errno));
            status = 1;
            continue;
        }

        char* line = NULL;
        size_t n = 0;
        while ((nread = getline(&line, &n, file)) > 0) {
            if (number) {
                printf("%6d\t", lineno++);
            }
            fwrite(line, 1, nread, stdout);
        }

        fclose(file);
    } while (++iargs < argc);

    return status;
}

int main(int argc, char** argv) {
    int opt;
    while ((opt = getopt_long(argc, argv, "nh", long_options, NULL)) != -1) {
        switch (opt) {
            case 'n':
                number = true;
                break;
            case 'h':
                usage();
                exit(0);
                break;
            default:
                fprintf(stderr, "Try \'cat --help\' for more inforamtion.\n");
                exit(1);
        }
    }

    return run(argc, argv);
}
