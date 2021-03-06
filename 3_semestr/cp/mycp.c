/* ------------------------------------------------- */
/* This is the program which emulates _cp_ in Linux  */
/* ------------------------------------------------- */ 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#define BUF_SIZE 1024 

#define PERMS 0666

char buf[BUF_SIZE + 1];

//-------flags--------
//--------------------
int VERBOSE = 0;
int INTERACTIVE = 0;
int FORCE = 0;
//--------------------

int create_file(char * name, int perms);

int read_file(int fd, char * buf, size_t count);

int write_file(int fd, char * buf, size_t count);

int close_file(int fd);

/**
 * //--verbose//
 * Explain what is being done
 */
int verbose(char * input, char * output)
{
    int fd = 1;
    write_file(1, "'", sizeof("'"));

    write_file(1, input, strlen(input));

    write_file(1, "' -> '", sizeof("' -> '"));

    write_file(1, output, strlen(output));

    write_file(1, "'\n", sizeof("'\n"));

    return fd;
}

/**
 * //--interactive//
 * Prompt before overwrite (overrides a previous -n option)
 */
int interactive(char * name_file)
{
    int fd = -1;
    if ((fd = open(name_file, O_WRONLY|O_CREAT|O_EXCL, PERMS)) == -1)
    {
        if (errno == EEXIST)
        {
            write_file(1, "cp: overwrite '", sizeof("cp: overwrite '"));
            write_file(1, name_file, strlen(name_file));
            write_file(1, "'? ", sizeof("'? "));

            char c = 0;
            while ((c = getchar()) == '\n')
                fflush(stdin);

            if (toupper(c) == 'Y')
            {
                return fd;
            }

            return -1;
        }
    }

    close_file(fd); 
    return 0;
}

/**
 * //--force//
 * If an existing destination file cannot be opened, remove it  and
 * try  again  (this  option  is ignored when the -n option is also
 * used)
 */
int force (char * name_file)
{
    int fd = -1;
    if ((fd = open(name_file, O_WRONLY|O_CREAT|O_EXCL, PERMS)) == -1)
    {
        if (errno == EEXIST)
        {
            if (open(name_file, O_WRONLY, PERMS) == -1)
            {
                if (remove(name_file) == -1)
                {
                    perror("ERROR to remove file");
                    return -1;
                }
                
                if (create_file(name_file, PERMS) == -1)
                {
                    perror("ERROR force");
                    return fd;
                }
            }
            return 0;
        }
    }

    close_file(fd);
    return 0;
}

int my_getopt (int argc, char ** argv)
{
    static struct option long_options[] =
    {
        {"verbose",     no_argument, NULL, 'v'},
        {"force",       no_argument, NULL, 'f'},
        {"interactive", no_argument, NULL, 'i'},
        {0,             0,           0,     0 }
    };

    int opt_index = 0;
    int flag = 0;

    while ((flag = getopt_long(argc, argv, "ivf", long_options, &opt_index)) != -1)
    {
        switch (flag)
        {
            case 'i':
                INTERACTIVE = 1;
                break;  

            case 'v':
                VERBOSE = 1;
                break;

            case 'f':
                FORCE = 1;
                break;

            case '?':
                fprintf(stderr, "%s : Unknown option\n", argv[optind-1]);
                return -1;

            default :
                perror("ERROR getopt_long");
                return -1;
        }
    }

    return optind;
}

int copy_file (char * input, char * output)
{
    int inputFd = open(input, O_RDONLY);
    int outputFd;
    
    if (inputFd  == -1) 
    {
        error(0, errno, "%s", input);
        return -1;
    }

    if (INTERACTIVE) 
        if ((outputFd = interactive(output)) < 0)
            return -1;     
            
    if (FORCE) 
        if ((outputFd = force(output)) < 0) 
            return -1;

    if (VERBOSE) 
        outputFd = verbose(input, output);

    //int outputFd = create_file(output, PERMS);

    size_t size = 0;
    while (size = read_file(inputFd, buf, BUF_SIZE))
    {
        write_file(outputFd, buf, size);
    }

    close_file(inputFd);
    close_file(outputFd);

    return 0;
}

int dir_copy (char * name_file, char * dir_file)
{
    int length = strlen(name_file) + strlen(dir_file) + 1;
    
    char * path = malloc(length);
    if (path == NULL)
    {
        error(0, errno, "ERROR, path == NULL");
        return -1;
    }

    strcpy(path, dir_file);

    if (strrchr(dir_file, '/') == NULL)
    {
        strcat(path, "/");
    }

    strcat(path, name_file);

    copy_file(name_file, path);

    free(path);

    return 0;
}

int create_file(char * name, int perms)
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

int close_file(int fd)
{
    if (close(fd) == -1) 
    {
        perror("ERROR to close file_descriptor");
        return -1;
    }
    
    return fd;
}

int is_dir(char * name_dir)
{
    DIR * ptr = NULL;
    if ((ptr = opendir(name_dir)) == NULL)
        return 0;

    closedir(ptr);
    return 1;
}

int main(int argc, char *argv[])
{
    int size = 0;

    int optind = my_getopt(argc, argv);
        
    if (optind < 0)
        return -1; 

    int new_argc = argc - optind;

    if (new_argc >= 2)
    {
        /**
         * if not a directory
         */
        if (!is_dir(argv[argc-1]))
        {
            if ((errno == ENOTDIR) || (errno == ENOENT))  
            {
                if (new_argc == 2)
                {
                    copy_file(argv[optind], argv[argc-1]);
                    return 0;
                }
                
                fprintf(stderr, "%s: the target %s is not a directory\n", argv[0], argv[argc-1]);
                return -1;
            }
        }
        /**
         * if a directory
         */
        else
        {
            for (optind; optind < argc-1; optind++)
                if (dir_copy(argv[optind], argv[argc-1]) < 0)
                    return -1;
        }
    }
    else
    {
        fprintf(stderr, "%s: the operand specifying the file is missed\n", argv[0]);
        return -1;
    }

    return 0;
}