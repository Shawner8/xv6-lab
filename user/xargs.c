#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) {
    char *args[MAXARG + 1];
    int i;
    for (i = 0; i < argc - 1; ++i) {
        args[i] = argv[i + 1];
    }
    int start = i;
    char buf[512];
    char chr;
    char *p = buf;
    while (read(0, &chr, sizeof chr) != 0) {
        // printf("%c\n", chr);
        if (chr == ' ') {
            if (p == buf) {
                continue;
            } else {
                *p = 0;
                p = buf;
                char *p_str = malloc(sizeof buf);
                strcpy(p_str, buf);
                args[i] = p_str;
                // printf("%s\n", args[i]);
                ++i;
            }
        } else if (chr == '\n') {
            if (p != buf) {
                *p = 0;
                p = buf;
                char *p_str = malloc(strlen(buf) + 1);
                strcpy(p_str, buf);
                args[i] = p_str;
                // printf("%s\n", args[i]);
                ++i;
            }
            args[i] = 0;
            if (fork() == 0) {
                // int j = 0;
                // while (args[j]) {
                //     printf("%s\n", args[j]);
                //     ++j;
                // }
                exec(args[0], args);
            } else {
                wait(0);
                while (i > start) {
                    --i;
                    free(args[i]);
                }
            }
        } else {
            *p++ = chr;
        }
    }
    exit(0);
}
