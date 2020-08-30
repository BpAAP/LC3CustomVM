CC=gcc
CFLAGS=-I

vm: vm.c
	$(CC) -o vm vm.c

testImage: makeTestImage.c
	$(CC) -o makeTestImage makeTestImage.c