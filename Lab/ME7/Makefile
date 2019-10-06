obj-m := lab7_part1_salmon.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
# CFLAGS_lab7_part1_salmon.o := -W -Wall

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
