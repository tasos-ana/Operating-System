*************************************************
* 	HY-345 / Exercise 3 / System Calls	*
* 	   Author: Anastasas Anastasios		*
* 		Code: csd3166			*
* 	       Year: 2016-2017			*	
*						*
*************************************************

~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.

Tropopoieseis apo tin exercice 3:
	-remaining_time = total_time*10000000; (einai micro seconds)
	-int remaining_time   (switch to ) u64 remaining_time
	
	-\include\linux\init_task.h
		:arxikopoiountai ta
			-remaining_time = 20000000 (2microseconds)
			-infinite	= 0

	-\include\linux\t_params.h
		int remaining_time	(switch to)	long long remaining_time

Ta system calls doulevoun opws kai stin exercice 03.

~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.

Eisagwgh kwdika egine se 2 arxeia: hint: (ctrl + f => CSD3166)

-kernel\sched_fair.c L422,L457
	-Dimiourgisa mia custom pick next entity function me tin opoia kanw diasxisi sto rb tree anadromika.(prodiatetagmenh)
	-Kanw extract ena sched_entity apo to node kai stin sunexeia to task struct apo afto to entity,
	-elegxetai an to current struct dn exei infinite brskoume to minimum metaksi tou curr node kai enos pou koubalame me to mikrotero
	  mexri tote.
	-afou epistrafei to node pou brethike an dn itan ola ta infinite 1 tote ginetai return, alliws pigainoume sto default node tou scheduler

~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.

-kernel\sched.c L2908
	-Stin sunartisis context switch egine add kwdikas opou:
		-ean to infinite dn einai 1
		-pernoume to sum_exec_runtime kai to prev_sum_exec_runtime kai ta aferoume antistoixa wste na broume poso xrono etrekse to proccess
			(sum_exec_runtime : total real time that proccess run on CPU)
			(prev_sum_exec_runtime: the previous real time that procces run on CPU)
		-elegxoume an to remaining einai <= apo ton xrono pou etrekse: diladi mas dilwse lathos time o xristis ara infinite=1
		-alliws aferoume ton xrono (sum) apo to remaining


~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.

Ta demo einai 5.

Ginetai sto kath' ena diaforetiko set_total_complete time kai trexei meta ena loop gia 1mirio fores.

Telos exw ftiaksei ena script arxeio me to opoio trexoun kai ta 5 arxeia meta to make fusika.

RUN CMD : 
	$ make
	$ sh run.sh














	