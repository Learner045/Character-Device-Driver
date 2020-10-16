obj-m+=cDevice.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) testcDevice.c -o testcDevice
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm process_list