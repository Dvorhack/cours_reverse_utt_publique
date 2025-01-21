obj-m = backdoor.o
CFLAGS_backdoor.o := -O1 -fno-inline-small-functions -fno-inline-functions-called-once -fno-inline-functions -fno-reorder-blocks -fno-reorder-functions -fno-unwind-tables -fno-asynchronous-unwind-tables
all:
	make -C /lib/modules/6.11.0-8-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/6.11.0-8-generic/build M=$(PWD) clean