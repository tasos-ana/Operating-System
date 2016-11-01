/*
 * HY-345 / Exercise 2 / The Game of Life (Gol)
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>


#include "gol.h"
#include "table.h"

#define TRUE 1

extern print_range_row;
extern print_range_col;

int** life_table;
int** new_generation_table;
int total_generations = 0;
static int thread_counter = 0 ;

static pthread_mutex_t spawn_mutex;//Use that mutex each time that spawn new thread so we can read function input before change

//We use one counter that increase each time the a thread end with current generation, 
//we want critical region because maybe 2 thread can finish at the same time
static pthread_mutex_t thr_cnt_inc_mutex;

static sem_t new_generation_semaphore;//Wait for new season on each thread

static sem_t generation_end_semaphore;//Wait for signal when all thread and print finished so we can move on new generation

static sem_t thread_complete_semaphore;//When threads complete all the job for current generation main do something

static pthread_mutex_t critical_region;//when a thread want to change something that it's on another thread enter on critical region


//Return the coordinates (i,j) where thread table start, we make the mistake and count thread from 1... but it's not promblem
int* convert_threadID_coordinates(int id){
	assert(id>0 && id<=100);
	int* num = (int*)malloc(sizeof(int) * 2);

	if(id%10 == 0){
		num[0] = id - 10;
		num[1] = 100 - id + num[0];
	}else{
		num[0] = (id/10)*10;
		num[1] = ((id%10)- 1)*10;
	}
	return num;
}

//Start spwaning 100 threads
void spawn_threads(void){
	int i = 0;
	pthread_t pth;

	pthread_mutex_init(&spawn_mutex,NULL);//create the mutex that we will use for thread
	while(i<100){
		pthread_mutex_lock(&spawn_mutex);//entered on critical region
		i++;//increase counter.(because we count from 1)
		pthread_create(&pth,NULL,(void*)thread_function,(void*)i);//create thread and run the function also function unlock
	}
	pthread_mutex_lock(&spawn_mutex);//lock for last thread so we didnt exit from function i lost.
}

//Function that called from each thread
void* thread_function(void* arg){
	int threadID;
	int* num,i,j,new_state;

	threadID = (int) arg;//get the thread id so we can extract coordinates
	num = convert_threadID_coordinates(threadID);
	pthread_mutex_unlock(&spawn_mutex);//unlock for the next thread spwan

	while(TRUE){
		sem_wait(&new_generation_semaphore);//waiting for new generation
		//check all the inner table 8x8
		for(i=num[0]; i<num[0]+10; ++i){
			for(j=num[1]; j<num[1]+10; ++j){
				new_state = dead_or_alive(i,j);
				if(new_state){
					pthread_mutex_lock(&critical_region);//try to enter critical region to change the max size of col,row that will printed 
						if(print_range_row<i){
							print_range_row = i;
						}

						if(print_range_col<j){
							print_range_col = j;
						}
					pthread_mutex_unlock(&critical_region);//exit from critical region
				}
				new_generation_table[i][j] = new_state; 
			}
		}

		//enter on critical region se we can increase the thread completed counter
		pthread_mutex_lock(&thr_cnt_inc_mutex);
		thread_counter++;
		if(thread_counter==100) {
			sem_post(&thread_complete_semaphore);//if we are the last thread that end unblock main se we can move on next generation
			total_generations++;
		}
		pthread_mutex_unlock(&thr_cnt_inc_mutex);//exit critical region

		sem_wait(&generation_end_semaphore);//w8 for main signal when all thread end
	}
}

//Checking for specification x,y the state and the neighbors
int dead_or_alive(int x , int y){
	int neighbor = 0,i,j,state;
	int i_start=0,i_finish=100,j_start=0,j_finish=100;
	//calculate one square 3x3 around the x,y
	//also check if we are out of table
	if(x-1>0) i_start = x-1;
	if(x+1<100) i_finish = x+1+1;

	if(y-1>0) j_start = y-1;
	if(y+1<100) j_finish = y+1+1;

	//getting the state
	state = life_table[x][y];

	//calculate how many neighbor alive we have include the state,
	for(i = i_start; i < i_finish; ++i){
		for (j = j_start; j < j_finish; ++j){
			neighbor += life_table[i][j];
		}
	}

	//if the state was 1 we remove it from neighbor that calculated
	neighbor = neighbor - state;
	if(state){//checks for the future of the state
		if(neighbor<2) return 0;
		else if(neighbor<=3) return 1;
		return 0;//neighbor between 2-3
	}else{
		if(neighbor == 3) return 1;
		return 0;
	}
}

void sigfun(int sig){
	printf("tr - C pressed. Programm closing normally.\n");
	destroy_life_table(new_generation_table);
	destroy_life_table(life_table);
	exit(0);
}

int main(int argc, char const *argv[])
{
	(void) signal(SIGINT, sigfun);
	
	int i,j,threads;

	threads = 100;

	pthread_mutex_init(&critical_region,NULL);//mutex for critical region for each cell of table if it was critical
	pthread_mutex_init(&thr_cnt_inc_mutex,NULL);//mutex for thread counter
	sem_init(&new_generation_semaphore,0,0);//semaphore to w8 until the next generation start
	sem_init(&generation_end_semaphore,0,0);//semaphore to w8 all the thread to end
	sem_init(&thread_complete_semaphore,0,0);//semaphore when all thread complete

	if(argc==2){
		life_table = create_life_table();//create the table
		new_generation_table = create_life_table();//create the table
		init_life_table(life_table,argv[1]);//read the data and store it on table
	}else{
		fprintf(stderr, "Please give the input file with run command like './a.out test.txt'\n" );
		return 0;
	}
	
	spawn_threads();//spawn threads

	
	while(TRUE){
		thread_counter = 0;

		//make clear on screen - print and then wait
		system("clear");
		print_life_table(life_table);
		sleep(1);

		//new generation performed and unlock the 100 threads
		for(i=0;i<100;++i) sem_post(&new_generation_semaphore);

		//wait the threads to complete and get signal from the last complete thread
		sem_wait(&thread_complete_semaphore);

		for(i=0;i<100;++i){
			for(j=0;j<100;++j){
				life_table[i][j] = new_generation_table[i][j];
			}
		}

		//unblock the rest threads and let the me known that all threads completed
		for(i=0;i<100;++i) sem_post(&generation_end_semaphore);
	}
	return 0;
}

 /*
 * HY-345 / Exercise 2 / The Game of Life (Gol)
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */