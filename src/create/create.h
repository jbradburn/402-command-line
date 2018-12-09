#ifndef __CREATE_H_
#define __CREATE_H_

#include <dirent.h>

int create_file(char *pathname);
int create_dir(char *pathname);
int create_link(char *old, char *new);
int create_symlink(char *old, char *new);

#endif // __CREATE_H_
