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
#define LEN(x) (sizeof(x) / sizeof(*(x)))

char *tokens[128];
char tmp[128];
char* home_dir = NULL;

/*
 *Required flags
 */
int deamon_f;
int redirection_f;
int pipe_f;
int input_redirection_f;
int output_redirection_f;

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
void tokenize(char *s) { /*tokenization of input arguments*/
	char *p, *last;
	int i = 0;

	for ((p = strtok_r(s, " ", &last)); p;//using the strtok_r to break in token the s each time we found "space"
	    (p = strtok_r(NULL, " ", &last))) {
		if (i < LEN(tokens) - 1) {
			tokens[i] = p;//store the token on global token array
			i++;
		}
	}
	last = tokens[i-1];
	if(last[strlen(last)-1] == '&'){//if we are on last token check if the cmd it's deamon
		strncpy(tmp,last,strlen(last)-1);
		tokens[i-1] = tmp;
		deamon_f = 1;//on the deamon flag
	}
	tokens[i] = NULL;
}

/*
 * Read the command and extract tokens from that cmd
 */
char** parse_command(void){
	char buf[1024];

	fgets(buf,sizeof(buf),stdin);//read the next line from stdin
	buf[strcspn(buf,"\n")] = '\0';//strcspn return the length of buff and put on last cell the '\0'
	tokenize(buf);
	
	return tokens;
}

/*
 * Dispatching each command and call the correct function
 */
void execute_cmd(char **buf){
	if(buf[0]==NULL) return;//if just press enter do nothing
	
	//if cmd = exit
	if(strcmp(buf[0],"exit")==0){
		execute_exit(buf);
		return;
	}

 	//if cmd = unset
	if(strcmp(buf[0],"unset")==0){
		execute_unset_var(buf);
		return;
	}

	//if cmd = set
	if(strcmp(buf[0],"set")==0){
		execute_set_var(buf);
		return;
	}

	//if cmd = printlvar
	if(strcmp(buf[0],"printlvar")==0){
		execute_printl_vars(buf);
		return;
	}

	//if cmd = cd
	if(strcmp(buf[0],"cd")==0){
		execute_cd(buf);
		return;
	}

	//check if we are on cmd with redirection or pipe
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

	//check if we have any simple command 
	if(strcmp(buf[0],"ls")==0 ||
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

	//msg for incorect cmd
	fprintf(stderr, "The command it's wrong.\n" );
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

		display_prompt(); /*display prompt on screen*/

		buff = parse_command(); /*read input from terminal*/

		execute_cmd(buff); //start running the cmds

	}
	return 0;
}

/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */