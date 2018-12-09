#include "create.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

mode_t perm = S_IRUSR | S_IWUSR | S_IRGRP;
mode_t perm_dir = S_IRWXU | S_IRGRP;

int main(int argc, char *argv[])
{
    char flag = argv[1][1];

    switch (flag) {
    case 'd':
        create_dir(argv[2]);
        break;
    case 'f':
        create_file(argv[2]);
        break;
    case 'h':
        create_link(argv[2], argv[3]);
        break;
    case 's':
        create_symlink(argv[2], argv[3]);
    }

    return 0;
}

int create_file(char *pathname)
{
    int fp;
    if ((fp = creat(pathname, perm)) == -1) {
        printf("%s\n", strerror(errno));
        return -1;
    }
    return 1;
}

int create_dir(char *pathname)
{
    int fp;
    if ((fp = mkdir(pathname, perm_dir)) == -1) {
        printf("%s\n", strerror(errno));
        return -1;
    }
    return 1;
}

int create_link(char *old, char *new)
{
    int fp;
    if ((fp = (link(old, new))) == -1) {
        printf("%s", strerror(errno));
        return -1;
    }
    return 1;
}

int create_symlink(char *old, char *new)
{
    int fp;
    if ((fp = (link(old, new)) == -1)) {
        printf("%s", strerror(errno));
        return -1;
    }
    return 1;
}
