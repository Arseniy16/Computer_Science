#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    int fds[2];
    pid_t pid;
    char buf[100];

    pipe(fds);

    pid = fork();

    if (pid)
    {
        while (1)
        {
            memcpy( buf, "abcdefghi\0",10);
            write( fds[1], buf, 10);
            //sleep(2);
        }
    }
    else
    {
        int retval = fcntl( fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK);
        printf("Ret from fcntl: %d\n", retval);
        while (1)
        {
            ssize_t r=read( fds[0], buf, 10 );
            printf("read: %d\n", r);

            if ( r > 0 )
            {
                printf("Buffer: %s\n", buf);
            }
            else
            {
                printf("Read nothing\n");
                perror("Error was");
                sleep(1);
            }
        }
    }
}