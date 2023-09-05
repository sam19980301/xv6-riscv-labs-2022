#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"

#define BUFSIZE 512

int match(char*, char*);

char*
get_basename(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    return ++p;
}

void
find(char *path, char *name)
{
    char buf[BUFSIZE], *p;
    int fd;
    struct stat st;
    struct dirent de;    

    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "Cannot open %s\n", path);
        return ;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "Cannot stat %s\n", path);
        close(fd);
        return ;
    }

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        if (strcmp(get_basename(path), name) == 0)
            printf("%s\n", path);        
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
        {
            printf("find: path too long: %s\n", path);
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = '\0';
            find(buf, name);
        }
        break;
    }
    close(fd);
    return ;
}

int
main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: find path pattern\n");
        exit(1);
    }
    find(argv[1], argv[2]);

    exit(0);
}