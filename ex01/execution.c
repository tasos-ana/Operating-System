#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "execution.h"
#include "datastructs.h"

extern char* home_dir;
extern char* curr_dir;

int status;

void execute_exit(char **buff){
	destroy_lvar();
	free(home_dir);
	free(curr_dir);
	exit(0);
}

void execute_simple(char **buff){
	printf("2----%s\n",buff[0]);
	char* tmp = strdup(buff[0]);
	int pid;
	pid = fork();
	if(pid>0){
		printf("%s\n","pateras");
		printf("0----%s\n",buff[0]);
		waitpid(-1,&status,0);
	}else if(pid==0){
		/*if(strcmp(buff[0],"echo")==0 && buff[1]!=NULL){
			char* cmd;
			cmd = get_lvar_cmd(buff[1]);
			if(cmd!=NULL){
				printf("%s\n",cmd);
			}
		}*/
		
		char bin_path[128];
		printf("3----%s\n",tmp);
		strcpy(bin_path,"/bin/");
		printf("4----%s\n",tmp);
		strcat(bin_path,buff[0]);
		printf("---%s\n",bin_path);
		char* param[128];
		int i = 1;

		while(buff[i]!=NULL){
			param[i-1] = buff[i];
			i++;
		}
		param[i] = NULL;
		printf("---%s\n",bin_path);
		execve(bin_path,&param[0],0);
	}else{
		perror("Fork failed\n");
	}
}

void execute_cd(char **buff){
	if(buff[1] == NULL){
		chdir(home_dir);
		free(curr_dir);
		curr_dir = strdup(home_dir);
	}else{
		int s1,s2;
		s1 = strlen(curr_dir);
		s2 = strlen(buff[1]);
		char new_dir[s1+s2+2];
		strcpy(new_dir,curr_dir);
		strcat(new_dir,"/");
		strcat(new_dir,buff[1]);

		if(chdir(new_dir)==-1){
			fprintf(stderr, "No such file or directory.\n");
			return;
		}

		free(curr_dir);
		curr_dir = strdup(new_dir);
	}	
}

void execute_pipe(char **buff){
	printf("pipe\n");
}

void execute_redirection(char **buff){
	printf("redirection\n");
}

void execute_set_var(char **buff){
  if(buff[1]==NULL){
  	fprintf(stderr, "The command it's wrong.\n" );
  	return;
  }
  char* saveptr;
  char* var, *ret;
  char cmd[1024];

  var = strtok_r(buff[1],"=",&saveptr);

  ret = strtok_r(NULL," ",&saveptr);
  strcpy(cmd,ret);

  int i = 2;
  while(buff[i]!=NULL){
  	strcat(cmd,buff[i]);
  	i++;
  }
  set_lvar(var,cmd);
}

void execute_unset_var(char **buff){
	if(buff[1]==NULL){
  		fprintf(stderr, "The command it's wrong.\n" );
  		return;
  	}
  	int i=1;
  	while(buff[i]!=NULL){
  		unset_lvar(buff[i]);
  		i++;
  	}
}

void execute_printl_vars(char **buff){
	if(buff[1]!=NULL){
  		fprintf(stderr, "The command it's wrong.\n" );
  		return;
  	}
	print_lvar();
}