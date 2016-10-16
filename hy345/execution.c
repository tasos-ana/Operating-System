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

#include <sys/stat.h>
#include <fcntl.h>

#include "execution.h"
#include "datastructs.h"


extern char* home_dir;
extern int status;

/*Flags that defined on file shell.c
 */
extern int deamon_f;
extern int redirection_f;	
extern int input_redirection_f;	
extern int output_redirection_f;
extern int append_redirection_f;
extern int pipe_f;

extern char** tokenize(char *s,const char* c);

int input_redirection_index = -1; //the number of token that found '<' on redirection cmd
int output_redirection_index = -1;//the number of token that found '>' on redirection cmd
int append_redirection_index = -1;//the number of token that found '>>' on redirection cmd
int pipe_index = -1;//the number of token that found '|' on redirection cmd
int pipe_cmd = 0;//0 for left command and 1 for right
int file_redirection_index = -1; ////the number of token that we add the file name for double redirection '<' '>' or '<' '>>'

int stdout_copy = -1;

/* 	Do all the action to exit from prompt
 *	destroy the local var table
 *	free the home dir that malloced
 */ 
void execute_exit(char **buff){
	destroy_lvar();
	free(home_dir);
	exit(0);
}

/*
 *Doing the action for cmd: cd
 */
void execute_cd(char **buff){
	if(buff[1] == NULL){ // if the user give cd then we moved on home directory
		chdir(home_dir);
	}else{
	  if(chdir(buff[1])==-1){// using the chdir to check if the file that want to moved exist
	    fprintf(stderr, "No such file or directory.\n");
	    return;
	  }
	}	
}

/*
 *Code to execute the set var
 */
void execute_set_var(char **buff){
  if(buff[1]==NULL){// if we have command like 'set' it's wrong
  	fprintf(stderr, "The command it's wrong.\n" );
  	return;
  }
  char* saveptr;
  char* var, *ret;
  char cmd[1024];

  var = strtok_r(buff[1],"=",&saveptr);//get the token before the '='

  ret = strtok_r(NULL," ",&saveptr);// getting the token after the '=' and break it in tokens depend on 'space'
  strcpy(cmd,ret);

  int i = 2;
  while(buff[i]!=NULL){
  	strcat(cmd,buff[i]);
  	i++;
  }

  assert(cmd!=NULL);

  if( (cmd[0] == '\"') && (cmd[strlen(cmd)-1] == '\"') ){//checking if the cmd it's syntax correct
	set_lvar(var,cmd);
  }else{// if the '"' missed then the command must be the ls or it's syntax error
  	if(strcmp(cmd,"ls")==0) set_lvar(var,cmd);
  	else fprintf(stderr, "The command it's wrong.\n" );
  }
}


 /*
  *Code to execute the unset var
  */
void execute_unset_var(char **buff){
	if(buff[1]==NULL){//checking for syntax error
  		fprintf(stderr, "The command it's wrong.\n" );
  		return;
  	}
  	int i=1;
  	while(buff[i]!=NULL){//we can unset multiple var like unset var1 var2 ....
  		unset_lvar(buff[i]);//removing the var from the struct list if contained
  		i++;
  	}
}

/*
 *Code to execute the printlv
 */
void execute_printl_vars(char **buff){
	if(buff[1]!=NULL){//checking for syntax error
  		fprintf(stderr, "The command it's wrong.\n" );
  		return;
  	}
	print_lvar();//printing the struct
}

/*
 * Do all the action for a simple commants like ls,mv,rm,cat,sort...
 */
void execute_simple(char **buff){
	if(strcmp(buff[0],"echo")==0){//checking if the simple command are the echo
		if (buff[1]!=NULL){//echo $var (cant be like echo)
			if(buff[1][0] == '$'){// if the parameter it's local var
				char* cmd;
				cmd = get_lvar_cmd(&(buff[1][1]));//getting from struck the node with $name and return the cmd
				if(cmd!=NULL){
					fprintf(stdout, "%s\n",cmd);
					exit(EXIT_SUCCESS); // sending the success signal on father so he can unblocked from wait
				}
			}
		}
	}
	execvp(buff[0],&buff[0]);
}

