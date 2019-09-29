obj-m := lab6_cpuinfo_salmon.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CFLAGS_cpuinfo.o := -O2 -W -Wall -Wextra -Wpedantic

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
