/*
 * HY-345 / Exercise 2 / The Game of Life (Gol)
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */
#ifndef GOL_H
#define GOL_H

//converting an ID of thread to x - y on table start
int* convert_threadID_coordinates(int id);

//spawning 100 threads
void spawn_threads(void);

//the function that called on each thread
void* thread_function(void* arg);

//check and return the future of the state[x][y]
int dead_or_alive(int x , int y);

#endif //GOL_H

 /*
 * HY-345 / Exercise 2 / The Game of Life (Gol)
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */