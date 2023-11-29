SOURCE = aesdsocket.c

ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	CC = aarch64-none-linux-gnu-gcc
else
	CC = gcc
endif


.PHONY: build
build:
	$(CC) -Wall $(SOURCE) -o aesdsocket

.PHONY: all
all:
	make clean
	make build
	


.PHONY: clean
clean:
	rm -f aesdsocket