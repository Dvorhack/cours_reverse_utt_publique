CHALLENGE := easy_syscall
SRC_PATH := src

.PHONY: default
default: all

.PHONY: all
all: clean $(CHALLENGE)

$(CHALLENGE):
	@mkdir -p dist
	@cp src/binary/backdoor.ko dist/backdoor.ko
	@cp src/connect.sh dist/connect.sh
	@cp src/backdoor.c docker/backdoor.c
	@cp src/Makefile docker/Makefile
	@cp src/flag docker/flag
	@cp src/init docker/init
	@cp src/run.sh docker/run.sh

.PHONY: clean
clean:
	@rm -f docker/backdoor.c docker/Makefile docker/flag docker/init docker/run.sh
	@rm -rf dist/*
