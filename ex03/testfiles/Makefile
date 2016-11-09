all: clear test1 test2 test3 test4

clear:
	clear

test1: clear
	gcc -o test1 test_current.c

test2: clear
	gcc -o test2 test_negative.c

test3: clear
	gcc -o test3 test_fork_correct.c

test4: clear
	gcc -o test4 test_fork_wrong.c
	
clean:
	rm -rf a.out test1 test2 test3 test4