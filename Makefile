
CC = gcc
#arm-linux-gnueabihf-gcc

OBJ = ./bin

target = ${OBJ}/my_project


source_c = ${wildcard ./log/*.c *.c}
object_c = ${patsubst %.c, ${OBJ}/%.o, ${notdir ${source_c}}}

INCLUDE =-I./log

${target}:${object_c}	
	${CC} -o $@ $^ ${INCLUDE}

${OBJ}/%.o:%.c
	${CC} -c $< -o $@ ${INCLUDE}

${OBJ}/%.o:./log/%.c
	${CC} -c $< -o $@ ${INCLUDE}

clean:
	rm -rf ${OBJ}/*
