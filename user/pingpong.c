#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    uint8 byte;
    int p_parent[2];
    int p_child[2];
    pipe(p_parent);
    pipe(p_child);
    if (fork() == 0) {
        close(p_parent[1]);
        close(p_child[0]);
        if (read(p_parent[0], &byte, sizeof(byte)) != 0) {
            fprintf(1, "%d: received ping\n", getpid());
            write(p_child[1], &byte, sizeof(byte));
            exit(0);
        } else {
            fprintf(2, "no byte from parent!\n");
            exit(1);
        }
    } else {
        close(p_parent[0]);
        close(p_child[1]);
        write(p_parent[1], &byte, sizeof(byte));
        if (read(p_child[0], &byte, sizeof(byte)) != 0) {
            fprintf(1, "%d: received pong\n", getpid());
            exit(0);
        } else {
            fprintf(2, "no byte from child!\n");
            exit(1);
        }
    }
}
