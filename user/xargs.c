void xargs(char* command, int paramCount, char** params)
{
    char buf[512] = {0};
    int bytes_read;
    while ((bytes_read = read(0, buf, 512)) > 0)
    {
        
    }
}

int main(int ac, char** av)
{
    if (ac < 2)
    {
        printf("Usage: xargs cmd [params...]\n");
        exit(1);
    }

    xargs(av[1], ac - 2, &av[2]);

    exit(0);
}