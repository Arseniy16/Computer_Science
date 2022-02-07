#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define PERMS 0777
#define MSG_ID 324

int read_file(int fd, char * buf, size_t count);

int write_file(int fd, char * buf, size_t count);

int close_file(int fd);

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

int main(int argc, char const *argv[])
{
    Message msg;
    msg.mtype = 1;
    int status = 0;
    int key = msgget(MSG_ID, PERMS | IPC_CREAT | IPC_EXCL);

    int fd[2];
    if (pipe(fd) < 0)
    {
        perror("ERROR with pipe");
        return -1;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        close_file(fd[0]);

        int size = 0;
        long long count = 0;
        while(1)
        {
            //memcpy(buf, "abcdefghi\0", 10);
            size = write(fd[1], "a", 1);
            printf("count = %d\n", count);
            
            if (size < 0)
            {
                printf("good\n");
                printf("count = %d\n", count);
                msg.mtext[0] = count;
                send(key, &msg);
                //printf("count = %d\n", count);

                printf("good\n");
                return 0;

            }
            
            count++;
        }
    }
    else if(pid > 0)
    {
        int retval = fcntl( fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
        close_file(fd[1]);
        
        if(wait(NULL) < 0)
        {
            perror("ERROR to wait");
            return -1;
        }

        status = receive(key, &msg);

        printf("pipe_size = %d\n", msg.mtext[0]);
        
        if (msgctl(key, IPC_RMID, (struct msqid_ds *)0) < 0)
        {
            perror("ERROR to remove msg");
            return -1;
        }

        close_file(fd[0]);

    } 
    else
    {
        perror("ERROR with fork");
        return -1;
    }
    
    return 0;
}


int read_file(int inputFd, char * buf, size_t count)
{
    int size = 0;

    if ((size = read(inputFd, buf, count)) == -1)
    {
        perror("ERROR to read from file");
        return -1;
    }

    return size;
}

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

int close_file(int fd)
{
    if (close(fd) == -1) 
    {
        perror("ERROR to close file_descriptor");
        return -1;
    }
    return fd;
}