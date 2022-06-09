#
# Makefile for the mcc device driver.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
# Note 2! The CFLAGS definition is now in the main makefile...

export CC = gcc
CFLAGS+= -I. 

SRC += ip4_str_to_hex.c

OBJ = $(SRC:.c=.o)
BIN := ip4_str_to_hex

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) -g -W $(CFLAGS) -lpthread
	rm -r *.o
clean:
	rm -f *.o $(BIN)
