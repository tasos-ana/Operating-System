/*
 * HY-345 / Exercise 2 / The Game of Life (Gol)
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */
#ifndef TABLE_H
#define TABLE_H

//Creating new life table
int** create_life_table(void);

//initialize the table with the data from filename file
void init_life_table(int** table, const char* filename);

//Printing all the data of life table, also change color on 1 to yellow
void print_life_table(int** table);

void destroy_life_table(int** table);

#endif //TABLE_H

 /*
 * HY-345 / Exercise 2 / The Game of Life (Gol)
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */