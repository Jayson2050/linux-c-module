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

SRC_CLIENT += demo1_client.c
SRC_SERVER += demo1_server_multi_thread.c

OBJ_CLIENT = $(SRC_CLIENT:.c=.o)
OBJ_SERVER = $(SRC_SERVER:.c=.o)

all: client server

client: $(OBJ_CLIENT)
	$(CC) -o $@ $(OBJ_CLIENT) -g -W $(CFLAGS) -lpthread
	rm -r *.o

server: $(OBJ_SERVER)
	$(CC) -o $@ $(OBJ_SERVER) -g -W $(CFLAGS) -lpthread
	rm -r *.o

clean:
	rm -f *.o client server
