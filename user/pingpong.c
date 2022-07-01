#include "kernel/types.h"
#include "user/user.h"

int main(int ac, char** av)
{
    int pingPipe[2];
    int pongPipe[2];
    pipe(pingPipe);
    pipe(pongPipe);

    int pid = fork();
    if (pid == 0)
    {
        char b = 0;
        read(pingPipe[0], &b, 1);
        printf("%d: received ping\n", getpid());
        write(pongPipe[1], &b, 1);
    }
    else if (pid > 0)
    {
        char b = 44;
        write(pingPipe[1], &b, 1);
        read(pongPipe[0], &b, 1);
        printf("%d: received pong\n", getpid());
        wait(0);
    }
    else
    {
        printf("Failed to fork\n");
        exit(1);
    }

    close(pingPipe[0]);
    close(pingPipe[1]);
    close(pongPipe[0]);
    close(pongPipe[1]);

    exit(0);
}