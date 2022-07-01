#include "kernel/types.h"
#include "user/user.h"

int main(int ac, char** av)
{
    if (ac < 2)
    {
        printf("Usage: sleep amount\n");
    }

    int amount = atoi(av[1]);
    sleep(amount);

    exit(0);
}