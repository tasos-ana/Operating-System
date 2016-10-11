#include "unistd.h"
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "shell.h"

#define TRUE 1
int status;

int main(int argc, char const *argv[])
{
		display_prompt(); /*display prompt on screen*/
		char* buff = parse_command(); /*read input from terminal*/


	/* 	int pid;
		pid = fork();

		if(pid>0){ //pid of the father
			printf("%s\n","hi father");
			waitpid(-1, &status, 0);

		}else if(pid==0){ //pid of the child
			printf("%s\n","i am child");
			execve(command[0],parameters,0);
	
		}else{/Something wrong hapend
			perror("Fork failed\n");
		}
	*/
	return 0;
}