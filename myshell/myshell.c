#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

// #include <readline/readline.h>
// #include <readline/history.h>


#define BUF_SIZE 1024 

int read_file(int fd, char * buf, size_t count);

int write_file(int fd, char * buf, size_t count);

int close_file(int fd);

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
            //printf("child: pid = %d, ppid = %d\n", getppid(), getpid());
            //todo
        
        /*    if (execvp(argv[1], argv+1) == -1)
            {
                perror("ERROR to exec file");
                return -1;
            }
        */
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

int main(int argc, char const *argv[])
{
    char buf[BUF_SIZE+1];
    //char * buf = (char *)calloc(BUF_SIZE, sizeof(char));
    //buf = readline("hello");
/*    
    while((buf = readline("$")) != NULL)
    {
        printf("good\n");
        //printf("%c", buf);
    }
*/
    printf("#");
    int size = read_file(0, buf, BUF_SIZE);
    printf("size = %d\n", size);

    //printf("buf = %s\n", buf);
/*    while (!eof(stdin))
    {
        printf("->");
        buf = readline('$');
        //read_file(0, buf, 1);
        //printf("good\n");
    }
*/
   //printf("buf = %s\n", buf);
    //write(1, buf, sizeof(buf));
    //exit(2);
    //free(buf);
    return 0;
}
int read_file(int inputFd, char * buf, size_t count)
{
    int size = 0;

    if ((size = read(inputFd, buf, count)) == -1)
    {
        perror("ERROR to read from file");
        return -1;
    }

    return size;
}

int write_file(int outputFd, char * buf, size_t count)
{
    int size = write(outputFd, buf, count);

    if (size == -1 || size != count)
    {
        perror("ERROR to write to file"); 
        return -1;
    }

    return size;
}

int close_file(int fd)
{
    if (close(fd) == -1) 
    {
        perror("ERROR to close file_descriptor");
        return -1;
    }
    return fd;
}
