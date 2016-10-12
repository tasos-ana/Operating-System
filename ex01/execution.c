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
  printf("set\n");
}

void execute_unset_var(char **buff){
	printf("unset\n");
}

void execute_printl_vars(char **buff){
	printf("print\n");
}