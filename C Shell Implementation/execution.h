/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */
#ifndef EXECUTION_H
#define	EXECUTION_H

/* 	Do all the action to exit from prompt
 *	destroy the local var table
 *	free the home dir that malloced
 */ 
void execute_exit(char **buff);

/*
 * Do all the action for a simple commants like ls,mv,rm,cat,sort...
 */
void execute_simple(char **buff);

/*
 *Doing the action for cmd: cd
 */
void execute_cd(char **buff);

void execute_pipe(char** buff);

/*
 * Code for redirection, input,output or both 
 */
void execute_redirection(char **buff);

/*
 *Code to execute the set var
 */
void execute_set_var(char **buff);

 /*
 *Code to execute the unset var
 */
void execute_unset_var(char **buff);

/*
 *Code to execute the printlv
 */
void execute_printl_vars(char **buff);

/*
 * Scouting buffer to learn if we have and where input(<) and/or output(>)
 */
void scout_buff(char** buff);

/*
 *Call this function to tokenize,parse the data from file when we have input redirection
 */
char** input_redirection(char* file);

/*
 *Redirection the stdout (1) to the file
 */
void output_redirection(char* file);

/*
 *Redirectio/append the stdout (1) to the file
 */
void append_redirection(char* file);

/*
 *Getting two tokens arrays and merge it in one
 */
char** merge_cmd_input(char** cmd,char** input_data);

/*
 * Export in a string all the tokens from the buff
 */
char* merge_tokens(char** buff);

#endif	/* EXECUTION_H*/
/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */