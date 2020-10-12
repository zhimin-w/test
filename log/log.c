
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define LOG_NAME "log.txt"

#define BUFFER_SIZE 64

#define DEBUG(...) printf(__VA_ARGS__)

void log_init(void)
{
	FILE *fp;
	fp = fopen(LOG_NAME, "a+");
	if(fp == NULL)
	{
		DEBUG("...%s %s fopen error \n", __FILE__, __func__);
	}
	fclose(fp);
}

void log_sys(const char *fmt, ...)
{
	FILE *fp;
	va_list va;
	char buff[BUFFER_SIZE];
	int len;

	va_start(va, fmt);

	len = vsnprintf(buff, BUFFER_SIZE, fmt, va);
	
	va_end(va);

	if(len > 0)
	{
		fp = fopen(LOG_NAME, "a+");
		if(fp == NULL)
		{
			DEBUG("...%s %s fopen error \n", __FILE__, __func__);
		}

		fwrite(buff, len, 1, fp);
		fflush(fp);
		fclose(fp);
	}


}
