#include <stdlib.h>
#include <stdio.h>

#include "dispatcher.h"

int get_cmd_type(char *buff){

}

void execute_simple(char *buff){

}

void execute_complex(char *buff){

}

void execute_exit(char *buff){

}

void execute_set_var(char *buff){

}

void execute_unset_var(char *buff){

}

void execute_printl_vars(char *buff){
	
}

execute_func_t executor[]={
	execute_simple,
	execute_complex,
	execute_exit,
	execute_set_var,
	execute_unset_var,
	execute_printl_vars
};

void execute(char *buff,int cmd_type){
	(*executor[cmd_type])(buff);
}