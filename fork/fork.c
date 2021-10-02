#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int fork_process(int num_proc)
{
    int status = 0;
    pid_t pid = 0;

    for(int i = 0; i < num_proc; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            perror("ERROR to fork");
            return -1;
        }
        else if (pid == 0)
        {
            printf("child: pid = %d, ppid = %d\n", getppid(), getpid());
        }
        else
        {
            if (wait(&status) == -1)
            {
                perror("ERROR to wait");
            }
            return 0;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        printf("parent: pid = %d\n", getpid());

        int num_proc = atoi(argv[1]);

        fork_process(num_proc);
    }
    else 
    {
        printf("Incorrect input argumets!\n");
    }

    return 0;
