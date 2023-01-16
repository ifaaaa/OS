obj-m += driver.o
KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR)  M=$(shell pwd) modules

clean:
	make -C $(KDIR)  M=$(shell pwd) clean

run:
	make clean
	sudo rmmod driver || true
	make all
	sudo insmod driver.ko


