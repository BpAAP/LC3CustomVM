CC=gcc
CFLAGS=-I

vm: vm.c
	$(CC) -o vm vm.c

