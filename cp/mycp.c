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
#include <assert.h>

#define BUF_SIZE 1024 

#define SIZE 100

#define PERMS 0666

int create_file(char * name, int perms);

int read_file(int fd, char * buf, size_t count);

int write_file(int fd, char * buf, size_t count);

int close_file(int fd);

char buf[BUF_SIZE + 1];

int inputFd = 0;
int outputFd = 1;
/*
int interactive(char* name_file)
{
    open(name_file, O_WRONLY|O_CREAT|O_EXCL, PERMS);
    if(errno == EEXIST)
    {
        mywrite(1, "cp: overwrite '",sizeof("cp: overwrite '"));
        mywrite(1, name_file, strlen(name_file));
        mywrite(1, "'? ", sizeof("'? "));
        char c = 0;
        while((c = getchar()) == '\n')
            fflush(stdin);
        if(c == 'Y' || c == 'y')
        {
            return 0;
        }
        return 1;
    }
    return 0;
}
*/
/*
void verbose(char* name_file, char* name_file)
{
    mywrite(1, "'", sizeof("'"));
    mywrite(1, name_file, strlen(name_file));
    mywrite(1, "' -> '", sizeof("' -> '"));
    mywrite(1, name_file, strlen(name_file));
    mywrite(1, "'\n", sizeof("'\n"));
}
*/

//todo
/*
int force (const char * name_file)
{
    if (open(name_file, O_WRONLY|O_CREAT|O_EXCL, PERMS) < 0)
        if(errno == EEXIST)
        {
            if(open(name_file, O_WRONLY, PERMS) < 0)
            {
                if(remove(name_file) < 0)
                {
                    perror("ERROR to remove file");
                    return -1;
                }
                if(open(name_file, O_WRONLY|O_CREAT, PERMS) < 0)
                {
                    perror("ERROR force");
                    return -1;
                }
            }
            return 0;
        }
    return 0;
}
*/
int my_getopt(int argc, char ** argv)
{
    static struct option long_options[] =
    {
        {"verbose",     no_argument, NULL, 'v'},
        {"force",       no_argument, NULL, 'f'},
        {"interactive", no_argument, NULL, 'i'},
        {0,             0,           0,     0}
    };

    int opt_index = 0;
    int flag = 0;

    while ((flag = getopt_long(argc, argv, "ivf", long_options, &opt_index)) != -1)
    {
        switch (flag)
        {
            case 'i':
                printf("intersact\n"); //
                //intersact();
                break;  

            case 'v':
                printf("verbose\n"); //
                //verbose();
                break;

            case 'f':
                printf("force\n"); //
                //force();
                break;

            case '?':
                fprintf(stderr, "%s : Unknown option\n", argv[optind-1]);
                return -1;

            default :
                perror("ERROR getopt_long");
                return -1;
        }
    }
    //printf("opt_ind = %d\n", optind);
    return optind;
}

char * get_path (char * name_file, char * dir_file)
{
    assert(dir_file);
    assert(name_file);
    printf("good\n");

    int len_file = strlen(name_file);
    int len_dir = strlen(dir_file);

    char * path = (char *) calloc(len_dir + len_file + 1, sizeof(char));
    
    if (strchr(dir_file, '/') == NULL)
    {
        strcat(dir_file, '/');
        printf("new_good\n");
    }
    
    printf("dir_file = %s\n", dir_file);
    //verify
    return strcat(dir_file, name_file);
}


int copy_file (char * input, char * output)
{
    assert(input);
    assert(output);

    int inputFd = open(input, O_RDONLY);
    
    if (inputFd  == -1) 
    {
        error(0, errno, "%s", input);
        return -1;
    }
    
    int outputFd = create_file(output, PERMS);

    int size = read_file(inputFd, buf, BUF_SIZE);
    
    write_file(outputFd, buf, size);  

    close_file(inputFd);
    close_file(outputFd);

    return 0;
}

int dir_copy (char * name_file, char * dir_file)
{
    assert(name_file);
    assert(dir_file);

    char * path_file = (char *) calloc(strlen(get_path(name_file, dir_file)), sizeof(char));
    
    path_file = get_path(name_file, dir_file);

    printf("path = %s\n", path_file);

    copy_file(name_file, path_file);

    free(path_file);

    return 0;
}


int main(int argc, char *argv[])
{
    
    int size = 0;
    //int optind = 0;
    int optind = my_getopt(argc, argv);
    
    printf("optind = %d\n", optind);
    
    if (optind < 0)
        return -1; 


    int new_argc = argc - optind;
    printf("new_argc = %d\n", new_argc);
    if (new_argc >= 2)
    {
        /**
         * if not a directory
         */
        if (opendir(argv[argc-1]) == NULL)
        {
            //perror("OPENDIR");
            if ((errno == ENOTDIR) || (errno == ENOENT))  
                if (new_argc == 2)
                {
                    printf("BAD\n");
                    //dir_copy(argv[optind], argv[argc-1]);
                    copy_file(argv[optind], argv[argc-1]);
                    //return 0;                    
                }
                else
                {
                    fprintf(stderr, "%s: указаная цель %s не является каталогом\n", argv[0], argv[argc-1]);
                    //error(0, errno, "%s", argv[argc-1]);
                    return -1;
                }
        }
        /**
         * if a directory
         */
        else
        {
            for (optind; optind < argc-1; optind++)
            {
                dir_copy(argv[optind], argv[argc-1]);
                printf("> %s\n", argv[optind]);
                //copy_file(argv[optind], argv[argc-1]);
            }
        }
        
    }
    else  // new_argc == 0 or new_argc == 1 
    {
        fprintf(stderr, "%s: пропущен операнд, задающий файл\n", argv[0]);
        return -1;
    }

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