# Thomas Hendrickson
# COMP111 PROJ1
# Makefile
# A make file to compile the 
# shell

CC = gcc
OBJS = shell.o
CFLAGS = -Wall -lpthread

sysproc: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o shell

clean:
	rm -f *.o core* shell

