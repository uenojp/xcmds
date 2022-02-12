#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 4096

void usage() {
    printf("Usage: cp SRC DEST\n");
}

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

int main(int argc, char** argv) {
    if (argc != 3) {
        usage();
        exit(1);
    }

    return run(argc, argv);
}
