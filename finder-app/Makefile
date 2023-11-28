SOURCE = writer.c
OBJS = $(SOURCE:.c=.o)

ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	CC = aarch64-none-linux-gnu-gcc
else
	CC = gcc
endif


.PHONY: build
build:
	$(CC) -Wall -Werror $(SOURCE) -o writer

.PHONY: all
all:
	make clean
	make build
	


.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f writer