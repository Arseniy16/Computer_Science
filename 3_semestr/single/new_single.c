#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SEM_ID 9313
#define PERMS 0666

#define TRY(cmd)                \
    if (cmd < 0)                \
    {                           \
        perror("ERROR #cmd");   \
        return -1;              \
    }                           \

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

int main()
{
    int semid;

    semid = semget(SEM_ID, 1, PERMS | IPC_CREAT | IPC_EXCL);
    
    if (semctl(semid, 0, GETVAL, 1))
    {
        char hello[] = "GOODBYE";
        for (int i = 0; i < sizeof(hello); ++i)
        {
            write_file(1, hello+i, 1);
            sleep(1);
        } 
        semctl(semid, 0, SETVAL, 0);
    }

    // semctl(semid, 0, GETVAL, 0)
    // {
        semctl(semid, 0, SETVAL, 1);

        char hello[] = "HELLO WORLD";
        for (int i = 0; i < sizeof(hello); ++i)
        {
            write_file(1, hello+i, 1);
            sleep(1);
        } 

        semctl(semid, 0, SETVAL, 0);
    // }
    
    return 0;
}
/*

    if (semid < 0)
    {
        TRY(semctl(semid, 0, SETVAL, 1));

        char hello[] = "HELLO WORLD";
        for (int i = 0; i < sizeof(hello); ++i)
        {
            write_file(1, hello+i, 1);
            sleep(1);
        } 
    }
    else 
    {
        semctl(semid, 0, SETVAL, 1);

        char hello[] = "HELLO WORLD";
        for (int i = 0; i < sizeof(hello); ++i)
        {
            write_file(1, hello+i, 1);
            sleep(1);
        } 

        semctl(semid, 0, SETVAL, 0);

    }
    return 0;
}

*/