#include <stdio.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//background color
#define BBLACK      "\x1B[40m"
#define BRED        "\x1B[41m"
#define BGREEN      "\x1B[42m"
#define BYELLOW     "\x1B[43m"
#define BBLUE       "\x1B[44m"
#define BPURPLE     "\x1B[45m"
#define D_BGREEN    "\x1B[46m"
#define BWHITE      "\x1B[47m"

//end color
#define NONE        "\x1B[0m"

int main()
{
    printf("%sred\n", KRED);
    printf("%sgreen\n", KGRN);
    printf("%syellow\n", KYEL);
    printf("%sblue\n", KBLU);
    printf("%smagenta\n", KMAG);
    printf("%scyan\n", KCYN);
    printf("%swhite\n", KWHT);
    printf("%snormal\n", KNRM);

    printf("%s------background------\n", KNRM);

    printf("%sBBLACK\n", BBLACK);
    printf("%s\n", NONE);
    printf("%sBRED\n", BRED);
    printf("%s\n", NONE);
    printf("%sBGREEN\n", BGREEN);
    printf("%s\n", NONE);
    printf("%sBYELLOW\n", BYELLOW);
    printf("%s\n", NONE);
    printf("%sBBLUE\n", BBLUE);
    printf("%s\n", NONE);
    printf("%sBPURPLE\n", BPURPLE);
    printf("%s\n", NONE);
    printf("%sD_BGREEN\n", D_BGREEN);
    printf("%s\n", NONE);
    printf("%sBWHITE\n", BWHITE);
    printf("%s\n", NONE);


    return 0;
}