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

/*
 *Defined on shell.c
 */
extern char* home_dir;

/*Flags that defined on file shell.c
 */
extern int deamon_f;
extern int redirection_f;	
extern int input_redirection_f;	
extern int output_redirection_f;
extern int pipe_f;

extern void execute_cmd(char **buf);

int status;
int input_redirection_index = -1; //the number of token that found < on redirection
int output_redirection_index = -1;//the number of token that found > on redirection

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
		if(redirection_f && output_redirection_f){//if we are on cmd like => cmd > file
			/*Creating a file descriptor so we can get from fd 1 (stdout)
	 		the output from execv
			O_RDWR : open the file for read & write
			O_CREAT: create the file and open it if isnt exist	
			S_IRUSR: give to user permission to read
			S_IwUSR: give to user permission to write
			*/
			int fd = open("tempfile.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			dup2(fd,1);
			close(fd);
		}
		if(pipe_f){

		}		
		if(strcmp(tmp[0],"echo")==0){//checking if the simple command are the echo
			if (tmp[1]!=NULL){// echo $var (cant be like echo)
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
void execute_redirection(char **buff){
	scout_buff(buff); //checking what redirection contained in our cmd, <,> or <>
	char* cmd[128];//store the tokes for the cmd
	char* file[128];//store the tokes for the file
	int i=0;
	int j=0;

	cmd[0] = NULL;
	file[0] =  NULL;

   /*
	* 	if we dont have < or > that mean that user type wrong command
	*	like ls -l <file.txt (<file.txt with out space)
	*/
	if(!input_redirection_f && !output_redirection_f){
		fprintf(stderr, "The command it's wrong.\n");
		return;
	}

	/*
	 * Action to collect all the tokens for command and file/s
	 */
	if(input_redirection_f){// if we have only input (cmd < file)
		for(i=0;i<input_redirection_index;i++){//extract from buff the command tokens on cmd array
			cmd[i] = buff[i];
		}
		cmd[i]= NULL;
		if(output_redirection_f){//extract from buff the file tokens on file array => case: cmd < file1 > file2
			for(i=input_redirection_index+1;i<output_redirection_index;i++){ //starting from    ^ until ^
				file[i] = buff[i]; 
			}
			file[i]= NULL;
		}else{// extract from buff the file tokens on file array => case:  cmd < file
			i=input_redirection_index+1;
			while(buff[i]!=NULL){
				file[j] = buff[i];
				i++;
				j++;
			}
			file[j]= NULL;
		}
	}else{// if we have only input (cmd > file)
		for(i=0;i<output_redirection_index;i++){
			cmd[i] = buff[i];//extracting the command
		}
		cmd[i]= NULL;

		i=output_redirection_index+1;// extract the file name on file array
		j=0;
		while(buff[i]!=NULL){
			file[j] = buff[i];
			i++;
			j++;
		}
		file[j]= NULL;
	}


	/*
	 * Start running cmd depends on state
	 */
	if(input_redirection_f && !output_redirection_f){//State 1 : we have only input (cmd<file)
		assert(input_redirection_index>=0);
		i=0;
		while(file[i]!=NULL){ //append pn cmd array the file array
			cmd[input_redirection_index + i] = file[i];
			i++;
		}
		cmd[input_redirection_index+i] = NULL;
		execute_cmd(cmd); //execute the command
	}else if(!input_redirection_f && output_redirection_f){//State2: we have only output (cmd>file)
		assert(output_redirection_index>=0);
		redirection_f = 1; //open the flag for redirection (look here on execute_simple why)
		execute_cmd(cmd); //execute the command and get the output from stdout in file
		output_redirection(file[0]); // open the file with output and the destination file and tranfer the data
	}else{//State3: we have both input/output (cmd<file1>file2)

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
	while(buff[i]!=NULL){
		if(strcmp(buff[i],"<")== 0){
			input_redirection_f = 1;//on the flag that we found input
			input_redirection_index = i;// where we found it
		}
		if(strcmp(buff[i],">")==0){
			output_redirection_f = 1;//on the flag that we found output
			output_redirection_index = i;// where we found it
		}
		i++;
	}
}

void input_r(void){

}

/*
 * Open 2 files and transfer the data from tempfile to file.
 * tmpfile.txt contain the out from the cmd that runned
 */
void output_redirection(char* file){
	FILE *fp1;
	fp1 = fopen("tempfile.txt","r");// r its for read

	FILE *fp2;
	fp2 = fopen(file,"w+");// w+ it's for write

	char temp[255];

	while(fgets(temp,sizeof(temp),fp1)!=NULL){//getting the data from tempfile line by line
		fprintf(fp2,temp);//write it on file
	}
	fclose(fp1);//close fp1 and fp2
	fclose(fp2);
	remove("tempfile.txt");// removing the temporary file that we create before
}

void append_r(void){

}

/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */