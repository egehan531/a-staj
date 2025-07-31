obj-m := my_gpio.o

KDIR := /home/ytigli/bbb-kernel
CROSS_COMPILE := /usr/bin/arm-linux-gnueabihf-
ARCH := arm

all:
    $(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules

clean:
    $(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) clean
