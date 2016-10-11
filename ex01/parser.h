#ifndef PARSER_H
#define	PARSER_H

void init_buf(void);

void expand_buf(void);

void destroy_buf(void);

char* parse_command(void);

#endif	/* PARSER_H*/