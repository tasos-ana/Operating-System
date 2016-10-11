#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#include "shell.h"

void display_prompt(void){
	char temp[1024];
	char* user_name = getlogin();
	char* dir_name  = getcwd(temp,1024);

	assert(user_name!=NULL);
	assert(dir_name!=NULL);

	printf("%s@cs345sh%s/$ ",user_name,dir_name);
}