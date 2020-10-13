
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
#include <signal.h>

#include "log.h"

#define DEBUG(...) printf(__VA_ARGS__)

void sig_func(int sig_num)
{
	DEBUG("... %s %s signal:%d \n", __FILE__, __func__, sig_num);
	exit(0);
}

int main(int argc, char **argv)
{
	DEBUG("...my project start\n");

	signal(SIGTERM, sig_func);
	signal(SIGINT, sig_func);
	signal(SIGQUIT, sig_func);

	log_init();
	log_sys(".. %s %s \n",  __DATE__, __TIME__);

	return 0;
}
