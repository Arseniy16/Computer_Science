#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf
{
	long type;
};

void judge(int msg_key, int num)
{
	printf("Judge init\n");

	msgbuf msg;

	for (int i = 0; i < num; i++)
	{
		if (msgrcv(msg_key, &msg, sizeof(msg), 1, 0) < 0)
		{
			perror("MSGRCV");
			exit(EXIT_FAILURE);
		}
	}

	printf("Start competition\n");
	
	exit(0);
}

void runner(int msg_key, int num_run, int num)
{
	printf("Runner %d init\n", num_run+1);
	exit(0);
}


int main(int argc, char * argv[])
{
	if (argc < 2)
		return -1;

	int num = atoi(argv[1]);

	int msg_key = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0777); //mode = 0777
	if (msg_key < 0)
	{
		perror("MSGGET");
		return -1;
	}	

	//for judge
	
	pid_t pid = fork();

    if (pid == -1)
    {
        perror("ERROR to fork");
        return -1;
    }
	else if (pid == 0)
	{
		judge(msg_key, num); //judge never return
	}
		

	//for runners
	
    for(int i = 0; i < num; i++)
    {
        pid = fork();

        if (pid == -1) 
        {
            perror("ERROR to fork");
            return -1;
        } 
        else if (pid == 0) 
        {
        	runner(msg_key, i, num);
        }

    }

    
    for (int i = 0; i < num; i++)
    {
	    int status = 0;
	    
	    if (wait(&status) == -1)
	    {
	        perror("ERROR to wait");
	        return -1;
	    }
	}

    msgctl(msg_key, IPC_RMID, 0);

	return 0;
}