/*
 * This is the program which emulates echo + (-n) in Linux 
 */
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int flag = 0;

    int i = 1;

    /*
     * to process the flag "-n" in echo 
     */
    if (argc > 1)
        if ((strcmp(argv[1], "-n") == 0)) 
        {
            flag = 1;
            i = 2;
        }

    for (i; i < argc; i++)
    {
        if (i == argc - 1) 
        {
            printf("%s", argv[i]); // to process the last symbol 
            continue;
        }

        printf((argc > 1) ? "%s " : "%s", argv[i]); 
    }
    
    if (!flag) printf("\n");

    return 0;
}
