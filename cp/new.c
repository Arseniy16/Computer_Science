#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <getopt.h>

#define BUF_SIZE 1024 

#define PERMS 0666

int create_file(char const * name, int perms);

int read_file(int fd, char * buf, size_t count);

int write_file(int fd, char * buf, size_t count);

int close_file(int fd);

char buf[BUF_SIZE + 1];

int inputFd = 0;
int outputFd = 1; 

int main (int argc, char *argv[])
{
    if (argc >= 3)
    {
        //printf("good\n");
        inputFd = open(argv[1], O_RDONLY);
        
        if (inputFd  == -1) 
        {
            //perror("ERROR to open file");
            error(0, errno, "%s", argv[1]);
            return -1;
            //continue;
        }
        outputFd = open(argv[2], PERMS);
        //outputFd = create_file(argv[2], PERMS);

        size = read_file(inputFd, buf, BUF_SIZE);
        
        write_file(outputFd, buf, size);

    /*
        if (close(inputFd) == -1) 
        {
            perror("ERROR to close input file_descriptor");
            return -1;
        }
    */
        close_file(inputFd);
    }
    else  
    {
        fprintf(stderr, "%s: пропущен операнд, задающий файл\n", argv[0]);
        return -1;
    }
    return 0;
}

int create_file(char const * name, int perms)
{
    int outputFd = 0;

    if ((outputFd = creat(name, perms)) == -1)
    {
        perror("ERROR to create file");
        return -1;
    }
    return outputFd;

}

int read_file(int inputFd, char * buf, size_t count)
{
    int size = 0;

    if ((size = read(inputFd, buf, count)) == -1)
    {
        perror("ERROR to read from file_descriptor");
        return -1;
    }

    return size;
}

int write_file(int outputFd, char * buf, size_t count)
{
    int size = write(outputFd, buf, count);

    if (size == -1 || size != count)
    {
        perror("ERROR to write to file_descriptor"); 
        return -1;
    }

    return size;
}
//todo
int close_file(int fd)
{
    if (close(fd) == -1) 
    {
        perror("ERROR to close file_descriptor");
        return -1;
    }
    return fd;
}