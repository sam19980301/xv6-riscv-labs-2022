#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
filter_proc(int lfds[])
{
    int prime, n, rfds[2];

    close(lfds[1]);

    if (read(lfds[0], &prime, sizeof(int)) != sizeof(int))
    {
        fprintf(2, "Error while child proc. first reading number\n");
        close(lfds[0]);
        exit(1);
    }
    printf("prime %d\n", prime);

    // still remain unfiltered number, keep forking
    if (read(lfds[0], &n, sizeof(int)) == sizeof(int))
    {
        if (pipe(rfds) != 0)
        {
            fprintf(2, "Error while piping\n");
            close(lfds[0]);
            exit(1);
        }

        // lfds[0] rfds[0] rfds[1] are open
        if (fork() == 0) // child process
        {
            close(lfds[0]);
            filter_proc(rfds);
        }
        else // parent process
        {
            close(rfds[0]);

            // filter-and-pass
            do
            {
                if (n % prime) // pass
                {
                    write(rfds[1], &n, sizeof(int));
                }
            } while (read(lfds[0], &n, sizeof(int)) == sizeof(int)); 

            close(lfds[0]);
            close(rfds[1]);
            wait(0); // exit in called order
        }
        exit(0); 
    }
}

int
main(int argc, char *argv[])
{
    /*
        p = get a number from left neighbor
        print p
        loop:
            n = get a number from left neighbor
            if (p does not divide n)
                send n to right neighbor
               
        PocA (w) --> (r) ProcB (w) --> (r) ProcC (w) --> (r) ProcD ...
                           2                 3                 5   ...
    */

    int fds[2], n;

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
        filter_proc(fds);
    }
    else // parent process
    {
        close(fds[0]);
        for (n = 2; n <= 35; n++)
            write(fds[1], &n, sizeof(int));
        close(fds[1]);
        wait(0); // exit in called order
    }
    exit(0);
}