CC=gcc
CFLAGS=-I

vm: vm.c
	$(CC) -o vm vm.c

%.bin:%.assm
	touch $@
	python3 python_assembler/assembler.py $(PWD)/$<

