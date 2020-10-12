
CC = arm-linux-gnueabihf-gcc

OBJ = ./bin

target = ${OBJ}/my_project


source_c = ${wildcard ./log/*.c *.c}
object_c = ${patsubst %.c, ${OBJ}/%.o, ${notdir ${source_c}}}

${target}:${object_c}	
	${CC} -o $@ $^

${OBJ}/%.o:%.c
	${CC} -c $< -o $@

${OBJ}/%.o:./log/%.c
	${CC} -c $< -o $@

clean:
	rm -rf ${OBJ}/*.o ${target} *~
