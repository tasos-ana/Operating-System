/*
 * HY-345 / Exercise 3 / System Calls
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

void test_code(int pid, unsigned int time_no){
	struct t_params *t_data;
	printf("\t Trap to kernel level\n");
	if(set_total_computation_time(pid,time_no)==-1){
		perror("\t set_total_computation_time error");
	}else{
		printf("\t Set total time complete\n");	
	}
	printf("\t Back to user level\n\n");

	printf("\t Trap to kernel level\n");
	t_data = (struct t_params*)malloc(sizeof(struct t_params));
	if(get_remaining_time(pid,t_data)==-1){
		perror("\t get_remaining_time error");
	}else{
		printf("\t Get total time complete\n");
		printf("\t remaining_time: %d ---- infinite: %d \n",t_data->remaining_time, t_data->infinite);
	}
	printf("\t Back to user level\n");
	free(t_data);
}

int main(void)
{
	printf("Test:: --- pid = -2 (wrong proccess id)\n");
	test_code(-2,2);
}
/*
 * HY-345 / Exercise 3 / System Calls
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */