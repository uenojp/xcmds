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

    int i_arg = 1;
    do {
        if (argc == 1 || !strcmp(argv[i_arg], "-"))
            fd = STDIN_FILENO;
        else
            fd = open(argv[i_arg], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "cat: %s: %s\n", argv[i_arg], strerror(errno));
            continue;
        }

        while ((nread = read(fd, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, nread);
        }

        close(fd);
    } while (++i_arg < argc);

    return 0;
}
