#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>

#define MAX 36
#define FIRST_PIRME 2

// return input stream FD of the generator
int
generator(){
    int out_pipe[2];
    pipe(out_pipe);
    if(!fork()) {
        close(out_pipe[0]); 
        // process that generating the primes and put into the pipe
        for(int i = FIRST_PIRME; i < MAX ; i++) {
            write(out_pipe[1], &i, sizeof(int));
        }
        close(out_pipe[1]);
        exit(0);
    }
    // parent process, only get the input stream from the generator
    close(out_pipe[1]);
    return out_pipe[0];
}

int
prime_filter(int in_fd, int prime) {
    int out_pipe[2];
    pipe(out_pipe);
    if(!fork()) {
        close(out_pipe[0]);
        int num;
        while((read(in_fd, &num, sizeof(int))) != 0) {
            if(num % prime) {
                // if input number of curr filter can't be divided by prime
                write(out_pipe[1], &num, sizeof(int));
            }
        }
        close(in_fd);
        close(out_pipe[1]);
        exit(0);
    }
    close(in_fd);  // only return the output stream of curr filter
    close(out_pipe[1]);
    return out_pipe[0];
}

int
main(int argc, char **argv) {
    int prime;
    int in = generator();
    while(read(in, &prime, sizeof(int))) {
        // the 1st number in input stream is prime
        printf("prime %d\n", prime);
        // connect curr input to next filter and get the new input stream
        in = prime_filter(in, prime);
    }
    return 0;
}
