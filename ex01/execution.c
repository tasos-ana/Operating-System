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

/*Flags that defined on file shell.c
 */
extern int deamon_f;
extern int redirection_f;	
extern int input_redirection_f;	
extern int output_redirection_f;
extern int append_redirection_f;
extern int pipe_f;
extern int var_f;

extern char** tokenize(char *s,const char* c);

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
  	strcat(cmd," ");
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
	scout_buff(buff);//scout buffer and check if we have | and with  <space>|<space>
	if(!pipe_f) perror("pipe syntax");

	int pipefd[2];
	pid_t cpid;
	char** cmd;
	int status;

	char* s = merge_tokens(buff);//merging all the tokens to export the cmd1 and cmd2 on cmd[char*]

	cmd = tokenize(s ,"|");

	if(pipe(pipefd)==-1){//creating the pipe and checking for errors
		perror("pipe");
		exit(EXIT_FAILURE);
	}

 	/*
 	* The basic idea is that we make one fork for pipe
 	* on child side redirect the stdout to the pipefd[0] and exec the cmd1
	* on father side waiting the cmd1 to end, and then redirecto the pipe output on stdin and run cmd2
	*/
	cpid = fork();

	if(cpid==0){//child
		dup2(pipefd[1],STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);

		run_cmd(tokenize(cmd[0]," "));
		exit(EXIT_SUCCESS);

	}else if(cpid>0){//father
		waitpid(cpid,&status,0);
		dup2(pipefd[0],STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		run_cmd(tokenize(cmd[1]," "));
	}else{
		perror("pipe fork failed");
	}

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
	 *O_RDWR : open the file for write,read
	 *O_CREAT: create the file and open it if isnt exist	
	 *S_IRUSR: give to user permission to read
	 *S_IwUSR: give to user permission to write
	 */
	int fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	dup2(fd,1);
	close(fd);
}

/*
 *Redirectio/append the stdout (1) to the file
 */
void append_redirection(char* file){
	/*Creating a file descriptor so we can get from fd 1 (stdout)
	 *the output from execv
	 *O_RDWR : open the file for write,read
	 *O_APPEND : open the file for append
	 *O_CREAT: create the file and open it if isnt exist	
	 *S_IRUSR: give to user permission to read
	 *S_IwUSR: give to user permission to write
	 */
	 int fd = open(file, O_CREAT | O_APPEND , S_IRUSR | S_IWUSR );
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
				return;
			}
			input_redirection_f = 1;//on the flag that we found input

		}
		if( (tmp = strstr(buff[i],">")) != NULL){
			if( (strlen(tmp)==2) && strcmp(tmp,">>")==0){
				append_redirection_f = 1;//on the flag that we found appendre we found it
				i++;
			}
			else if(strlen(tmp)!=1){
				input_redirection_f  =0;
				output_redirection_f =0;
				append_redirection_f =0;
				return;
			}else{
				output_redirection_f = 1;//on the flag that we found output
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
		}
		if( (tmp = strstr(buff[i],"$"))!=NULL){
			var_f = 1;
			return;
		}
		i++;
	}
}

/*
 *Getting two tokens arrays and merge it in one
 */
char** merge_cmd_input(char** cmd,char** input_data){
	int i;
	while(cmd[i]!=NULL){
		i++;
		printf("%d\n",i);
	}
	int j = 0;

	while(input_data[j]!=NULL){
		cmd[i] = strdup(input_data[j]);
		i++;
		j++;
	}

	return cmd;
}

/*
 * Export in a string all the tokens from the buff
 */
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