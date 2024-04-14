#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
    char* args[MAXARG];
    int argsSize = argc - 1;
    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }

    char buf[512];
    int bufSize = 0, bufLast = 0;
    char input[512];
    int len;
    while ((len = read(0, input, sizeof(input))) > 0) {
        for (int i = 0; i < len; i++) {
            if (input[i] == '\n') {
                buf[bufSize++] = '\0';
                args[argsSize++] = buf + bufLast;
                bufLast = bufSize;
                args[argsSize] = 0;

                if (fork() == 0) {
                    exec(args[0], args);
                    exit(0);
                }

                wait(0);
                argsSize = argc - 1;
            } else if (input[i] == ' ') {
                buf[bufSize++] = '\0';
                args[argsSize++] = buf + bufLast;
                bufLast = bufSize;
            } else {
                buf[bufSize++] = input[i];
            }
        }
    }
    exit(0);
}