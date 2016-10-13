#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "shell.h"
#include "execution.h"

#define TRUE 1
#define LEN(x) (sizeof(x) / sizeof(*(x)))

char *tokens[128];
char tmp[128];
char* home_dir = NULL;
int deamon_f;
int redirection_f;
int pipe_f;

void display_prompt(void){
	char temp[1024];
	char* user_name = getlogin();
	char* curr_dir = getcwd(temp,1024);
	
	if(home_dir==NULL){
		home_dir = strdup(curr_dir);
	}
	assert(user_name!=NULL);
	assert(curr_dir!=NULL);

	printf("%s@cs345sh%s/$ ",user_name,curr_dir);
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
	last = tokens[i-1];
	if(last[strlen(last)-1] == '&'){/*eimaste sto teleftaio token*/
		strncpy(tmp,last,strlen(last)-1);
		tokens[i-1] = tmp;
		deamon_f = 1;
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

void execute_cmd(char **buf){
	if(buf[0]==NULL) return;
	
	if(strcmp(buf[0],"exit")==0){
		execute_exit(buf);
		return;
	}

	if(strcmp(buf[0],"unset")==0){
		execute_unset_var(buf);
		return;
	}

	if(strcmp(buf[0],"set")==0){
		execute_set_var(buf);
		return;
	}

	if(strcmp(buf[0],"printlvar")==0){
		execute_printl_vars(buf);
		return;
	}

	if(strcmp(buf[0],"cd")==0){
		execute_cd(buf);
		return;
	}

	int i=0;
	while(buf[i]!=NULL){
		if(strstr(buf[i],"|")!=NULL){
      		execute_pipe(buf);
      		return;
   		}
    	if(strstr(buf[i],"<")!=NULL){
      		execute_redirection(buf);
      		return;
    	}
    	if(strstr(buf[i],">")!=NULL){
      		execute_redirection(buf);
      		return;
    	}
    	if(strstr(buf[i],"<<")!=NULL){
      		execute_redirection(buf);
      		return;
    	}
		i++;
	}

	if(strstr(buf[0],"ls")!=NULL ||
	   strcmp(buf[0],"echo")==0 ||
	   strcmp(buf[0],"cat")==0 ||
	   strcmp(buf[0],"mv")==0 ||
	   strcmp(buf[0],"rm")==0 ||
	   strcmp(buf[0],"cp")==0 ||
	   strcmp(buf[0],"sort")==0 ||
	   strcmp(buf[0],"head")==0 ||
	   strcmp(buf[0],"grep")==0 ||
	   strcmp(buf[0],"mkdir")==0)
	{
		execute_simple(buf);
		return;
	}

	fprintf(stderr, "The command it's wrong.\n" );
}

int main(int argc, char const *argv[]){
	char** buff;
	while(TRUE){

		deamon_f = 0;
		redirection_f = 0;
		pipe_f = 0;
		display_prompt(); /*display prompt on screen*/

		buff = parse_command(); /*read input from terminal*/

		execute_cmd(buff);

	}
	return 0;
}