/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "shell.h"
#include "execution.h"

 #define TRUE 1

extern int stdout_copy;

char* home_dir = NULL;

 /*
 *Required flags
 */
int deamon_f;
int redirection_f;
int pipe_f;int input_redirection_f;
int output_redirection_f;
int append_redirection_f;
int var_f;

int status;

/*
 *Displaying the prompt
 */
void display_prompt(void){
	char temp[1024];
	char* user_name = getlogin();//getting the user name
	char* curr_dir = getcwd(temp,1024);//getting the current path
	
	if(home_dir==NULL){
		home_dir = strdup(curr_dir);//keep the home directory
	}
	assert(user_name!=NULL);
	assert(curr_dir!=NULL);

	printf("%s@cs345sh%s/$ ",user_name,curr_dir);
}

/*
 * Make tokens from speciefied string s
 */
 char** tokenize(char* s, const char* c){
 	char *p,*last;
 	int i = 0;
 	char** tokens = malloc(256*sizeof(char*));

 	for( (p = strtok_r(s, c, &last)); p; (p = strtok_r(NULL, c, &last)) ){//using the strtok_r to break in token the s each time we found "space"
 		tokens[i] = strdup(p);//store the token on global token array
 		i++;
 	}
 	last = tokens[i-1];
 	if(last[strlen(last)-1] == '&'){//if we are on last token check if the cmd it's deamon
 		char tmp[128];
 		strncpy(tmp,last,strlen(last)-1);
 		tokens[i-1] = tmp;
 		deamon_f = 1;//on the deamon flag
 	}
 	tokens[i] = NULL;

 	return tokens;
 }

/*
 * Read the command and extract tokens from that cmd
 */
 char** parse_command(void){
 	char buff[1024];

 	fgets(buff,sizeof(buff),stdin);//read the next line from stdin
 	buff[strcspn(buff,"\n")] = '\0';

 	return tokenize(buff," ");
 }

/*
 * Dispatching each command and call the correct function
 */
int execute_cmd(char **buff){
	if(buff[0]==NULL) return;//if just press enter do nothing
	
	//if cmd = exit
	if(strcmp(buff[0],"exit")==0){
		execute_exit(buff);
		return 1;
	}

 	//if cmd = unset
	if(strcmp(buff[0],"unset")==0){
		execute_unset_var(buff);
		return 1;
	}

	//if cmd = set
	if(strcmp(buff[0],"set")==0){
		execute_set_var(buff);
		return 1;
	}

	//if cmd = printlvar
	if(strcmp(buff[0],"printlvar")==0){
		execute_printl_vars(buff);
		return 1;
	}

	//if cmd = cd
	if(strcmp(buff[0],"cd")==0){
		execute_cd(buff);
		return 1;
	}
	return 0;
}

//check for |,>,>>,<
int execute_redirection_pipe(char** buff){
	int i=0;
	while(buff[i]!=NULL){
		if(strstr(buff[i],"|")!=NULL){
      		execute_pipe(buff);
      		return 1;
   		}
   		if(strstr(buff[i],"<<")!=NULL){
      		execute_redirection(buff);
      		return 1;
    	}
    	if(strstr(buff[i],">")!=NULL){
      		execute_redirection(buff);
      		return 1;
    	}
    	if(strstr(buff[i],"<")!=NULL){
      		execute_redirection(buff);
      		return 1;
    	}
		i++;
	}
	return 0;
}

void run_cmd(char** buff){
	int pid;
	if(execute_cmd(buff)) return; //check for exit,set,unset,printlvar,cd
	scout_buff(buff);
	char* cmd;
	char** tmp;

	/*
	 *if we have $var getting the cmd from var, replace \" and replace buff with that cmd
	 */
	if(var_f){
		char* s = merge_tokens(buff);
		buff = tokenize(s,"$");
		cmd = (char*)get_lvar_cmd(buff[0]);

		if(cmd == NULL) perror("var isnt exist");
		destroy_buff(buff);
		tmp = tokenize(cmd,"\"");
		buff = tokenize(tmp[0]," ");
		destroy_buff(tmp);
	}

	pid = fork();
	if(pid>0){// father, pid = proccess id
		if(!deamon_f) waitpid(-1,&status,0);//if that flag it's on we dont w8 cause it's deamon proccess
		else fprintf(stdout, "%s:%d\n",buff[0],pid); //if we come here mean that our proccess are deamon => print the name and pid
		if(stdout_copy!=-1){
			dup2(stdout_copy, 1);
			close(stdout_copy);
			stdout_copy = -1;
		}
	}else if(pid==0){// child
		if(!execute_redirection_pipe(buff)){//check for |,>,>>,<
			execute_simple(buff);
		}
		exit(EXIT_SUCCESS);
	}else{
		perror("Fork failed\n");
	}
}

void destroy_buff(char** buff){
	int i=0;
	while(buff[i]!=NULL){
		free(buff[i]);
		buff[i] = NULL;
		i++;
	}
}

int main(int argc, char const *argv[]){
	char** buff;
	while(TRUE){
		//init the flag = false/0/off
		deamon_f = 0;
		redirection_f = 0;
		pipe_f = 0;
		input_redirection_f = 0;
		output_redirection_f = 0;
		append_redirection_f = 0;
		var_f = 0;
		stdout_copy = -1;

		display_prompt(); /*display prompt on screen*/

		buff = parse_command(); /*read input from terminal*/

		run_cmd(buff);

		//Free buff because we make it with malloc
		if(!deamon_f)destroy_buff(buff);
	}
	free(buff);
	buff = NULL;
	return 0;
}

/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */