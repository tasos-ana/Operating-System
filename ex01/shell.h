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
void tokenize(char *s);

/*
 * Read the command and extract tokens from that cmd
 */
char** parse_command(void);

/*
 * Dispatching each command and call the correct function
 */
void execute_cmd(char** buf);

#endif	/* SHELL_H*/

/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */