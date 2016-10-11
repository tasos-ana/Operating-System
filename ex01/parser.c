#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"


#define SIZE 1024

int curr_size=0;
char* buf;

/*Expanding the buffer if needed*/
void expand_buf(void){
	int new_size = curr_size + SIZE; /*Calculating the new size*/
	char* tmp = malloc(new_size*sizeof(char));
	assert(tmp!=NULL);
	assert(buf!=NULL);
	memmove(tmp,buf,curr_size);	/*Coming the old array in to the new*/
	destroy_buf();	/*Destroy the old array*/
	buf = tmp;
	curr_size = new_size;
}

/*Initialiaze the array.(called on start)*/
void init_buf(void){
	buf = malloc(SIZE*sizeof(char));
	assert(buf!=NULL);
	curr_size = curr_size + SIZE;
}

/*Destroy the array with free and null the pointer*/
void destroy_buf(void){
	assert(buf!=NULL);
	free(buf);
	buf = NULL;
	curr_size = 0;
}

/*Parsing the command from stdin and 
 *getting all the tokens at array buf
 */
char* parse_command(void){
	char c;
	int i = 0;
	init_buf();
	while((c = fgetc(stdin))!='\n'){
		if((curr_size-1) == i){
			expand_buf();
		}
		if(c != ' '){
			buf[i] = c;
			i++;
		}
	}
	buf[i] = '\0';
	return buf;
}
