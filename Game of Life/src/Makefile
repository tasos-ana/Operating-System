all: clear gdb compile

clear:
	clear

compile:
	gcc -o game gol.c table.c -pthread

gdb:
	gcc -g gol.c table.c -pthread

clean:
	rm -rf a.out game