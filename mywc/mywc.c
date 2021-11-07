/* ------------------------------------------------------------------- */
/*          This is the program which emulates _wc_ in Linux           */
/* It used pipe and communicates throw this pipe using fork processes  */
/* ------------------------------------------------------------------- */ 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 1024 

int QUIET = 0;

struct Counter
{
    size_t lines;
    size_t words;
    size_t bytes;
};

double time_ms(struct timeval start, struct timeval stop)
{
    double sec1 = (stop.tv_usec - start.tv_usec)/1000000.0;
    double sec2 = stop.tv_sec - start.tv_sec;
    return (sec1 + sec2) * 1000.0;
}

int my_getopt(int argc, char ** argv)
{
    static struct option long_options[] =
    {
        {"quiet", no_argument, NULL, 'q'},
        {0,            0,       0,    0 }
    };

    int opt_index = 0;
    int flag = 0;

    while ((flag = getopt_long(argc, argv, "+q", long_options, &opt_index)) != -1)
    {
        switch (flag)
        {
            case 'q':
                QUIET = 1;
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

void PrintCount(int pipefd)
{
    char buf;
    int maybe_word = 0;

    struct Counter info = {0, 0, 0};

    while (read(pipefd, &buf, 1) > 0)
    {
        // for the first word
        if (isalpha(buf) && info.words == 0)
            info.words = 1;

        // for words
        if (maybe_word && !isspace(buf))
        {
            info.words++;
            maybe_word = 0;
        }

        if (isspace(buf))
            maybe_word = 1;

        // print OR not print
        if (!QUIET)
            if (write(STDOUT_FILENO, &buf, 1) < 0)
            {
                perror("ERROR to write");
                exit(EXIT_FAILURE);
            }

        // for lines
        if (buf == '\n')
            info.lines++;

        info.bytes++;
    }

    printf("count lines = %lu\ncount words = %lu\ncount bytes = %lu\n", info.lines, info.words, info.bytes);
}

int main(int argc, char * argv[])
{
    int optind = my_getopt(argc, argv);
        
    if (optind < 0)
        return -1; 

    int new_argc = argc - optind;

    if (new_argc == 0)
    {
        fprintf(stderr, "%s: Write more argumets\n", argv[0]);
        return -1;
    }

    int pfd[2];

    struct timeval start = {0, 0};
    struct timeval stop = {0, 0};

    if (gettimeofday(&start, NULL) < 0)
    {
        perror("ERROR with start time");
        return -1;
    }
    // create pipe
    if (pipe(pfd) < 0)
    {
        perror("ERROR with pipe");
        return -1;
    }

    switch(fork())
    {
        case -1:
            perror("ERROR to fork");
            return -1;

        case 0:     /* child process */
            // read end is unused
            if (close(pfd[0]) < 0)
            {
                perror("ERROR to close pfd[0]");
                return -1;
            }
            // defence check
            if (pfd[1] != STDOUT_FILENO)
            {
                // duplicate stdout on write end of pipe 
                if (dup2(pfd[1], STDOUT_FILENO) < 0)
                {
                    perror("ERROR to duplicate file_descriptor");
                    return -1;
                }
                // close duplicate fd
                if (close(pfd[1]) < 0)
                {
                    perror("ERROR to close pfd[1]");
                    return -1;
                } 
            }
            // call the execution program
            if (execvp(argv[optind], argv+optind) < 0)
            {
                perror("ERROR with execvp");
                return -1;
            }

        default:    /* parent process */
            // write end is unused
            if (close(pfd[1]) < 0)
            {
                perror("ERROR to close pfd[1]");
                return -1;
            }

            PrintCount(pfd[0]);

            // close stdin
            if (close(pfd[0]) < 0)
            {
                perror("ERROR to close pfd[1]");
                return -1;
            }
            // wait to finish child process
            if (wait(NULL) < 0)
            {
                perror("ERROR with wait");
                return -1;
            }
    }

    if (gettimeofday(&stop, NULL) < 0)
    {
        perror("ERROR with stop time");
        return -1;
    }

    printf("TIME: %.3lg ms\n", time_ms(start, stop));

    return 0;
}
