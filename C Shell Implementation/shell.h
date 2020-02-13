/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */

#ifndef SHELL_H
#define	SHELL_H

/*
 *Displaying the prompt
 */
void display_prompt(void);

/*
 * Make tokens from speciefied string s
 */
char** tokenize(char *s,const char* c);

/*
 * Read the command and extract tokens from that cmd
 */
char** parse_command(void);

void run_cmd(char** buff);

/*
 * Dispatching each command and call the correct function
 */
int execute_cmd(char** buff);

/*
 *check for |,>,>>,<
 */
int execute_redirection_pipe(char** buff);

void destroy_buff(char** buff);

#endif	/* SHELL_H*/

/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */