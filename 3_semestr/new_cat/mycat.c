/* ------------------------------------------------- */
/* This is the program which emulates _cat_ in Linux */
/* ------------------------------------------------- */ 

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

#define BUF_SIZE 1024 


int read_file(int fd, char * buf, size_t count);

int write_file(int fd, char * buf, size_t count);

int my_cat(int child_pfd, int parent_pfd, char * buf)
{
    while ((size = read_file(child_pfd[0], buf, BUF_SIZE)) > 0);
    
    write_file(parent_pfd[1], buf, size);

    if (close(child_pfd[0]) < 0) 
    {
        perror("ERROR to close input file descriptor");
        return -1;
    }

    if (close(parent_pfd[1]) < 0)
    {
        perror("ERROR to close output file descriptor");
        return -1;
    }
    
    return 0;
}

int mypipe(int * pfd)
{
    if (pipe(pfd) < 0)
    {
        perror("ERROR with pipe");
        return -1;
    }
}

int main(int argc, char * argv[])
{
    char buf[BUF_SIZE + 1];
    
    int num_proc = 10;

    if (argc == 2)
    {
        num_proc = atoi(argv[1]);
    }

    for (int i = 0; i < num_proc; i++)
    {
        int child_pfd[2];
        int parent_pfd[2];

        // create pipes
        mypipe(child_pfd);
        mypipe(parent_pfd);

        switch (fork())
        {
            case -1:
                perror("ERROR to fork");
                return -1;

            case 0:     /* child process */
                // read end is unused
                if (close(child_pfd[1]) < 0)
                {
                    perror("ERROR to close child_pfd[1]");
                    return -1;
                }

                if (my_cat(child_pfd, parent_pfd, buf) < 0)
                {
                    perror("ERROR with child");
                    return -1;
                }


            default:    /* parent process */
                // write end is unused
            /*
                if (close(pfd[1]) < 0)
                {
                    perror("ERROR to close pfd[1]");
                    return -1;
                }
            */  
                struct pollfd* fds = (pollfd*)calloc(num_proc, sizeof(fds[0]));
                for (size_t i = 0; i < num_proc; ++i)
                {
                    fds[i].fd = child_pfd[0];
                    fds[i].events = POLLIN | POLLNVAL | POLLERR;
                }

                while(1)
                {
                    int state = poll(fds, num_proc, timeout_msecs);
                    if (state == 0)
                        break;                    
                
                    for (size_t i = 0; i < num_proc; ++i)
                    {
                        if ( (fds[i].revents & POLLERR) || (fds[i].revents & POLLNVAL) )
                        {
                            perror("ERROR fds");
                            return -1;
                        }
                        if (fds[i].revents & POLLIN)
                        {
                            char * new_buf = NULL;
                            size_t buf_size = 0;

                            ssize_t file_size = read_file(parent_pfd[0], new_buf, buf_size);

                            write_file(parent_pfd[1], new_buf, file_size);

                        } 
                    }
                }

                // close stdin
                if (close(child_pfd[0]) < 0)
                {
                    perror("ERROR to close child_pfd[0]");
                    return -1;
                }

                if (close(parent_pfd[1]) < 0)
                {
                    perror("ERROR to close child_pfd[1]");
                    return -1;
                }    
        }

    }
    return 0;
    

}  


//--------------------------------
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
