#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<math.h>
#include<sys/wait.h>

#define BUF 4096

int main()
{
	char buffer[BUF];
	char * stop = "exit";
	char * enter = "$";
	char * next[1];
	next[0] = "./shell";
	int * wstatus;
	pid_t proc = getpid();
	//printf("main is %d\n", proc);
	int symbol = 0, key = 0, key2 = 0, i = 0, k = 0, q = 0, adress = 0, a = 0, b = 0;
//	fflush(NULL);
		write(1, enter, 2);
		int count = read(0, buffer, BUF);
		count++;
		if(strncmp(buffer, stop, 4) == 0)
		{
			return 0;
		}
		char * argv = calloc(pow(count, 3), sizeof(char));
		int * words = calloc(count, sizeof(int));
		char ** commands = (char**)calloc(pow(count, 2), sizeof(char*));
		for(i = 0; i < count - 1; i++)
		{
			for(k = 0; k < count; k++)
			{
				for(q = 0; q < count; q++)
				{
				        adress = i * pow(count, 2) + k * count + q;
					if(buffer[symbol] == ' ')
					{
						if(buffer[symbol - 1] == '|' || buffer[symbol + 1] == '|')
						{
							symbol++;
							q--;
							continue;
						}
						symbol++;
						break;
					}
					if(buffer[symbol] == '|')
					{
						symbol++;
						key = 1;
						break;
					}
					argv[adress] = buffer[symbol];
		//			printf("another argv check: %d %c : i = %d, k = %d, q = %d\n", adress, argv[adress], i, k, q);
					symbol++;
					if(symbol == count)
					{
						key2 = 1;
						break;
					}
				}
				argv[adress + 1] = '\0';		
				if(key == 1 || key2 == 1)
				{
					key = 0;
					break;
				}	
			}
			words[i] = k;
			if(key2 == 1)
			{
				argv[adress - 1] = '\0';
				break;
			}
		}
		for(a = 0; a <= i; a++)
		{
		//	printf("helo its %d\n", words[a]);
			for(b = 0; b <= words[a]; b++)
			{
				commands[a * count + b] = argv + ((count) * b) + (int)(pow(count, 2) * a);
		//	       printf("check before NULL: %s %d\n", commands[a * count + b], a * count + b);	
			}
			commands[a * count + b] = NULL;
		}
		if(i == 0)
		{
			pid_t child = fork();
			if(child == 0)
			{
				execvp(commands[0], commands);
			}
			if(child != 0)
			{
				wait(wstatus);
				free(commands);
				free(argv);
				free(words);
				execvp(next[0], next);
			}
		}
	        int x = i, pipebuf[2], number = 0;
		int * pipefds = calloc(2 * x, sizeof(int));
		while(x > 0)
		{
			pipe(pipebuf);
			pipefds[number] = pipebuf[0];
			pipefds[number + 1] = pipebuf[1];
			number = number + 2;
			x--;
		}
		pid_t * ids = calloc(i + 1, sizeof(pid_t));
		//printf("file is %s with arg %s\n", commands[0], commands[1]);
		for(; x <= i; x++)
		{
			if(getppid() == proc)
			{
				break;
			}
			ids[x] = fork();
		//	printf("current proc %d %d\n", getpid(), getppid());
			if(ids[x] == 0)
			{
				break;
			}
		//	printf("still forking %d\n", getpid());
		}
		if(getpid() == proc)
		{
		//	printf("waiter is %d\n",getpid());
			wait(wstatus);
			execvp(next[0], next);
		//	printf("start %d\n", getpid());
		}
		else
		{
		//printf("parent and forkers are %d, %d\n", proc, getpid());
		x = 0;
		for(; x <= i; x++)
		{
			if(0 == ids[x])
			{
				if(i != 0)
				{
					if(0 == ids[0])
					{
						printf("%s %s\n",commands[0], commands[1]);
						dup2(pipefds[1], 1);
					       	execvp(commands[0], commands);
					}
					else if(0 == ids[i])
					{
						printf("%s\n", commands[x * (count)]);
						dup2(pipefds[number - 2], 0);
						//sleep(1);
						execvp(commands[x * count], commands + (x * count));
					}
					/*else
					{
						dup2(pipefds[2 * x], 1);
						dup2(pipefds[2 * x - 1], 0);
						execvp(commands[x * (count + 1)], commands + (x * (count + 1)) + 1);
					}*/
				}
			}	
		}
		exit(0);
		}
		free(pipefds);
		free(commands);
		free(words);
		free(argv);
		return 0;
}
