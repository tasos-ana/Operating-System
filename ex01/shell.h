#ifndef SHELL_H
#define	SHELL_H

typedef struct lvar_list{
	char* name;
	char* cmd;

	struct lvar_list* next;

}lvar_s , *lvar_p;

void set_lvar(char* name, char* cmd);

void unset_lvar(char* name);

void print_lvar(void);

int validate_lvar_name(char* name);

void destroy_lvar(void);

void display_prompt(void);

void execute_cmd(char* buff);

#endif	/* SHELL_H*/