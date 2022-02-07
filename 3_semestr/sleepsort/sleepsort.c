#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int fork_process(int num_proc, int * number)
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
        	usleep(10000 * (unsigned int)number[i]);
        	printf("%d ", number[i]);
        	return 0;
        }
    }
	
	for (int i = 0; i < num_proc; i++)
	{
		if (wait(&status) == -1)
	    {
	        perror("ERROR to wait");
	    }
	}
	return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
    	printf("Incorrect input argumets!\n");
    	return -1;
    }

    int num_proc = argc - 1;

    int * number = (int *)calloc(argc - 1, sizeof(int));
   
    for (int i = 0; i < num_proc; i++)
    {
    	number[i] = atoi(argv[i+1]);
    }

    fork_process(num_proc, number);

    free(number);

    return 0;
}