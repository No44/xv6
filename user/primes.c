#include "kernel/types.h"
#include "user/user.h"

#define READ_END 0
#define WRITE_END 1

void sieve_primes_child(int my_prime, int *left_pipe)
{
    printf("[%d] prime %d\n", getpid(), my_prime);

    close(left_pipe[WRITE_END]);

    int prime = -1;
    int right_pipe[2] = {-1, -1};
    while (read(left_pipe[READ_END], &prime, sizeof(prime)) > 0)
    {
        if (right_pipe[0] == -1)
        {
            pipe(right_pipe);
            
            int pid = fork();
            if (pid == 0)
            {
                // no return
                sieve_primes_child(prime, right_pipe);
            }
            else if (pid < 0)
            {
                printf("Fork failed\n");
                exit(1);
            }

            // parent only
            close(right_pipe[READ_END]);
        }
        else
        {
            write(right_pipe[WRITE_END], &prime, sizeof(prime));
        }
    }

    close(left_pipe[READ_END]);
    close(right_pipe[WRITE_END]);
    wait(0);
    exit(0);    
}

int is_prime(int target)
{
    for (int i = 2; i < target / 2; i++)
    {
        if (target % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

void sieve_primes_root()
{
    int p[2];
    pipe(p);

    int pid = fork();
    if (pid == 0) // child
    {
        sieve_primes_child(2, p);
    }
    else if (pid > 0) // parent
    {
        close(p[READ_END]);
        for (int i = 3; i <= 35; i++)
        {
            if (is_prime(i))
            {
                write(p[WRITE_END], &i, sizeof(i));
            }
        }
        close(p[WRITE_END]);
        wait(0);
    }
    else
    {
        printf("Fork failed\n");
        exit(1);
    }
}

int main(int ac, char** av)
{
    sieve_primes_root();
    exit(0);
}