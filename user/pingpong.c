#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

    int p1[2], p2[2];
    pipe(p1), pipe(p2);

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    } else if (pid > 0) {
        // parent
        close(p1[0]), close(p2[1]);
        write(p1[1], "ping", 5);
        close(p1[1]);

        char buf[5];
        read(p2[0], buf, 5);
        close(p2[0]);
        fprintf(1, "%d: received %s\n", getpid(), buf);
    } else {
        // child
        close(p1[1]), close(p2[0]);
        char buf[5];
        read(p1[0], buf, 5);
        close(p1[0]);

        fprintf(1, "%d: received %s\n", getpid(), buf);
        write(p2[1], "pong", 5);
        close(p2[1]);
    }
    exit(0);
}