
obj-m := led.o  
KERNEL_DIR := /home/wzm/a7/MYiR-iMX-Linux

PWD := $(shell pwd)  

all:  
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules  
	
clean:  
	rm *.o *.ko *.mod.c  
