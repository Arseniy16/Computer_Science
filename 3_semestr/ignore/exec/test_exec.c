
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

#define BUF_SIZE 1024 

#define PERMS 0666

char buf[BUF_SIZE + 1];

//-------flags--------
//--------------------
int VERBOSE = 0;
int INTERACTIVE = 0;
int FORCE = 0;
//--------------------

int my_getopt(int argc, char ** argv)
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

int main(int argc, char * argv[])
{
    int optind = my_getopt(argc, argv);

    printf("optind = %d, opt_arg = %s\n", optind, optarg);

    return 0;
}