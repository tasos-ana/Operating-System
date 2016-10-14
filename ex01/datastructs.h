/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */

#ifndef DATASTRUCTS_H
#define	DATASTRUCTS_H

typedef struct lvar_list{
	char* name;
	char* cmd;

	struct lvar_list* next;

}lvar_s , *lvar_p;

/*
 *Check if the struct it's empty
 */
int isEmpty_lvar(void);

/*
 *Create new node local var
 */
lvar_p create_lvar(char* name, char* cmd);

/*
 *Searching on struct if the node with name contained
 * and return that node or null
 */
lvar_p search_on_lvar(char* name);

/*
 * Searching for the name and get the field cmd
 */
char* get_lvar_cmd(char* name);

/*
 *Add new element on struct
 */
void set_lvar(char* name, char* cmd);

/*
 * Remove the node with name if exist
 */
void unset_lvar(char* name);

/*
 *Print all the active node from the list
 */
void print_lvar(void);

/*
 * Function for name validation, [a-zA-Z][0-9][_]
 */
int isvalid_lvar_name(char* name);

/*
 *Destroy all the struct, free and nullifie each node from
 * root to the end
 */
void destroy_lvar(void);

#endif	/* DATASTRUCTS_H*/

/*
 * HY-345 / Exercise 1 / make shell with basic cmd
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */