#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>

int
main(int argc, char **argv) {
   int p1[2], p2[2]; 
   pipe(p1);
   pipe(p2);
   if(fork() == 0) {
        close(p1[1]);
        close(p2[0]); 
        char c;
        read(p1[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], &c, 1);
        close(p1[0]);
        close(p2[1]);
        exit(0);
   } else {
        char c = 'a';
        close(p1[0]);
        close(p2[1]);
        write(p1[1], &c, 1);
        read(p2[0], &c, 1);
        printf("%d: received pong\n", getpid());
        close(p1[1]);
        close(p2[0]);
        exit(0);
   }
}
