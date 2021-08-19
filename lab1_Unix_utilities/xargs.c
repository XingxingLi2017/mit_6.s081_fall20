#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include"kernel/param.h"

#define size_cp (sizeof(char*))
#define STDIN 0

int
main(int argc, char *argv[]) {
    char **newarg = malloc(argc * size_cp);
    memmove(newarg, argv + 1, size_cp * (argc - 1));
    int flag = 1;
    while(flag) {
        char buffer[100], *p = buffer;
        while((flag = read(STDIN, p, 1)) && *p != '\n') {
            ++p;
        }
        if(!flag) { // the input is EOF 
            exit(0);
        }
        *p = 0;
        newarg[argc - 1] = buffer;
        if(!fork()) {
            // child process
            exec(argv[1], newarg);
            exit(0);
        }
        // wait for child termination
        wait(0);
    }
    return 0;
}

