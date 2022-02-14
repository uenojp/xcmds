#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 4096

bool recursive = false;

void usage() {
    printf("Usage: cp [SRC] [DEST]\n");
}

struct option const long_options[] = {
    {"recursive", no_argument, NULL, 'r'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
};

int run(int argc, char** argv) {
    int status = 0;
    char* buf[BUF_SIZE];

    FILE* input_file = NULL;
    FILE* output_file = NULL;
    if ((input_file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "cat: %s: %s\n", argv[1], strerror(errno));
        status = 1;
        goto cleanup;
    }
    if ((output_file = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "cat: %s: %s\n", argv[2], strerror(errno));
        status = 1;
        goto cleanup;
    }

    size_t nread;
    while ((nread = fread(buf, 1, BUF_SIZE, input_file)) > 0) {
        fwrite(buf, 1, nread, output_file);
    }

cleanup:
    if (input_file != NULL)
        fclose(input_file);
    if (output_file != NULL)
        fclose(output_file);

    return status;
}

int copy_file_to_file(int argc, char** argv) {
    int status = 0;
    char* buf[BUF_SIZE];

    FILE* input_file = NULL;
    FILE* output_file = NULL;
    if ((input_file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "cat: %s: %s\n", argv[1], strerror(errno));
        status = 1;
        goto cleanup;
    }
    if ((output_file = fopen(argv[2], "w+")) == NULL) {
        fprintf(stderr, "cat: %s: %s\n", argv[2], strerror(errno));
        status = 1;
        goto cleanup;
    }

    size_t nread;
    while ((nread = fread(buf, 1, BUF_SIZE, input_file)) > 0) {
        fwrite(buf, 1, nread, output_file);
    }

cleanup:
    if (input_file != NULL)
        fclose(input_file);
    if (output_file != NULL)
        fclose(output_file);

    return status;
}

/* cp (file|dir)+ dir */
int copy_to_dir(int argc, char** argv) {
    ;
}

int main(int argc, char** argv) {
    int opt;
    while ((opt = getopt_long(argc, argv, "rh", long_options, NULL)) != -1) {
        switch (opt) {
            case 'r':
                recursive = true;
                break;
            case 'h':
                usage();
                exit(0);
                break;
            default:
                fprintf(stderr, "Try \'cp --help\' for more inforamtion\n");
                exit(1);
        }
    }

    int status = 0;

    /* cp (file|dir){0,1} */
    if (argc < 3) {
        fprintf(stderr, "cp: missing file\n");
        exit(1);
    }

    /* cp file (file|dir) */
    if (argc == 3) {
        struct stat src;
        struct stat dest;
        stat(argv[1], &src);
        stat(argv[2], &dest);
        if (S_ISREG(src.st_mode) || S_ISCHR(src.st_mode)) {
            if (S_ISDIR(dest.st_mode)) {
                status = copy_to_dir(argc, argv);
            } else {
                status = copy_file_to_file(argc, argv);
            }
        }
    }
    /* cp (file|dir){2,} file */
    // if (argc > 3 && !S_ISDIR(s.st_mode)) {
    //     struct stat s;
    //     stat(argv[argc - 1], &s);
    //     fprintf(stderr, "cp: \'%s\' is not a directory\n", argv[argc - 1]);
    //     exit(1);
    // }

    return status;
}
