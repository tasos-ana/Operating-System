/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructs.h"


lvar_p lvar_list = NULL;//head of the local var struct

/*
 *Check if the struct it's empty
 */
int isEmpty_lvar(void){
	if(lvar_list==NULL) return 1;
	return 0;
}

/*
 *Create new node local var
 */
lvar_p create_lvar(char* name, char* cmd){
	assert(name!=NULL);
	assert(cmd!=NULL);
	lvar_p tmp = malloc(sizeof(lvar_s));
	assert(tmp!=NULL);
	tmp->name = strdup(name);//copying
	tmp->cmd = strdup(cmd);//copying
	tmp->next = NULL;
}

/*
 *Searching on struct if the node with name contained
 * and return that node or null
 */
lvar_p search_on_lvar(char* name){
	assert(name!=NULL);
	lvar_p head = lvar_list;

	if(isEmpty_lvar()) return NULL;
	
	while(head!=NULL){
		if(strcmp(head->name,name)==0){
			return head;
		}
		if(head->next!=NULL) {head=head->next;}
		else break;
	}
	return head;
}

/*
 * Searching for the name and get the field cmd
 */
char* get_lvar_cmd(char* name){
	lvar_p node = search_on_lvar(name);
	if(node!=NULL) return node->cmd;
	else return NULL;
}

/*
 *Add new element on struct
 */
void set_lvar(char* name, char* cmd){
	lvar_p index;

	assert(name!=NULL);
	assert(cmd!=NULL);
	if(isvalid_lvar_name(name)==0) return;//checking the validation of the name


	if(isEmpty_lvar()){//if it's empty we have the first node
		lvar_list = create_lvar(name,cmd);
		return;
	}
	index = search_on_lvar(name);
	if(strcmp(index->name,name)==0)	index->cmd = strdup(cmd);
	else				index->next = create_lvar(name,cmd);
}

/*
 * Remove the node with name if exist
 */
void unset_lvar(char* name){
	lvar_p head = lvar_list;

	assert(name!=NULL);
	if(isEmpty_lvar()){//error if the list it's empty
		fprintf(stderr, "fatal error- none variable defined.\n");
		return;
	}

	//if it's the first node 'destroy' the list, free,nullifie
	if(strcmp(head->name,name)==0){
		lvar_list = head->next;
		free(head);
		head = NULL;
		return;
	}

	while(head->next!=NULL){
		if(strcmp((head->next)->name,name)==0){
			lvar_p tmp = head->next;
			head->next = tmp->next;
			tmp->next = NULL;
			free(tmp);
			return;
		}
		head=head->next;
	}
	//if we didnt found it printing error
	fprintf(stderr, "fatal error- '%s' isnt defined.\n",name);
}

/*
 *Print all the active node from the list
 */
void print_lvar(void){
	lvar_p head = lvar_list;

	if(isEmpty_lvar()){//error if it's empty
	  printf("Local variables table it's empty.\n");
	}
	
	while(head!=NULL){
		printf("%s=%s\n", head->name , head->cmd);
		head = head->next;
	}
}

/*
 * Function for name validation, [a-zA-Z][0-9][_]
 */
int isvalid_lvar_name(char* name){
	int i = 0;
	
	assert(name!=NULL);
	while(name[i]!='\0'){
		if(isalpha(name[i]==0) && isdigit(name[i]==0) && name[i]!='_'){
			fprintf(stderr,"fatal error- variable name containe the invalid character (%c)\n",name[i]);
			return 0;
		}
		i++;
	}
	return 1;
}

/*
 *Destroy all the struct, free and nullifie each node from
 * root to the end
 */
void destroy_lvar(void){
	lvar_p head = lvar_list;
	lvar_p tmp;
	while(head!=NULL){
		tmp = head->next;
		head->next = NULL;
		free(head);
		head = tmp;
	}
	lvar_list = NULL;
}

/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */