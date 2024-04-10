#include "kernel/types.h"
#include "user/user.h"

void primes(int pl[]) {
    close(pl[1]);
    int p;
    if (!read(pl[0], &p, sizeof(p))) {
        exit(0);
    }
    fprintf(1, "prime %d\n", p);

    int pr[2];
    pipe(pr);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    } else if (pid > 0) {
        // parent
        close(pr[0]);
        int n;
        while (read(pl[0], &n, sizeof(n))) {
            if (n % p != 0) {
                write(pr[1], &n, sizeof(n));
            }
        }
        close(pr[1]);
        close(pl[0]);
        wait(0);
    } else {
        // child
        primes(pr);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "Usege: primes\n");
        exit(1);
    }

    int pr[2];
    pipe(pr);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    } else if (pid > 0) {
        // parent
        close(pr[0]);
        for (int i = 2; i <= 35; i++) {
            write(pr[1], &i, sizeof(i));
        }
        close(pr[1]);
        wait(0);
    } else {
        // child
        primes(pr);
    }
    exit(0);
}