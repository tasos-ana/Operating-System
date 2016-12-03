/*
 * HY-345 / Exercise 3 / System Calls
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/t_params.h>
#include <linux/slab.h>

#define EINVAL_ERROR  		22
#define EFAULT_ERROR		14

asmlinkage long set_total_computation_time(int pid, unsigned int total_time)
{
	printk("Tasos Anastasas - csd3166 - syscall 'set_total_computation_time'\n");
	struct task_struct *current_proccess;
	struct list_head *children_list;
	struct task_struct *children_task;
	current_proccess = current;
	if ( pid == -1 ){
		current_proccess->total_computation_time = total_time;
		current_proccess->remaining_time = total_time*10000000;
		current_proccess->infinite = 0;

		return (long)0;
	}
	if ( pid>=0 ){
		if (current_proccess->pid == pid){
			current_proccess->total_computation_time = total_time;
			current_proccess->remaining_time = total_time*10000000;
			current_proccess->infinite = 0;
			return (long)(0);
		}else{
			list_for_each(children_list,&current_proccess->children){
				children_task = list_entry(children_list,struct task_struct,sibling);
				if(children_task->pid == pid){
					children_task->total_computation_time = total_time;
					children_task->remaining_time = total_time*10000000;
					children_task->infinite = 0;
					return (long)(0);
				}
			}
		}	
	}
	return (long)( (-1) * (EINVAL_ERROR) ); 
	
}

asmlinkage long get_remaining_time(int pid, struct t_params *t_arguments)
{
	printk("Tasos Anastasas - csd3166 - syscall 'get_remaining_time'\n");
	struct t_params *t_tmp;
	struct task_struct *current_proccess;
	struct list_head *children_list;
	struct task_struct *children_task;
	current_proccess = current;

	children_task = NULL;
	if (t_arguments==NULL)
	{
		return (long)( (-1) * (EINVAL_ERROR) ); 
	}

	if ( pid == -1 ){
		t_tmp = (struct t_params*)kmalloc(sizeof(struct t_params), GFP_KERNEL);
				
		t_tmp->remaining_time = current_proccess->remaining_time;
		t_tmp->infinite = current_proccess->infinite;

		if (copy_to_user(t_arguments,t_tmp,sizeof(t_tmp))){
			return (long)((-1)*EFAULT_ERROR);
		}
		return (long)0;
	}
	if ( pid>=0 ){
		if (current_proccess->pid == pid){
			t_tmp = (struct t_params*)kmalloc(sizeof(struct t_params), GFP_KERNEL);
				
			t_tmp->remaining_time =  (current_proccess->remaining_time);
			t_tmp->infinite = current_proccess->infinite;

			if (copy_to_user(t_arguments,t_tmp,sizeof(t_tmp))){
				return (long)((-1)*EFAULT_ERROR);
			}
			return (long)(0);
		}else{
			list_for_each(children_list,&current_proccess->children){
				children_task = list_entry(children_list,struct task_struct,sibling);
				if(children_task->pid == pid){
					t_tmp = (struct t_params*)kmalloc(sizeof(struct t_params), GFP_KERNEL);
				
					t_tmp->remaining_time =  (children_task->remaining_time);
					t_tmp->infinite = children_task->infinite;

					if (copy_to_user(t_arguments,t_tmp,sizeof(t_tmp))){
						return (long)((-1)*EFAULT_ERROR);
					}
					return (long)(0);
				}
			}
		}	
	}
	return (long)( (-1) * (EINVAL_ERROR) ); 
}

/*
 * HY-345 / Exercise 3 / System Calls
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2016-2017
 */