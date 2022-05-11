## What?

### U50 

Linux kernel module for the Adesto/Dialog U60 USB to Lon interface, patched to work with Linux kernel 5.4.0

Source:

https://www.dialog-semiconductor.com/sites/default/files/u60_driver_source_2018-12-14.zip

### U61

No idea some random code from Dialog for some unknown gear

### lonifd

daemon process to manage the LON network interface via the serial link

### Compilation, usage

Install the kernel sources

```
make -C /lib/modules/`uname -r`/build KDIR=/lib/modules/`uname -r`/build SUBDIRS=$PWD M=$PWD modules
insmod u50.ko
lonifd
ls /dev/lon*
ifconfig -a # shall list the lon interface
```

See the README file from an old Echelon package in this repo

### License

Unknown, I do not give a shit about it.

Copyright 2017 Echelon Corp present in several files...

Use of this code is subject to the terms of the Echelon Example Software License
Agreement which is available at www.echelon.com/license/examplesoftware/.
