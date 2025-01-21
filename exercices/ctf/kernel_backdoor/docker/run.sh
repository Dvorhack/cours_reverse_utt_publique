#!/bin/sh
qemu-system-x86_64 -m 1G -kernel /app/kernel -initrd /app/initrd\
 -virtfs local,path=/bin,mount_tag=bin,security_model=passthrough,readonly=on\
 -virtfs local,path=/var,mount_tag=var,security_model=passthrough,readonly=on\
 -virtfs local,path=/usr,mount_tag=usr,security_model=passthrough,readonly=on\
 -virtfs local,path=/etc/alternatives,mount_tag=alternatives,security_model=passthrough,readonly=on\
 -nographic -monitor none -append "console=ttyS0 panic=-1" -no-reboot

exit