EXTRA_CFLAGS += -I$(src)/include -DU50_KERNEL -DU50_LAYER5 -g
obj-m += u50.o
u50-y += U50Link.o U50Osal.o U50Log.o U50Driver.o packet_util.o ipv6_ls_to_udp.o ip_util.o
MODDIR = $(shell uname -r)

clean:
	-rm -f *.o .*.cmd .*.flags *.mod.c
-include $(KERNEL)/Rules.make

install:
	install -d /lib/modules/$(MODDIR)/kernel/drivers/lon/
	install -m 644 u50.ko /lib/modules/$(MODDIR)/kernel/drivers/lon
