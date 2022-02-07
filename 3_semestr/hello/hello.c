#include <stdio.h>
#include <unistd.h>

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

int main(int argc, char const *argv[])
{
	char hello[] = "HELLO WORLD";
	char bye[] = "GOODBYE";

	for (int i = 0; i < sizeof(hello); ++i)
	{
		write_file(1, hello+i, 1);
		sleep(1);
	}	

	// if (kill(0, ))
	return 0;
}


