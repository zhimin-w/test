
CC = gcc
#arm-linux-gnueabihf-gcc

OBJ = ./bin

target = ${OBJ}/my_project


source_c = ${wildcard ./log/*.c *.c}
object_c = ${patsubst %.c, ${OBJ}/%.o, ${notdir ${source_c}}}

INCLUDE =-I./log
LIB = -lpthread

${target}:${object_c}	
	${CC} -o  $@ $^  ${INCLUDE} ${LIB}

${OBJ}/%.o:%.c
	${CC} -c  $< -o $@  ${INCLUDE} ${LIB}

${OBJ}/%.o:./log/%.c
	${CC} -c  $< -o $@  ${INCLUDE} ${LIB}

clean:
	rm -rf ${OBJ}/*
