#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "shell.h"
#include "datastructs.h"
#include "dispatcher.h"
#include "parser.h"

#define TRUE 1

int status;
extern lvar_p lvar_list;

void display_prompt(void){
	char temp[1024];
	char* user_name = getlogin();
	char* dir_name  = getcwd(temp,1024);

	assert(user_name!=NULL);
	assert(dir_name!=NULL);

	printf("%s@cs345sh%s/$ ",user_name,dir_name);
}

void execute_cmd(char* buff){
	int cmd_type;
	cmd_type = get_cmd_type(buff);
	execute(buff,cmd_type);
}

int main(int argc, char const *argv[]){
		display_prompt(); /*display prompt on screen*/
		char* buff = parse_command(); /*read input from terminal*/

		execute_cmd(buff);

		display_prompt();
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