CC=gcc
CFLAGS=-I

vm: vm.c
	$(CC) -o vm vm.c

assembler: assembler.c
	$(CC) -o assembler assembler.c

testImage: makeTestImage.c
	$(CC) -o makeTestImage makeTestImage.c