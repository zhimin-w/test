
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
#include <sys/select.h>
#include <sys/time.h>

#include "log.h"

#define DEBUG(...) printf(__VA_ARGS__)

void sig_func(int sig_num)
{
	DEBUG("... %s %s signal:%d \n", __FILE__, __func__, sig_num);
	exit(0);
}


void *led_pthread_func(void *argv)
{
	pthread_detach(pthread_self());

	while(1)
	{
		DEBUG(".... %s \n", __func__);
		sleep(1);
	}

	pthread_exit(0);
}

void *key_pthread_func(void *argv)
{
	pthread_detach(pthread_self());

	int ret;
	struct timeval time;
	fd_set fds;
	int file = open("/dev/key", O_RDWR | 0666);	
	if(file)
	{
		DEBUG("... %s open error \n", __func__);
		pthread_exit(0);
		exit(-1);
	}

	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(file, &fds);

		time.tv_sec = 0;
		time.tv_usec = 10000;
	
		ret = select(file +1, &fds, NULL, NULL, &time);
		if(-1 == ret)
		{
			DEBUG(" ... %s select error \n", __func__);
			pthread_exit(0);
			exit(-1);
		}
		else if(0 == ret)
		{
			continue;
		}
		else if(0 < ret)
		{
			if(FD_ISSET(file, &fds))
			{
				DEBUG(" ... select FD_ISSET \n");
			}
		}

	}

	pthread_exit(0);
}


int main(int argc, char **argv)
{
	pthread_t pthread_led, pthread_key;

	DEBUG("...my project start\n");

	signal(SIGTERM, sig_func);
	signal(SIGINT, sig_func);
	signal(SIGQUIT, sig_func);

	log_init();
	log_sys(".. %s %s \n",  __DATE__, __TIME__);


	pthread_create(&pthread_led, NULL, led_pthread_func, NULL);
	pthread_create(&pthread_key, NULL, key_pthread_func, NULL);
	
	while(1);

	return 0;
}
