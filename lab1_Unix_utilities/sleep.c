#include<kernel/types.h>
#include<kernel/stat.h>
#include"user/user.h"

int
main(int ac, char ** argv) {
    if(ac < 0) exit(1);
    int ticks = atoi(argv[1]);
    printf("ticks = %d\n", ticks);
    sleep(ticks);
    exit(0);
}
