#include <stdbool.h>
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void f(int p_read) {
    int prime;
    read(p_read, &prime, sizeof prime);
    printf("prime %d\n", prime);
    int n;
    int p[2];
    bool uninitialized = true;
    while (read(p_read, &n, sizeof n) != 0) {
        if (n % prime != 0) {
            if (uninitialized) {
                uninitialized = false;
                // initialization
                pipe(p);
                if (fork() == 0) {
                    close(p[1]);
                    f(p[0]);
                } else {
                    close(p[0]);
                }
            }
            write(p[1], &n, sizeof n);
        }
    }
    close(p[1]);
    close(p_read);
    wait((int *) 0);
    exit(0);
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        f(p[0]);
    } else {
        close(p[0]);
        int i;
        for (i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof i);
        }
        close(p[1]);
        wait((int *) 0);
    }
    exit(0);
}
