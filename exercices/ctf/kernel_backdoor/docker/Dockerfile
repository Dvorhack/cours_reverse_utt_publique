FROM ubuntu:oracular
ARG KVERSION="6.11.0-8-generic"

RUN apt-get update
RUN apt-get install -y linux-headers-${KVERSION} wget tar cpio gzip bzip2 make build-essential autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev libelf-dev libssl-dev libncurses-dev dwarves

RUN mkdir /app
WORKDIR /app

COPY backdoor.c /app/backdoor.c
RUN chmod 700 backdoor.c
COPY Makefile /app/Makefile
RUN chmod 700 Makefile
RUN make

FROM pwntools/pwntools
RUN sudo apt-get update && sudo apt-get install -y wget tar qemu-system-x86 cpio gzip bzip2 curl clang gcc zstd strace nano vim socat file nasm

RUN sudo mkdir /app
RUN sudo chmod 777 /app
WORKDIR /app

RUN wget https://busybox.net/downloads/busybox-1.36.1.tar.bz2

RUN tar xvf busybox-1.36.1.tar.bz2

WORKDIR /app
RUN mkdir initrd_unpacked
WORKDIR /app/initrd_unpacked

WORKDIR /app/busybox-1.36.1
RUN make defconfig
RUN sed -i 's/^CONFIG_TC=y/CONFIG_TC=n/' .config
RUN make -j4 LDFLAGS="--static"
RUN cp /app/busybox-1.36.1/busybox /app/initrd_unpacked/busybox
RUN sudo chmod 755 /app/initrd_unpacked/busybox

RUN mkdir /tmp/kernel
WORKDIR /tmp/kernel
RUN wget https://fr.archive.ubuntu.com/ubuntu/pool/main/l/linux-signed/linux-image-6.11.0-8-generic_6.11.0-8.8_amd64.deb
RUN ar vx linux-image-6.11.0-8-generic_6.11.0-8.8_amd64.deb
RUN sudo tar --use-compress-program=unzstd -xvf data.tar.zst
RUN sudo cp /tmp/kernel/boot/vmlinuz-6.11.0-8-generic /app/kernel
RUN sudo chmod 744 /app/kernel

RUN mkdir /tmp/modules
WORKDIR /tmp/modules
RUN wget http://fr.archive.ubuntu.com/ubuntu/pool/main/l/linux/linux-modules-6.11.0-8-generic_6.11.0-8.8_amd64.deb
RUN ar vx linux-modules-6.11.0-8-generic_6.11.0-8.8_amd64.deb
RUN sudo tar -xvf data.tar
RUN mkdir /app/initrd_unpacked/modules
RUN sudo unzstd /tmp/modules/lib/modules/6.11.0-8-generic/kernel/fs/9p/9p.ko.zst -o /app/initrd_unpacked/modules/9p.ko
RUN sudo chmod 744 /app/initrd_unpacked/modules/9p.ko
RUN sudo unzstd /tmp/modules/lib/modules/6.11.0-8-generic/kernel/net/9p/9pnet.ko.zst -o /app/initrd_unpacked/modules/9pnet.ko
RUN sudo chmod 744 /app/initrd_unpacked/modules/9pnet.ko
RUN sudo unzstd /tmp/modules/lib/modules/6.11.0-8-generic/kernel/net/9p/9pnet_fd.ko.zst -o /app/initrd_unpacked/modules/9pnet_fd.ko
RUN sudo chmod 744 /app/initrd_unpacked/modules/9pnet_fd.ko
RUN sudo unzstd /tmp/modules/lib/modules/6.11.0-8-generic/kernel/net/9p/9pnet_virtio.ko.zst -o /app/initrd_unpacked/modules/9pnet_virtio.ko
RUN sudo chmod 744 /app/initrd_unpacked/modules/9pnet_virtio.ko
RUN sudo unzstd /tmp/modules/lib/modules/6.11.0-8-generic/kernel/fs/netfs/netfs.ko.zst -o /app/initrd_unpacked/modules/netfs.ko
RUN sudo chmod 744 /app/initrd_unpacked/modules/netfs.ko

WORKDIR /app

COPY init /app/initrd_unpacked/init
RUN sudo chmod 700 /app/initrd_unpacked/init

COPY flag /app/initrd_unpacked/UNLOCK_STEP2
RUN sudo chmod 400 /app/initrd_unpacked/UNLOCK_STEP2

RUN mkdir -p /app/initrd_unpacked/root
COPY flag /app/initrd_unpacked/root/UNLOCK_STEP2
RUN sudo chmod 700 /app/initrd_unpacked/root
RUN sudo chmod 400 /app/initrd_unpacked/root/UNLOCK_STEP2

COPY --from=0 /app/backdoor.ko /app/backdoor.ko
RUN cp /app/backdoor.ko /app/initrd_unpacked/backdoor.ko

WORKDIR /app/initrd_unpacked
RUN sudo find . | sudo cpio --create --format='newc' > ../initrd

WORKDIR /app

COPY run.sh /app/run.sh
RUN sudo chmod 755 /app/run.sh

# CMD qemu-system-x86_64 -m 1G -kernel kernel -initrd initrd\
#  -virtfs local,path=/bin,mount_tag=bin,security_model=passthrough,readonly=on\
#  -virtfs local,path=/var,mount_tag=var,security_model=passthrough,readonly=on\
#  -virtfs local,path=/usr,mount_tag=usr,security_model=passthrough,readonly=on\
#  -virtfs local,path=/etc/alternatives,mount_tag=alternatives,security_model=passthrough,readonly=on\
#  -nographic -monitor none -append "console=ttyS0"

ENTRYPOINT ["socat", "TCP-LISTEN:1337,reuseaddr,fork", "EXEC:\"/app/run.sh\""]