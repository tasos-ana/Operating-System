all: clear gdb compile

clear:
	clear

compile:
	gcc -o cs345sh shell.c execution.c datastructs.c

gdb:
	gcc -g shell.c execution.c datastructs.c

clean:
	rm -rf a.out cs345sh