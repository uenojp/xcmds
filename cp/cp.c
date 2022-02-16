#include <errno.h>
#include <getopt.h>
#include <libgen.h>
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

/* Copy file to file.
 *
 * Intention: argc == 3
 *          : argv[1] and argv[2] are files
 *
 * TODO:
 *  - support character file(e.g. cp /etc/stdin foo) */
int copy_file_to_file(int argc, char** argv) {
    int status = 0;

    char* buf[BUF_SIZE];

    FILE* ifile = NULL;
    FILE* ofile = NULL;
    if ((ifile = fopen(argv[optind], "r")) == NULL) {
        fprintf(stderr, "cat: %s: %s\n", argv[1], strerror(errno));
        status = 1;
        goto cleanup;
    }
    if ((ofile = fopen(argv[optind + 1], "w+")) == NULL) {
        fprintf(stderr, "cat: %s: %s\n", argv[2], strerror(errno));
        status = 1;
        goto cleanup;
    }

    size_t nread;
    while ((nread = fread(buf, 1, BUF_SIZE, ifile)) > 0) {
        fwrite(buf, 1, nread, ofile);
    }

cleanup:
    if (ifile != NULL)
        fclose(ifile);
    if (ofile != NULL)
        fclose(ofile);

    return status;
}

/* Copy some files to a directory.
 * e.g. cp file1 file2 file3 dir/
 *
 * Intention: argv[i] is an existent file, for all optind < i < argc - 1
 *          : argv[argc - 1] is a directory */
int copy_files_to_dir(int argc, char** argv) {
    int status = 0;

    char buf[BUF_SIZE];

    FILE* ifile;
    FILE* ofile;
    for (int i = optind; i < argc - 1; i++) {
        ifile = NULL;
        ofile = NULL;

        /* Concatenate the directory name and the file name
         * output path := dir + "/" + file
         *  where dir := argv[argc-1], file := basename(argv[i])
         *
         * `dir` may contain trailing slashes, but this is not a problem
         * because multiple shashes are interpreted as a single slash.
         * ref. https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_271 */
        const char* base = basename(argv[i]);
        size_t olen = strlen(argv[argc - 1]) + strlen("/") + strlen(base) + 1;
        char opath[olen];
        memset(opath, 0, olen);
        strncat(opath, argv[argc - 1], strlen(argv[argc - 1]) + 1);
        strncat(opath, "/", strlen("/") + 1);
        strncat(opath, base, strlen(base) + 1);

        ifile = fopen(argv[i], "r");
        if (ifile == NULL) {
            fprintf(stderr, "cp: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            goto cleanup;
        }
        ofile = fopen(opath, "w+");
        if (ofile == NULL) {
            fprintf(stderr, "cp: %s: %s\n", opath, strerror(errno));
            status = 1;
            goto cleanup;
        }

        size_t nread;
        while ((nread = fread(buf, 1, BUF_SIZE, ifile)) > 0) {
            fwrite(buf, 1, nread, ofile);
        }

    cleanup:
        if (ifile != NULL)
            fclose(ifile);
        if (ofile != NULL)
            fclose(ofile);
    }
    return status;
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

    /* Assume no options for now. */

    int status = 0;

    if (argc < 3) {
        fprintf(stderr, "cp: missing file\n");
        exit(1);
    }

    /* Copy file to file or file to directory. */
    if (argc == 3) {
        struct stat src;
        struct stat dest;
        stat(argv[1], &src);
        stat(argv[2], &dest);
        if (S_ISREG(src.st_mode) /* || S_ISCHR(src.st_mode) */) {
            if (S_ISDIR(dest.st_mode)) {
                /* Copy files to directory.
                 * e.g. cp file dir/ */
                status = copy_files_to_dir(argc, argv);
            } else {
                /* Copy file to file.
                 * e.g. cp file1 file2, cp file dir/file */
                status = copy_file_to_file(argc, argv);
            }
        } else {
            fprintf(stderr, "cp: %s is not a regular file\n", argv[1]);
            status = 1;
        }
    }

    /* Copy some files to directory. */
    if (argc > 3) {
        struct stat dest;
        stat(argv[argc - 1], &dest);
        if (S_ISDIR(dest.st_mode)) {
            copy_files_to_dir(argc, argv);
        } else {
            fprintf(stderr, "cp: %s is not a directory\n", argv[argc - 1]);
            status = 1;
        }
    }

    return status;
}
