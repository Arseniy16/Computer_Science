#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PERMS 0777
#define MSG_ID 324

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

struct msgbuf
{
    long mtype;
    int mtext[1];
};

typedef struct msgbuf Message;

int receive(int key_id, Message* msg)
{
    if (msgrcv(key_id, msg, sizeof(*msg), 1, 0) < 0)
    {
        perror("ERROR with receive");
        return -1;
    }
    
    return msg->mtext[0];
}

int send(int key_id, Message* msg)
{
    if (msgsnd(key_id, msg, sizeof(*msg), 0) < 0)
    {
        perror("ERROR with send");
        return -1;
    }
}

int main()
{
    Message msg;
    msg.mtype = 1;
    int status = 0;
    int key = msgget(MSG_ID, PERMS | IPC_CREAT | IPC_EXCL);
    if(key < 0)
    {
        key = msgget(MSG_ID, PERMS);
        status = receive(key, &msg);
    }
    msg.mtext[0] = status + 1;
    send(key, &msg);

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

    status = receive(key, &msg);
    msg.mtext[0] = status - 1;
    send(key, &msg);

    // msgctl(key, IPC_RMID, (struct msqid_ds *)0);

    return 0;
}
