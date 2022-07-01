#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char* start_directory, char* filename)
{
    int dir_fd;
    if ((dir_fd = open(start_directory, 0)) < 0)
    {
        fprintf(2, "Could not open %s\n", start_directory);
        return;
    }

    struct dirent de;
    while (read(dir_fd, &de, sizeof(de)) > 0)
    {
        if (de.inum == 0
            || strcmp(de.name, ".") == 0
            || strcmp(de.name, "..") == 0)
        {
            continue;
        }

        if (strcmp(filename, de.name) == 0)
        {
            printf("%s/%s\n", start_directory, de.name);
        }

        char full_path[512] = {0};
        strcpy(full_path, start_directory);
        strcat(full_path, "/");
        strcat(full_path, de.name);

        struct stat filestat;
        if (stat(full_path, &filestat))
        {
            fprintf(2, "stat failed for %s\n", full_path);
            continue;
        }
        else
        {
            if (filestat.type == T_DIR)
            {
                find(full_path, filename);
            }
        }

    }
}

int main(int ac, char** av)
{
    if (ac != 3)
    {
        printf("Usage: find directory filename");
        exit(1);
    }

    find(av[1], av[2]);

    exit(0);
}