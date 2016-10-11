#ifndef DISPATCHER_H
#define	DISPATCHER_H

typedef void (*execute_func_t)(char*);

int get_cmd_type(char *buff);

void execute_simple(char *buff);

void execute_complex(char *buff);

void execute_exit(char *buff);

void execute_set_var(char *buff);

void execute_unset_var(char *buff);

void execute_printl_vars(char *buff);

void execute(char *buff,int cmd_type);

#endif	/* DISPATCHER_H*/