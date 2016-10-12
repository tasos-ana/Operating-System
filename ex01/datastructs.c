#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructs.h"

lvar_p lvar_list = NULL;

int isEmpty_lvar(void){
	if(lvar_list==NULL) return 1;
	return 0;
}

lvar_p create_lvar(char* name, char* cmd){
	assert(name!=NULL);
	assert(cmd!=NULL);
	lvar_p tmp = malloc(sizeof(lvar_s));
	assert(tmp!=NULL);
	tmp->name = strdup(name);
	tmp->cmd = strdup(cmd);
	tmp->next = NULL;
}

lvar_p search_on_lvar(char* name){
	lvar_p head = lvar_list;

	assert(name!=NULL);
	while(head!=NULL){
		if(strcmp(head->name,name)==0){
			return head;
		}
		if(head->next!=NULL) {head=head->next;}
		else break;
	}
	return head;
}

char* get_lvar_cmd(char* name){
	if(isEmpty_lvar()) return NULL;

	lvar_p head = lvar_list;
	while(head!=NULL){
		if(strcmp(head->name,name)==0){
			return head->cmd;
		}
		head=head->next;
	}
	return NULL;
}

void set_lvar(char* name, char* cmd){
	lvar_p index;

	assert(name!=NULL);
	assert(cmd!=NULL);
	if(isvalid_lvar_name(name)==0) return;

	if(isEmpty_lvar()){
		lvar_list = create_lvar(name,cmd);
		return;
	}
	index = search_on_lvar(name);
	if(strcmp(index->name,name)==0)	index->cmd = strdup(cmd);
	else				index->next = create_lvar(name,cmd);
}

void unset_lvar(char* name){
	lvar_p head = lvar_list;

	assert(name!=NULL);
	if(isEmpty_lvar()){
		fprintf(stderr, "fatal error- none variable defined.\n");
		return;
	}

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
	fprintf(stderr, "fatal error- '%s' isnt defined.\n",name);
}

void print_lvar(void){
	lvar_p head = lvar_list;

	if(isEmpty_lvar()){
	  printf("Local variables table it's empty.\n");
	}
	
	while(head!=NULL){
		printf("%s=%s\n", head->name , head->cmd);
		head = head->next;
	}
}

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