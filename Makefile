
CC = arm-linux-gnueabihf-gcc

target = my_project

source_c = ${wildcard *.c}
object_c = ${patsubst %.c, %.o, ${notdir ${source_c}}}

${target}:${object_c}
	${CC} -o $@ $^

%.o:%.c
	${CC} -c $< -o $@

clean:
	rm -rf *.o ${target} *~
