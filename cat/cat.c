#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
    char buf[4096];
    int fd;
    ssize_t nread;

    int iargs = 1;
    do {
        if (argc == 1 || !strcmp(argv[iargs], "-"))
            fd = STDIN_FILENO;
        else
            fd = open(argv[iargs], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "cat: %s: %s\n", argv[iargs], strerror(errno));
            continue;
        }

        while ((nread = read(fd, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, nread);
        }

        close(fd);
    } while (++iargs < argc);

    return 0;
}
