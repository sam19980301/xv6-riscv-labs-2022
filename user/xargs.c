#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUFSIZE 512
int
main(int argc, char *argv[])
{
    int i, idx, offset;
    char buf[BUFSIZE], *xargv[MAXARG];
    memset(xargv, 0, sizeof(char *) * MAXARG);

    if (argc < 2)
    {
        fprintf(2, "Usage: xargs cmd args\n");
        exit(1);
    }

    // initial arguments
    for (i = 0; i < argc - 1; i++)
    {
        xargv[i] = malloc(strlen(argv[i+1]) + 1);
        strcpy(xargv[i], argv[i+1]);
    }
    // xargv[0 ... argc - 2] are initialized

    // appending line-arguments from xargv[argc - 1]
    idx = 0;
    offset = 0;
    while (read(0, buf + idx, 1) == 1)
    {
        if (idx >= BUFSIZE - 1)
        {
            fprintf(2, "The argument is too long\n");
            exit(1);
        }

        if (buf[idx] == ' ' || buf[idx] == '\n') // delimiter
        {
            buf[idx] = '\0';
            xargv[i] = malloc(idx - offset + 1);
            strcpy(xargv[i], buf + offset);

            if (buf[idx] == ' ') // arg delimiter
            {
                idx++;
            }
            else // cmd delimiter
            {
                // xexec
                if (fork() == 0) // chlid process
                {
                    if (exec(argv[1], xargv) < 0)
                    {
                        fprintf(2, "Error while xexec-ing\n");
                        exit(1);
                    }
                }
                else // parent process
                {
                    wait(0);
                }
                // removing appended line-arguments
                for (; i > argc - 2; i--)
                {
                    free(xargv[i]);
                    xargv[i] = 0;
                }
                buf[0] = '\0';
                idx = 0;
            }

            i++;
            offset = idx;
        }
        else
        {
            idx++;
        }
    }
    exit(0);
}