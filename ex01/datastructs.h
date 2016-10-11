#ifndef DATASTRUCTS_H
#define	DATASTRUCTS_H

typedef struct lvar_list{
	char* name;
	char* cmd;

	struct lvar_list* next;

}lvar_s , *lvar_p;

int isEmpty_lvar(void);

lvar_p create_lvar(char* name, char* cmd);

lvar_p search_on_lvar(char* name);

void set_lvar(char* name, char* cmd);

void unset_lvar(char* name);

void print_lvar(void);

int isvalid_lvar_name(char* name);

void destroy_lvar(void);

#endif	/* DATASTRUCTS_H*/