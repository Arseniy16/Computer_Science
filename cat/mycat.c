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

int main(int argc, char * argv[])
{
    /**
     * Initialize all variables
     */
    int inputFd = 0;
    int outputFd = 1;

    char buf[BUF_SIZE + 1];
    int size = 0;
    
    /**
     * When there aren't parametrs in console: <myid __> 
     */
    if (argc == 1)
    {   
        /**
         * Read file until reach EOF
         */
        while ((size = read_file(inputFd, buf, BUF_SIZE)) > 0)
        {         
            write_file(outputFd, buf, size);
        }

        if (close(inputFd) == -1) 
        {
            perror("ERROR to close input file descriptor");
            return -1;
        }
    }
    else
    {
        /**
         * Read all parametrs
         */
        while (*++argv != NULL)
        {
            inputFd = open(*argv, O_RDONLY);
            
            if (inputFd  == -1) 
            {
                error(0, errno, "%s", argv[0]);
                //perror("ERROR to open file");
                continue;
            }
            
            /**
             * Miss the empty file
             */
            if ((size = read_file(inputFd, buf, BUF_SIZE)) == 0)
                continue;

            write_file(outputFd, buf, size);

            if (close(inputFd) == -1) 
            {
                perror("ERROR to close input file descriptor");
                return -1;
            }

        }
    }

    if (close(outputFd) == -1)
    {
        perror("ERROR to close output file descriptor");
        return -2;
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
