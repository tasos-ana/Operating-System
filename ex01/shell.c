#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "shell.h"
#include "execution.h"

#define TRUE 1
#define LEN(x) (sizeof(x) / sizeof(*(x)))

int status;
char *tokens[128];
char** buff;

void display_prompt(void){
	char temp[1024];
	char* user_name = getlogin();
	char* dir_name  = getcwd(temp,1024);

	assert(user_name!=NULL);
	assert(dir_name!=NULL);

	printf("%s@cs345sh%s/$ ",user_name,dir_name);
}

void tokenize(char *s) { /*tokenization of input arguments*/
	char *p, *last;
	int i = 0;

	for ((p = strtok_r(s, " ", &last)); p;
	    (p = strtok_r(NULL, " ", &last))) {
		if (i < LEN(tokens) - 1) {
			tokens[i] = p;
			i++;
		}
	}
	tokens[i] = NULL;
}

char** parse_command(void){
	char buf[1024];

	fgets(buf,sizeof(buf),stdin);
	buf[strcspn(buf,"\n")] = '\0';
	tokenize(buf);
	
	return tokens;
}

void execute_cmd(void){
	if(buff[0]==NULL) return;
	
	if(strcmp(buff[0],"exit")==0){
		execute_exit(buff);
		return;
	}

	if(strcmp(buff[0],"unset")==0){
		execute_unset_var(buff);
		return;
	}

	if(strcmp(buff[0],"set")==0){
		execute_set_var(buff);
		return;
	}

	if(strcmp(buff[0],"printlvar")==0){
		execute_printl_vars(buff);
		return;
	}

	if(strcmp(buff[0],"cd")==0){
		execute_cd(buff);
		return;
	}

	int i=0;
	while(buff[i]!=NULL){
		if(strstr(buff[i],"|")!=NULL){
      		execute_pipe(buff);
      		return;
   		}
    	if(strstr(buff[i],"<")!=NULL){
      		execute_redirection(buff);
      		return;
    	}
    	if(strstr(buff[i],">")!=NULL){
      		execute_redirection(buff);
      		return;
    	}
    	if(strstr(buff[i],"<<")!=NULL){
      		execute_redirection(buff);
      		return;
    	}
		i++;
	}

	if(strcmp(buff[0],"ls")==0 ||
       strcmp(buff[0],"echo")==0 ||
       strcmp(buff[0],"cat")==0 ||
       strcmp(buff[0],"mv")==0 ||
       strcmp(buff[0],"rm")==0 ||
       strcmp(buff[0],"cp")==0 ||
       strcmp(buff[0],"sort")==0 ||
       strcmp(buff[0],"head")==0 ||
       strcmp(buff[0],"grep")==0)
	{
		execute_simple(buff);
		return;
	}

	fprintf(stderr, "The command it's wrong.\n" );
}

int main(int argc, char const *argv[]){
	while(TRUE){
		display_prompt(); /*display prompt on screen*/

		buff = parse_command(); /*read input from terminal*/

		execute_cmd();

	}
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