/*
 * Code for redirection, input,output or both 
 */
void execute_redirection(char **buff){
	scout_buff(buff);
	char** cmd;
	char** input_data;
	char **fname;

	/*
	* 	if we dont have < or > that mean that user type wrong command
	*	like ls -l <file.txt (<file.txt with out space)
	*/
	if(!input_redirection_f && !output_redirection_f && !append_redirection_f){
		fprintf(stderr, "The command it's wrong.\n");
		return;
	}

	char* s = merge_tokens(buff);
	if(append_redirection_f){
		assert(!output_redirection_f);
		stdout_copy = dup(1);
		append_redirection_f = 0;
		cmd = tokenize(s,">>");//export the tokens from buff left,right from'>>'
		free(s);
		s = NULL;
		fname = tokenize(cmd[1]," ");//getting the file name (cmd < file)
		append_redirection(fname[0]);
		cmd[1] = NULL;
		cmd = tokenize(cmd[0]," ");
		
		run_cmd(cmd);
	}

	if(output_redirection_f){
		assert(!append_redirection_f);
		stdout_copy = dup(1);
		output_redirection_f = 0;
		cmd = tokenize(s,">");//export the tokens from buff left,right from'>'
		s = NULL;
		fname = tokenize(cmd[1]," ");//getting the file name (cmd < file)
		output_redirection(fname[0]);
		cmd[1] = NULL;

		cmd = tokenize(cmd[0]," ");
		run_cmd(cmd);
	}

	if(input_redirection_f){
		input_redirection_f = 0;

		cmd = tokenize(s,"<");//export the tokens from buff left,right from'<'
		free(s);
		s = NULL;
		fname = tokenize(cmd[1]," ");//getting the file name (cmd < file)
		input_data = input_redirection(fname[0]);
		cmd[1] = NULL;

		cmd = tokenize(cmd[0]," ");
		run_cmd(merge_cmd_input(cmd,input_data));
	}
}

void execute_pipe(char** buff){
	scout_buff(buff);
}

