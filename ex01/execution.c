#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "execution.h"
#include "datastructs.h"

void execute_exit(char **buff){
	printf("exit\n");
}

void execute_simple(char **buff){
	printf("simple\n");
}

void execute_cd(char **buff){
	printf("cd\n");
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