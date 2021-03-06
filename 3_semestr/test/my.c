#include <color.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BOOK_PLACE 0
#define CAR_CAN_GO 1
#define BOAT_CAN_GO 2
#define SHARED_VAR 3
#define START 4

const int NUMBER_SEM = 5;

#define TRY(cmd)                                     \
    if ((cmd) < 0)                                   \
    {                                                \
        perror(#cmd);                                \
        exit(EXIT_FAILURE);                          \
    }

#define UP(SEM_NAME)                                 \
    do                                               \
    {                                                \
        op = {SEM_NAME, 1, 0};                       \
        TRY(semop(semid, &op, 1));                   \
    } while (0);

#define DOWN(SEM_NAME)                               \
    do                                               \
    {                                                \
        op = {SEM_NAME, -1, 0};                      \
        TRY(semop(semid, &op, 1));                   \
    } while (0);

const size_t MAX_WAIT_BOATS = 4;

struct Shm
{
    size_t cur_wait_boats = 0;
    size_t cur_wait_cars = 0;
    size_t cur_passed_boats = 0;
    size_t cur_passed_cars = 0;
    char bridge_status = 0; // 0 - bridge is down
};

void boat(int semid, int pass_name, Shm *shm, unsigned sleep_mcs)
{
    sembuf op{};
    DOWN(START);

    DOWN(SHARED_VAR);
    shm->cur_wait_boats++;

    printf("Boat %d in the queue: ", pass_name);
    printf("%lu near bridge\n", shm->cur_wait_boats);
    // PRINT_CYAN_E(N("Boat %d in the queue: "), pass_name);
    // PRINT_CYAN_E(N("%lu near bridge\n"), shm->cur_wait_boats);

    UP(SHARED_VAR);
    DOWN(BOAT_CAN_GO);

    // CRITICAL
    DOWN(BOOK_PLACE)
    printf("Boat %d under the bridge: ", pass_name);

    DOWN(SHARED_VAR);
    printf("%lu near bridge\n", shm->cur_wait_boats);
    UP(SHARED_VAR);

    DOWN(SHARED_VAR);
    shm->cur_wait_boats--;
    shm->cur_passed_cars = 0;
    UP(SHARED_VAR);

    printf("Boat %d passing the bridge\n", pass_name);
    usleep(sleep_mcs);
    printf("Boat %d passed the bridge\n", pass_name);

    DOWN(SHARED_VAR);
    shm->cur_passed_boats++;
    UP(SHARED_VAR);

    DOWN(SHARED_VAR);

    if (shm->cur_passed_boats >= 3 && shm->cur_wait_cars != 0)
    {
        UP(CAR_CAN_GO);
        if (shm->bridge_status == 1)
        {
            printf("Bridge is gonna to be downed -> ");
            usleep(sleep_mcs);
            shm->bridge_status = 0;
            printf("Bridge is downed\n");
        }
    }
    else if (shm->cur_wait_boats != 0)
    {
        UP(BOAT_CAN_GO);
        if (shm->bridge_status == 0)
        {
            printf("Bridge is gonna to be upped -> ");
            usleep(sleep_mcs);
            shm->bridge_status = 1;
            printf("Bridge is upped\n");
        }
    }
    else
    {
        UP(CAR_CAN_GO);
        if (shm->bridge_status == 1)
        {
            printf("Bridge is gonna to be downed -> ");
            shm->bridge_status = 0;
            usleep(sleep_mcs);
            printf("Bridge is downed\n");
        }
    }
    UP(SHARED_VAR);

    UP(BOOK_PLACE);
    // CRITICAL
}
void car(int semid, int pass_name, Shm *shm, unsigned sleep_mcs)
{
    sembuf op{};
    DOWN(START);

    DOWN(SHARED_VAR);
    shm->cur_wait_cars++;

    printf("Car %d in the queue: ", pass_name);
    printf("%lu near bridge\n", shm->cur_wait_cars);
    UP(SHARED_VAR);
    DOWN(CAR_CAN_GO);

    // CRITICAL
    DOWN(BOOK_PLACE)
    printf("Car %d on the bridge: ", pass_name);

    DOWN(SHARED_VAR);
    printf("%lu near bridge\n", shm->cur_wait_cars);
    UP(SHARED_VAR);

    DOWN(SHARED_VAR);
    shm->cur_wait_cars--;
    shm->cur_passed_boats = 0;
    UP(SHARED_VAR);

    // printf("boats: %d, cars: %d\n", shm->cur_wait_boats, shm->cur_wait_cars);
    printf("Car %d passing the bridge\n", pass_name);
    usleep(sleep_mcs);
    printf("Car %d passed the bridge\n", pass_name);

    DOWN(SHARED_VAR);
    shm->cur_passed_cars++;
    UP(SHARED_VAR);

    DOWN(SHARED_VAR);
    if (shm->cur_passed_cars >= 3 && shm->cur_wait_boats != 0)
    {
        UP(BOAT_CAN_GO);
        if (shm->bridge_status == 0)
        {
            printf("Bridge is gonna to be upped -> ");
            usleep(sleep_mcs);
            shm->bridge_status = 1;
            printf("Bridge is upped\n");
        }
    }
    else if (shm->cur_wait_cars != 0)
    {
        UP(CAR_CAN_GO);
        if (shm->bridge_status == 1)
        {
            printf("Bridge is gonna to be downed -> ");
            usleep(sleep_mcs);
            shm->bridge_status = 0;
            printf("Bridge is downed\n");
        }
    }
    else
    {
        UP(BOAT_CAN_GO);
        if (shm->bridge_status == 0)
        {
            printf("Bridge is gonna to be upped -> ");
            usleep(sleep_mcs);
            shm->bridge_status = 1;
            printf("Bridge is upped\n");
        }
    }
    UP(SHARED_VAR);

    UP(BOOK_PLACE);
    // CRITICAL
}

int main(int argc, char **argv)
{
    char print_buf[BUFSIZ];
    setvbuf(stdout, print_buf, _IONBF, BUFSIZ);

    if (argc != 4)
    {
        printf("Invalid number of parameters\nDo:./sem <num_boats> <num_cars>\n");
        exit(-1);
    }

    int number_boats = atoi(argv[1]);
    int number_cars = atoi(argv[2]);
    unsigned sleep_mcs = (unsigned)atoi(argv[3]) * 1000;

    if (number_boats < 0 || number_cars < 0)
    {
        printf("Invalid input. Numbers must be positive\n");
        exit(-1);
    }

    int semid = semget(IPC_PRIVATE, NUMBER_SEM, 0777);
    TRY(semid);
    sembuf op{};

    UP(SHARED_VAR);
    UP(BOOK_PLACE);

    // Open bridge
    //  if (number_boats < number_cars)
    UP(CAR_CAN_GO)
    //  else
    //    UP(BOAT_CAN_GO)

    // shared vars
    int shmid = shmget(IPC_PRIVATE, sizeof(Shm), IPC_CREAT | 0777);
    TRY(shmid);

    Shm *shm = (Shm *)calloc(1, sizeof(Shm));
    shm = (Shm *)shmat(shmid, NULL, 0);
    if (shm == NULL || (void *)shm == (void *)-1)
    {
        perror("shmat");
        exit(-1);
    }
    shm->cur_wait_boats = 0;
    shm->cur_wait_cars = 0;

    // for normal start
    op = {START, (short int)(number_boats + number_cars), 0};
    TRY(semop(semid, &op, 1));

    int minn = number_cars > number_boats ? number_boats : number_cars;
    int maxx = number_cars < number_boats ? number_boats : number_cars;

    for (int i = 0; i < minn; ++i)
    {
        pid_t pid = fork();
        TRY(pid);

        if (pid == 0)
        {
            car(semid, i + 1, shm, sleep_mcs);
            return 0;
        }

        pid = fork();
        TRY(pid);

        if (pid == 0)
        {
            boat(semid, i + 1, shm, sleep_mcs);
            return 0;
        }
    }

    // make childs
    for (int i = minn; i < maxx; ++i)
    {
        pid_t pid = fork();
        TRY(pid);

        if (pid == 0)
        {
            if (number_boats > number_cars)
                boat(semid, i + 1, shm, sleep_mcs);

            if (number_boats < number_cars)
                car(semid, i + 1, shm, sleep_mcs);

            return 0;
        }
    }

    for (int i = 0; i < number_boats + number_cars; ++i)
        wait(NULL);

    return 0;
}