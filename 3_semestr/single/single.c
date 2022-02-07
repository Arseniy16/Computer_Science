#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define FULL_ACCESS 0777
#define IDENTIFIER 924551

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

#define TRY(FUNC)               \
    if(FUNC < 0)                \
    {                           \
        perror("ERROR");        \
        exit(-1);               \
    }                           \


struct msgbuf
{
    long mtype;
    int mtext[1];
};

typedef struct msgbuf Message;

int rcv(int key_id, Message* msg)
{
    TRY(msgrcv(key_id, msg, sizeof(*msg), 1, 0));
    return msg->mtext[0];
}

void snd(int key_id, Message* msg)
{
    TRY(msgsnd(key_id, msg, sizeof(*msg), 0));
}

int main()
{
    Message msg;
    msg.mtype = 1;
    int status = 0;
    int key = msgget(IDENTIFIER, FULL_ACCESS | IPC_CREAT | IPC_EXCL);
    if(key < 0)
    {
        key = msgget(IDENTIFIER, FULL_ACCESS);
        status = rcv(key, &msg);
    }
    msg.mtext[0] = status + 1;
    snd(key, &msg);

    if(msg.mtext[0] == 1)
    {
        char hello[] = "HELLO WORLD";
        for (int i = 0; i < sizeof(hello); ++i)
        {
            write_file(1, hello+i, 1);
            sleep(1);
        } 
    }
    else
    {
        char bye[] = "GOODBYE";
        for (int i = 0; i < sizeof(bye); ++i)
        {
            write_file(1, bye+i, 1);
            sleep(1);
        } 
    }

    status = rcv(key, &msg);
    msg.mtext[0] = status - 1;
    snd(key, &msg);

    // TRY(msgctl(key, IPC_RMID, (struct msqid_ds *)0));

    return 0;
}
