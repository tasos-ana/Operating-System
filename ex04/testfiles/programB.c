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
	set_total_computation_time(-1,40);
	printf("Program B (%d) startRunning with total_time 40sec\n",getpid());
	int i;
	sleep(1);
	for(i=0; i<100000000; ++i){}
	printf("FINISHED - Program B (%d)\n",getpid());
	return 0;
}
/*
 * HY-345 / Exercise 4 / Scheduler
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */