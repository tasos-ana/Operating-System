/*
 * HY-345 / Exercise 4 / Scheduler
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "t_params.h"

#define __NR_set_total_computation_time 342
#define __NR_get_remaining_time 		343

#define set_total_computation_time(pid,total_time) syscall(__NR_set_total_computation_time,pid,total_time)
#define get_remaining_time(pid,t_arg) syscall(__NR_get_remaining_time,pid,t_arg)

int main(void)
{
	int status;
	pid_t pid;

	pid=fork();

	if(pid>0){
		set_total_computation_time(-1,80);
		printf("Program A (%d) startRunning with total_time 80sec\n",getpid());
		int i;
		for(i=0; i<1000000; ++i){}
		printf("FINISHED - Program A (%d)\n",getpid());
		waitpid(-1,&status,0);
	}else{
		pid=fork();
		if(pid>0){
			set_total_computation_time(-1,50);
			printf("Program B (%d) startRunning with total_time 50sec\n",getpid());
			int j;
			for(j=0; j<1000000; ++j){}
			printf("FINISHED - Program B (%d)\n",getpid());
			waitpid(-1,&status,0);
			exit(EXIT_SUCCESS);
		}else{
			pid=fork();
			if(pid>0){
				set_total_computation_time(-1,20);
				printf("Program C (%d) startRunning with total_time 20sec\n",getpid());
				int k;
				for(k=0; k<1000000; ++k){}
				printf("FINISHED - Program C (%d)\n",getpid());
				waitpid(-1,&status,0);
				exit(EXIT_SUCCESS);
			}else{
				set_total_computation_time(-1,10);
				printf("Program D (%d) startRunning with total_time 10sec\n",getpid());
				int l;
				for(l=0; l<1000000; ++l){}
				printf("FINISHED - Program D (%d)\n",getpid());
				exit(EXIT_SUCCESS);
			}
		}	
	}
}
/*
 * HY-345 / Exercise 4 / Scheduler
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */