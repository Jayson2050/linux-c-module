#
# Makefile for the mcc device driver.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
# Note 2! The CFLAGS definition is now in the main makefile...

# export CC = gcc
export CC = /xel_work_local/toolchain/gcc-linaro-4.9-2016.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
CFLAGS+= -I. 

SRC += debug_print.c

OBJ = $(SRC:.c=.o)
BIN := debug_print.exe

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) -g -W $(CFLAGS)
	rm -r *.o
clean:
	rm -f *.o $(BIN)
