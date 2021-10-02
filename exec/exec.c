#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>

double time_ms(struct timeval start, struct timeval stop)
{
    double sec1 = (stop.tv_usec - start.tv_usec)/1000000.0;
    double sec2 = stop.tv_sec - start.tv_sec;
    return (sec1 + sec2) * 1000.0;
}

int main(int argc, char const *argv[])
{
    pid_t pid = 0;  
    int status = 0;

    struct timeval start = {0, 0};
    struct timeval stop = {0, 0};

    if (argc == 1)
    {
        printf("Write more argumets\n");
        return -1;
    }
    else
    {
        pid = fork();

        if (gettimeofday(&start, NULL) == -1)
        {
            perror("ERROR to get time start");
            return -2;
        }

        if (pid == -1)
        {
            perror("ERROR to fork");
            return -1;
        }
        else if (pid == 0)
        {
            if (execvp(argv[1], argv+1) == -1)
            {
                perror("ERROR to exec file");
                return -1;
            }           
        }
        else
        {
            wait(&status);
        }   

        if (gettimeofday(&stop, NULL) == -1)
        {
            perror("ERROR to get time stop");
            return -2;
        }                       
    }

    printf("TIME: %.3lg ms\n", time_ms(start, stop));

    return 0;
}