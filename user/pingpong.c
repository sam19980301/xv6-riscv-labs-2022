#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int fds[2];
    char buf[1];

    if (argc != 1)
    {
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

    if (pipe(fds) != 0)
    {
        fprintf(2, "Error while piping\n");
        exit(1);
    }

    if (fork() == 0) // child process
    {
        read(fds[0], buf, 1);
        fprintf(0, "%d: received ping\n", getpid());
        close(fds[0]);

        write(fds[1], "b", 1);
        close(fds[1]);
    }
    else // parent child
    {
        write(fds[1], "a", 1);
        close(fds[1]);

        read(fds[0], buf, 1);
        fprintf(0, "%d: received pong\n", getpid());
        close(fds[0]);
    }

    exit(0);
}