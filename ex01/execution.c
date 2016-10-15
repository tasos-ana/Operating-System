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

#define LEN(x) (sizeof(x) / sizeof(*(x)))
/*
 *Defined on shell.c
 */
extern char* home_dir;
extern char** parse_command(void);

/*Flags that defined on file shell.c
 */
extern int deamon_f;
extern int redirection_f;	
extern int input_redirection_f;	
extern int output_redirection_f;
extern int append_redirection_f;
extern int pipe_f;

extern void execute_cmd(char **buf);

int status;
int input_redirection_index = -1; //the number of token that found '<' on redirection cmd
int output_redirection_index = -1;//the number of token that found '>' on redirection cmd
int append_redirection_index = -1;//the number of token that found '>>' on redirection cmd
int file_redirection_index = -1; ////the number of token that we add the file name for double redirection '<' '>' or '<' '>>'
char* ret[128];

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
 * Do all the action for a simple commants like ls,mv,rm,cat,sort...
 */
void execute_simple(char **buff){
	assert(buff[0]!=NULL);
	int pid;
	char* tmp[128];
	int i=0;
	int j=0;
	
	while(buff[i]!=NULL){//copy the data from buff to a tmp array of string (because after fork the buff contain trashes)
		tmp[i] = strdup(buff[i]);
	 	i++;
	}
	tmp[i] = NULL;
	
	pid = fork();
	if(pid>0){ // pid>0  = father and pid = progress id
		if(!deamon_f) waitpid(-1,&status,0);	//if that flag it's on we dont w8 cause it's deamon proccess
		else{
			fprintf(stdout, "%s:%d\n",tmp[0],pid); //if we come here mean that our proccess are deamon => print the name and pid
		}
	}else if(pid==0){// pid=0 => we are on child
		char* input_fname;
		char* output_append_fname;
		if(redirection_f){//if we have simple cmd from redirection
			if(input_redirection_f){// if we have atleast input, maybe we have output or append or nothing
				input_fname = strdup(tmp[input_redirection_index]);//copying the name of the input file
				tmp[input_redirection_index] = NULL;//nulifie that cell because isn't parameter of the cmd
				
				input_redirection(input_fname);

				//keep the file name if exist for output or append
				free(input_fname);
				input_fname=NULL;
				if(output_redirection_f || append_redirection_f){//if we have output or append take the file name from tmp 
					input_fname = strdup(tmp[input_redirection_index+1]);
					free(tmp[input_redirection_index+1]);
					tmp[input_redirection_index+1] = NULL;
				}
				
				//add the exported data from file that return to ret in to the tmp
				i = input_redirection_index;
				j=0;
				while(ret[j]!=NULL){
					tmp[i] = strdup(ret[j]);
					free(ret[j]);
					ret[j]=NULL;
					i++;
					j++;
				}
				tmp[i] = NULL;

				//redirect stdout to the file
				if(output_redirection_f){//if we have output after input ( cmd<file1>file)
					output_redirection(input_fname);
					free(input_fname);
					input_fname=NULL;
				}
				//redirect stdout to append on file
				if(append_redirection_f){//if we have append after input (cmd<file>>file)
					append_redirection(input_fname);
					free(input_fname);
					input_fname=NULL;
				}
			}else if(output_redirection_f){//if we have only one redirection and it's output
				output_redirection(tmp[output_redirection_index]);//redirect stdout
				free(tmp[output_redirection_index]);
				tmp[output_redirection_index] = NULL;
			}else{//if we have only one redirection and it's append
				append_redirection(tmp[append_redirection_index]);
				free(tmp[append_redirection_index]);
				tmp[append_redirection_index] = NULL;
			}
			char bin_path[128];
			strcpy(bin_path,"/bin/");
			strcat(bin_path,tmp[0]);
			execve(bin_path,&tmp[0],0);//run the command

			redirection_f = 0; //reset flag
		}
		if(pipe_f){

		}		
		if(strcmp(tmp[0],"echo")==0){//checking if the simple command are the echo
			if (tmp[1]!=NULL){//echo $var (cant be like echo)
				if(tmp[1][0] == '$'){// if the parameter it's local var
					char* cmd;
					cmd = get_lvar_cmd(&(tmp[1][1]));//getting from struck the node with $name and return the cmd
					if(cmd!=NULL){
						fprintf(stdout, "%s\n",cmd);
						exit(EXIT_SUCCESS); // sending the success signal on father so he can unblocked from wait
					}
				}
			}
		}
		char bin_path[128];
		/*
		 *Calculate the path for simple cmd /bin/cmd
		 */
		strcpy(bin_path,"/bin/");
		strcat(bin_path,tmp[0]);
		execve(bin_path,&tmp[0],0);//run the command
	}else{
		perror("Fork failed\n");
	}
	
	i=0;
	while(tmp[i]!=NULL){//clear the tmp array that we allocate on function start
	  free(tmp[i]);
	  tmp[i]=NULL;
	  i++;
	}
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
 *Code for pipes
 */
void execute_pipe(char **buff){
	printf("pipe\n");
}

/*
 * Code for redirection, input,output or both 
 */
void execute_redirection(char** buff){
	scout_buff(buff); //checking what redirection contained in our cmd, <,> or <>
	char* cmd[128];//store the tokes for the cmd
	char* file[128];//store the tokes for the file
	int i,j;

	cmd[0] = NULL;
	file[0] = NULL;

	/*
	* 	if we dont have < or > that mean that user type wrong command
	*	like ls -l <file.txt (<file.txt with out space)
	*/
	if(!input_redirection_f && !output_redirection_f && !append_redirection_f){
		fprintf(stderr, "The command it's wrong.\n");
		return;
	}

	redirection_f = 1;//on the flag so we can known on simple cmd that it's simple from redirection

	//if the most left redirection that found it's < (input)
	if(input_redirection_f){

		//exporting from buffer the cmd
		for (i = 0; i < input_redirection_index; ++i){
			cmd[i] = buff[i];
		}
		
		cmd[i] = buff[i+1];//and after the last token that adden on cmd the file token (i=input_redirection_index)
		i++;//moving on next cell that we will add the next token if that exist

		//if the most left redirection toke it's < (input) then we can have later only 1 >(output) or >>(append)
		if(output_redirection_f){//if we have output
			assert(!append_redirection_f);//asserting for append
			cmd[i] = buff[output_redirection_index+1]; //add the name of file that we want to output the command
			i++;//moving on next free cell for upcoming token
		}

		//if the most left redirection toke it's < (input) then we can have later only 1 >(output) or >>(append)
		if(append_redirection_f){//if we have append
			assert(!output_redirection_f);//asserting for append
			cmd[i] = buff[append_redirection_index+1]; //add the name of file that we want to append the command
			i++;
		}
		cmd[i] = NULL;//nullifie the cell after the last token so we can known when to stop
		execute_cmd(cmd);//execute the simple command
	}else{//we have output or append
		int index;
		if(output_redirection_f) index = output_redirection_index;
		else index = append_redirection_index;

		for(i=0; i<index; ++i){
			cmd[i] = buff[i];
		}
		cmd[i] = buff[i+1];//add the file name on cmd
		i++;
		cmd[i] = NULL;
		cmd[i] = NULL;//nullifie the cell after the last token so we can known when to stop
		execute_cmd(cmd);//execute the simple command
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
				return;
			}else{
				output_redirection_f = 1;//on the flag that we found output
				output_redirection_index = i;// where we found i
			}
		}
		i++;
	}
}

/*
 *Call this function to tokenize,parse the data from file when we have input redirection
 */
void input_redirection(char* file){
	char buf[1024];
	FILE *fp;
	fp = fopen(file,"r");

	//it's like tokenize that parsing command, we use it to parse file
	int j = 0;
	while(fgets(buf,sizeof(buf),fp)!=NULL){
		buf[strcspn(buf,"\n")] = '\0';
		char *p, *last;
		//using the strtok_r to break in token the s each time we found "space"
		for ((p = strtok_r(buf, " ", &last)); p;(p = strtok_r(NULL, " ", &last))){
			if (j < LEN(ret) - 1) {
				ret[j] = strdup(p);//store the token on global token array
				j++;
			}
		}
		last = ret[j -1];
		ret[j] = NULL;
	}
	fclose(fp);
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
	int fd = open(file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
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
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */