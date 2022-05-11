include ../../../Apollo.cfg

KMODS = u61 u50
KDIR = ../../../$(SYSROOTDEV)/usr/src/linux-$(KVERSION)

all: $(ARCH)

$(ARCH): $(KMODS:%=%.ko)

$(KMODS:%=%.ko):
	sudo ARCH=$(KARCH) CROSS_COMPILE=$(CROSS_COMPILE) make -C $(KDIR) M=`pwd`/$(@:%.ko=%) modules
	mv $(@:%=$(@:%.ko=%)/%) $(@)
	@echo 'XXX Commit new kernel module to git ??? XXX'

install:

clean:
	for i in $(KMODS) ; do (cd $$i && make $@) ; done
