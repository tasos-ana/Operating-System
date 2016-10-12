#ifndef EXECUTION_H
#define	EXECUTION_H

void execute_exit(char **buff);

void execute_simple(char **buff);

void execute_cd(char **buff);

void execute_pipe(char **buff);

void execute_redirection(char **buff);

void execute_set_var(char **buff);

void execute_unset_var(char **buff);

void execute_printl_vars(char **buff);

#endif	/* EXECUTION_H*/