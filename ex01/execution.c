#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "execution.h"
#include "datastructs.h"

extern char* home_dir;

int status;

void execute_exit(char **buff){
	destroy_lvar();
	free(home_dir);
	exit(0);
}

void execute_simple(char **buff){
	assert(buff[0]!=NULL);
	int pid;
	char* tmp[128];
	int i=0;
	
	while(buff[i]!=NULL){
	  tmp[i] = strdup(buff[i]);
	  i++;
	}
	tmp[i] = NULL;
	
	pid = fork();
	if(pid>0){
		waitpid(-1,&status,0);
	}else if(pid==0){
		if(strcmp(tmp[0],"echo")==0){
			char* cmd;
			cmd = get_lvar_cmd(tmp[1]);
			if(cmd!=NULL){
				printf("%s\n",cmd);
			}
		}
		char bin_path[128];
		strcpy(bin_path,"/bin/");
		strcat(bin_path,tmp[0]);
		execve(bin_path,&tmp[0],0);
	}else{
		perror("Fork failed\n");
	}
	
	i=0;
	while(tmp[i]!=NULL){
	  free(tmp[i]);
	  tmp[i]=NULL;
	  i++;
	}
}

void execute_cd(char **buff){
	if(buff[1] == NULL){
		chdir(home_dir);
	}else{
	  if(chdir(buff[1])==-1){
	    fprintf(stderr, "No such file or directory.\n");
	    return;
	  }
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