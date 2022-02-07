#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#define MAX_SIZE 1024 
#define MAX_NUM_CMD 10

int read_file(int fd, char * buf, size_t count);

int write_file(int fd, char * buf, size_t count);

int close_file(int fd);

char** spliting_cmd(char* cmd)
{
    char** line = (char**)malloc(MAX_SIZE);
    char* word = strtok(cmd, " ");
    size_t i = 0;

    while(word)
    {
        line[i++] = (char*)malloc(MAX_SIZE);
        strcpy(line[i-1], word);
        word = strtok(NULL, " ");
        printf("<%s>", line[i-1]);
    }

    line[i++] = (char*)malloc(MAX_SIZE);
    line[i-1] = NULL;

    return line;
}

size_t parsing_cmd(char* line, char*** cmd)
{
    (*cmd) = (char**)malloc(MAX_NUM_CMD * sizeof(cmd[0]));
    (*cmd)[0] = line;
    size_t cur_pos = 1;
    char* cur_ptr = strchr(line, '|');
    
    while (cur_ptr)
    {
        (*cmd)[cur_pos++] = cur_ptr + 1;
        *cur_ptr = 0;
        line = cur_ptr + 1;
        cur_ptr = strchr(line, '|');
    }

    return cur_pos;
}

int main()
{
    while(1)
    {
        size_t num_cmd = 0;
        char** cmd = NULL;

        char* line = readline("\x1B[31m###->\x1B[33m");

        num_cmd = parsing_cmd(line, &cmd);

        int fd_in = 0;

        for (size_t i = 0; i < num_cmd; ++i)
        {
            int fd[2];
            if (pipe(fd) < 0)
            {
                perror("ERROR with pipe");
                return -1;
            }

            pid_t pid = fork();

            if(pid == 0)
            {
                close_file(fd[0]);

                char** line = spliting_cmd(cmd[i]);
                
                if (fd_in != STDIN_FILENO)
                {
                    if (dup2(fd_in, STDIN_FILENO) < 0)
                    {
                        perror("ERROR to duplicate fd");
                        return -1;
                    }
                    close_file(fd_in);
                }

                if(i != num_cmd - 1)
                {
                    if (fd[1] != STDOUT_FILENO)
                    {
                        if (dup2(fd[1], STDOUT_FILENO) < 0)
                        {
                            perror("ERROR to duplicate fd");
                            return -1;
                        }
                        close_file(fd[1]);
                    }
                }

                if (execvp(line[0], line) < 0)
                {
                    perror("ERROR with execvp");
                    return -1;
                }
            } 
            else if(pid > 0)
            {
                close_file(fd[1]);

                if(wait(NULL) < 0)
                {
                    perror("ERROR to wait");
                    return -1;
                }

                fd_in = fd[0];
            } 
            else
            {
                perror("ERROR with fork");
                return -1;
            }
        }
        free(cmd);
        free(line);
    }
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
