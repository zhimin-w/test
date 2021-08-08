
///////////// test

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
#include <sys/ioctl.h>

#include "log.h"

#define DEBUG(...) printf(__VA_ARGS__)

void sig_func(int sig_num)
{
	DEBUG("... %s %s signal:%d \n", __FILE__, __func__, sig_num);
	exit(0);
}

#define KEY_SHORT (1 << 0)
#define KEY_LONG  (1 << 1)
unsigned char key_status = 0;

void *led_pthread_func(void *argv)
{
	pthread_detach(pthread_self());

	pthread_mutex_t pthread_mutex;
	pthread_mutex_init(&pthread_mutex, NULL);

	int file = open("/dev/led", O_RDWR | 0666);
	if(file < 0)
	{
		DEBUG(" ... %s open error ....\n", __func__);		
		pthread_exit(0);
		exit(-1);
	}

	while(1)
	{
		
		if(key_status & KEY_LONG)
		{
			pthread_mutex_lock(&pthread_mutex);
			ioctl(file, 1);
			sleep(3);
			ioctl(file, 0);
			key_status &= ~KEY_LONG;
			pthread_mutex_unlock(&pthread_mutex);
		}
		if(key_status & KEY_SHORT)
		{
			pthread_mutex_lock(&pthread_mutex);
			ioctl(file, 1);
			sleep(1);
			ioctl(file, 0);
			key_status &= ~KEY_SHORT;
			pthread_mutex_unlock(&pthread_mutex);
		}
	}

	pthread_exit(0);
}

void *key_pthread_func(void *argv)
{
	pthread_detach(pthread_self());

	int ret;
	struct timeval time;
	fd_set fds;
	char buff[1];
	static int count = 0, flag = 0;

	int file = open("/dev/key", O_RDWR | 0666);	
	if(-1 == file)
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
		time.tv_usec = 50000;
	
		ret = select(file + 1, &fds, NULL, NULL, &time);
		if(-1 == ret)
		{
			DEBUG(" ... %s select error \n", __func__);
			pthread_exit(0);
			exit(-1);
		}
		else if(0 == ret)
		{
			if(flag)
			{
				read(file, buff, 1);
				if(0 == buff[0])
					count++;
				else
				{
					if(count <= 20)
					{
						DEBUG(" ...short key \n");
						key_status |= KEY_SHORT;
					}	
					else
					{
						DEBUG(" ... long key \n");
						key_status |= KEY_LONG;
					}
					count = 0;
					flag = 0;	
				}
				
			}
			
			continue;
		}
		else if(0 < ret)
		{			

			if(FD_ISSET(file, &fds))
			{
				flag = 1;
				//read(file, buff, 1);
						
				//count = 0;				
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
