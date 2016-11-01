 /*
 * HY-345 / Exercise 2 / The Game of Life (Gol)
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ZERO "\x1B[0m"
#define ONE "\x1B[31m"

extern int total_generations;

int print_range_row = 0;
int print_range_col = 0;

//Creating new life table
int** create_life_table(void){
	int i;
	int **arr = (int **)malloc(100 * sizeof(int *));
	assert(arr!=NULL);
   	for (i=0; i<100; ++i){
        arr[i] = (int *)malloc(100 * sizeof(int));
        assert(arr[i]!=NULL);
   	}

   	return arr;
}

//initialize the table with the data from filename file
void init_life_table(int** table, const char* filename){
	assert(filename!=NULL);
	FILE *myFile;
	myFile = fopen(filename,"r");
	if(myFile == NULL) {
		perror("cant open the file");
	}
	int i,j;
	for(i=0; i<100; ++i){
		for(j=0; j<100; ++j){
			fscanf(myFile,"%d",&table[i][j]);
			if(table[i][j] == 1) {
				print_range_row = i;
				if(print_range_col<j){
					print_range_col = j;
				}
			}
		}
	}

	fclose(myFile);
}

//Printing all the data of life table, also change color on 1 to yellow
void print_life_table(int** table){
	assert(table!=NULL);

	int i,j,num, curr_range_row,curr_range_col;

	curr_range_row = print_range_row + 5;
	curr_range_col = print_range_col + 5;

	if(curr_range_row>=100) curr_range_row = 100;
	if(curr_range_col>=100) curr_range_col = 100;
	for(i=0; i<curr_range_row; ++i){
		for(j=0; j<curr_range_col; ++j){
			num = table[i][j];
			if(num){
				printf("%s%d",ONE,num);
			}
			else printf("%s%d",ZERO,num);
		}
		printf("%s\n",ZERO);
	}
	printf("Generations: %d\n",total_generations);
}

//Why is that here? I dont known :D
void destroy_life_table(int** table){
	int i;
	for(i=0; i<100; ++i){
		free(table[i]); 
	}
	free(table);
}

  /*
 * HY-345 / Exercise 2 / The Game of Life (Gol)
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */
