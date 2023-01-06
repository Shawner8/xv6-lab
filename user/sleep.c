// Pause for a user-specified number of ticks

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: sleep number-of-ticks\n");
        exit(1);
    }

    int num_of_ticks = atoi(argv[1]);
    sleep(num_of_ticks);
    exit(0);
}
