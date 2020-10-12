
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define DEBUG(...) printf(__VA_ARGS__)

int main(int argc, char **argv)
{
	DEBUG(" ... main\n");

	return 0;
}
