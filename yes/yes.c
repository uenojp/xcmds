#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// join_argv concatenates the strings argv with exception of filename,
// and returns a pointer to the concatenated string.
char* join_argv(int argc, char** argv, char* sep) {
    if (argc == 1)
        return NULL;

    size_t len = 0;
    size_t nargs = argc - 1;
    for (int i = 1; i < argc; i++) {
        len += strlen(argv[i]);
    }
    len += (nargs - 1) * strlen(sep);
    len++;  // For '\0'

    char* res = malloc(len * sizeof(char));
    if (res == NULL)
        return NULL;
    memset(res, '\0', len);

    for (int i = 1; i < argc; i++) {
        strncat(res, argv[i], strlen(argv[i]));
        if (i < argc - 1)
            strncat(res, sep, strlen(sep));
    }
    return res;
}

int main(int argc, char** argv) {
    char* msg;

    if (argc == 1) {
        msg = "y";
    } else {
        msg = join_argv(argc, argv, " ");
        if (!msg)
            perror("yes");
    }

    for (;;) {
        printf("%s\n", msg);
    }

    free(msg);
    return 0;
}
