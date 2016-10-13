#ifndef SHELL_H
#define	SHELL_H

void display_prompt(void);

void tokenize(char *s);

char** parse_command(void);

void execute_cmd(char** buf);

#endif	/* SHELL_H*/