int* initialize_pipe(void){
	if(!pipe_f) return NULL;
	assert(pipe_f);
	int* pipefd = malloc(2*sizeof(int));

	if(pipe(pipefd)==-1){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	return pipefd;
}

/*
 *Code for pipes
 */
void execute_pipe_father_side(char **buff,int* pipefd){
	pipe_f = 0;
	char** cmd;

	close(pipefd[0]);
	dup2(pipefd[1],STDOUT_FILENO);

	char* s = merge_tokens(buff);
	cmd = tokenize(s,"|");//export the tokens from buff left,right from'>'
	s = NULL;
	run_cmd(tokenize(cmd[0]," "));
}

void execute_pipe_child_side(char** buff,int* pipefd){
	char input;
	char input_buff[1024];
	char** cmd;
	char** tmp;

	close(pipefd[1]);
	dup2(pipefd[0],STDIN_FILENO);

	char* s = merge_tokens(buff);
	cmd = tokenize(s,"|");//export the tokens from buff left,right from'>'
	s = NULL;

	cmd = tokenize(cmd[1]," ");

	while(read(pipefd[0], &input,1) ){
		write(STDOUT_FILENO,&input,1);
	}
	write(STDOUT_FILENO,"\n",1);

	fgets(input_buff,sizeof(input_buff),stdout);
	input_buff[strcspn(input_buff,"\n")] = '\0';

	tmp = tokenize(input_buff," ");

	cmd = merge_cmd_input(cmd,tmp);

	run_cmd(cmd);
}

/*
 *Call this function to tokenize,parse the data from file when we have input redirection
 */
char** input_redirection(char* file){
	char buff[1024];
	char** ret = malloc(128*sizeof(char*));

	FILE *fp;
	fp = fopen(file,"r");

	//it's like tokenize that parsing command, we use it to parse file
	int i = 0;
	char *p, *last;
	while(fgets(buff,sizeof(buff),fp)!=NULL){
		buff[strcspn(buff,"\n")] = '\0';
		//using the strtok_r to break in token the s each time we found "space"
		for ((p = strtok_r(buff, " ", &last)); p;(p = strtok_r(NULL, " ", &last))){
			ret[i] = strdup(p);//store the token on global token array
			i++;
		}
	}
	ret[i] = NULL;
	fclose(fp);
	return ret;
}

/*
 *Redirection the stdout (1) to the file
 */
void output_redirection(char* file){
	/*Creating a file descriptor so we can get from fd 1 (stdout)
	 *the output from execv
	 *O_WRONLY : open the file for write
	 *O_CREAT: create the file and open it if isnt exist	
	 *S_IRUSR: give to user permission to read
	 *S_IwUSR: give to user permission to write
	 */
	int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	dup2(fd,1);
	close(fd);
}

/*
 *Redirectio/append the stdout (1) to the file
 */
void append_redirection(char* file){
	/*Creating a file descriptor so we can get from fd 1 (stdout)
	 *the output from execv
	 *O_WRONLY : open the file for write
	 *O_APPEND : open the file for append
	 *O_CREAT: create the file and open it if isnt exist	
	 *S_IRUSR: give to user permission to read
	 *S_IwUSR: give to user permission to write
	 */
	 int fd = open(file, O_CREAT | O_WRONLY  | O_APPEND,S_IRUSR | S_IWUSR );
	 dup2(fd,1);
	 close(fd);
}


/*
 * Scouting buffer to learn if we have and where input(<) and/or output(>)
 */
void scout_buff(char** buff){
	int i = 0;
	char *tmp;
	while(buff[i]!=NULL){
		if( (tmp = strstr(buff[i],"<")) != NULL){
			if(strlen(tmp)!=1){
				input_redirection_f  =0;
				output_redirection_f =0;
				append_redirection_f =0;
				pipe_f = 0;
				return;
			}
			input_redirection_f = 1;//on the flag that we found input
			input_redirection_index = i;// where we found it
		}
		if( (tmp = strstr(buff[i],">")) != NULL){
			if( (strlen(tmp)==2) && strcmp(tmp,">>")==0){
				append_redirection_f = 1;//on the flag that we found append
				append_redirection_index = i;//where we found it
				i++;
			}
			else if(strlen(tmp)!=1){
				input_redirection_f  =0;
				output_redirection_f =0;
				append_redirection_f =0;
				pipe_f = 0;
				return;
			}else{
				output_redirection_f = 1;//on the flag that we found output
				output_redirection_index = i;// where we found i
			}
		}if( (tmp = strstr(buff[i],"|"))!=NULL ){
			if(strlen(tmp)!=1){
				input_redirection_f  =0;
				output_redirection_f =0;
				append_redirection_f =0;
				pipe_f = 0;
				return;
			}
			pipe_f = 1;
			pipe_index = i;
		}
		i++;
	}
}

char** merge_cmd_input(char** cmd,char** input_data){
	int i;
	while(cmd[i]!=NULL){
		//printf("%s\n",cmd[i]);
		i++;
	}
	int j = 0;

	while(input_data[j]!=NULL){
		cmd[i] = strdup(input_data[j]);
		i++;
		j++;
	}

	return cmd;
}

char* merge_tokens(char** buff){
	char *s = malloc(1024*sizeof(char));
	int i = 1;
	strcpy(s,buff[0]);
	while(buff[i]!=NULL){
		strcat(s," ");
		strcat(s,buff[i]);
		i++;
	}
	return s;
}

/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */