# REPORT
## Changes made to the code
### Specification 1: syscall strace
  - In order to implement strace we first had to define the syscall number in syscall.h, this was chose to be 22 to preserve the order
  - After this we had to define its handler function in syscall.c, this was done in the struct for all the syscalls, the handler is uint64 sys_strace(void) which is a default prototype
  - The definition for this is included in sysproc.c where it calls the function strace(int mask)
  - strace is defined in proc.c close to the wait system call. it takes one argument which is the mask. 
  - sys_strace uses argaddr to obtain the argument from the command line which and passes it to strace
  - strace sets the p->mask of the process to this argument. p->mask was declared in proc.h
  - strace is declared in defs.h and user.h
  - we define another struct to keep track of the names and another to keep track of the number of arguments
  - in syscall() we obtain the arguments for the process using argaddr and we store the return value of the syscall separately before overwriting trapframe->a0 in order to preserve the values of the system calls
  - the return value is obtained from calling the handler function which returns the function it called which in turn returns the required return value
  - userprogram was added, run it using strace arguments in command line
  - this concludes the implementation of the strace system call

### Specification 2
  - Used compiler directives to determine the scheduler policy-> #ifdef SCHEDULER_NAME followed by the respective code
  - add CCFLAGS to the make file for the above purposed
#### FCFS
  - disabled clock interrupts
  - added ctime to proc.h which records the creation time of the process using p->ctime = ticks in allocproc
  - added another for loop to iterate over proc table and find the process with the minimum ctime
  - when this process is found, its pid is stored and in the next for loop we run the process if the pid matches
  - two for loops were used in order to preserve similarity with RR, in addition to this the two for loops prevent any edge cases that may arise due to the process being killed or the process sleeping, a single for loop implementation was considered, where after finding the process with the min ctime, we just schedule it, however this required the additional steps of storing it in a pointer and continuously updating the pointer and i preffered the simpler way which maintained the original xv6 for loop
  - after the process with the min ctime is run, it runs till it completes as per the specifications. the above procedure is repeated to find the next lowest ctime and so on
  - the changes were made in proc.c in allocproc to record the creation time and in scheduler to define the policy
  - this completes the implementation for FCFS
#### PBS
  - preemption was disabled
  - niceness, SP and DP variables were added to struct proc
  - niceness was initialized to 5, SP to 60 and DP to 0
  - niceness was computed right before scheduling in the scheduler function
  - using this DP was calculated
  - we iterate over all the processes once and find the process with the minimum DP, if two processes have the same value of DP then we choose the one with lower num_sched (num of times the process has been scheduled which starts at 0 and is incremented right before swtch when the process is scheduled) 
  - the sleep time is calculated as the difference between when the process is put to sleep using sleep() and when the process wakes up using wakeup
  - the run time is calculated as the difference between the time when the process is scheduled before swtch and after swtch when it just returns to scheduler
  - using this we calculate nicess and use that to calculate DP 
  - setpriority function was implemented using the steps mentioned for spec 1
  - if priority > 100 or < 0, it sets priority of the process to 100 and 0 resp
  - we iterate over all the processes to find the process whose pid == pid in the argument
  - if the SP decreases we call the scheduler function to reschedule as this is the only way DP can change
  - niceness is reset to 5
  - userprogram was added, run it using setpriority in command line
  - i disabled preemption except if setpriority causes a rescheduling
  - this concludes the implementation of PBS
#### MLFQ
  - This was by far the most complicated scheduler
  - A thorough understanding of the code's functioning and logic was required
  - Had to restart from scratch thrice
  - I decided to approach it in a step wise manner and let the original xv6 code handle most of the heavy work
  - this means that i cant use any additional data structures or system calls like some other ppl on github
  - the idea is that when the original code is reused in a modified manner, chances of getting an error are significantly reduced
  1. implemented time slicing by using code for RR but added an extra && condition that checks whether the time slice has been completed based on the priority. If it has been used up we yield the process just like RR
  2. I added a priority variable to determine the queue the process should be in, this simulates actual queues without implementing them
  3. I used this variable to run a random process with the minimum priority based and if the time slice was used, increment the priority in trap.c itself.
  4. after this worked, i added 5 queues in the form of a 2d array with 5 rows and 64 columns
  5. the scheduler functions adds the processes based on the priority to these queues and updates a variable i[5] for the corresponding value of the priority to keep track of the num of processes in each queue
  6. in order to make sure that a process that comes later is added to the end of queue 0, we keep an array of global variables which are always incremented and never decremented
  7. p->index is made equal to the corresponding global variable and the variable is incremented
  8. after adding all the processes from the proc table into the respective queues we sort the queues based on the index value mentioned earlier to ensure that processes are scheduled in order
  9. we used bubble sort for this. after sorting the first process is chosen from the first non empty queue with the highest priority
  10. after choosing this process we run another for loop to match its pid with each process and then run it just like in PBS and FCFS, 
  11. making the scheduler add the processes to the queues every time starting from index 0 means that if a process is removed it wont be added and we dont need to remove it from a queue
  12. this also allows us to simply change the value of the priority variable in order to change its queue as the scheduler function will do that by itself
  13. after this i decided to implement the sleep and wakeup exploit thing in which a process enters the same queue at the end
  14. in wakeup, i just set p->index to the global index for the respective queue and then incremented the variable which allowed an easy way to shift it to the end when the scheduler calls bubblesort
  15. we dont need to make any special changes to priority of a child process and a parent process as only RUNNABLE processes are added to the queue and since the queue is reset every time, any non runnable process is not added
  16. for aging i set the aging limit to 128. inside the update time function i added an additional condition that checks if the wait time is greater than 128 we decrement the priority by one unless our priority is 0
  17. the scheduler automatically puts this in a higher priority queue when scheduler is called
  18. the total time not running is used as the wait time as shown in the update time function
  PID 	 Priority 	 State 		 rtime 	 wtime 	 nrun 	 q0 	 q1 	 q2 	 q3 	 q4
    1 	 0 		 sleeping 	 1 	 284 	 22 	 287 	 127 	 0 	 0 	 0
    2 	 0 		 sleeping 	 0 	 411 	 10 	 411 	 0 	 0 	 0 	 0
    14 	 2 		 running 	 4 	 0 	 54 	 3 	 4 	 2 	 0 	 0
    15 	 2 		 running 	 4 	 0 	 3 	 2 	 4 	 2 	 0 	 0

    PID 	 Priority 	 State 		 rtime 	 wtime 	 nrun 	 q0 	 q1 	 q2 	 q3 	 q4
    1 	 0 		 sleeping 	 1 	 289 	 22 	 292 	 127 	 0 	 0 	 0
    2 	 0 		 sleeping 	 0 	 416 	 10 	 416 	 0 	 0 	 0 	 0
    14 	 3 		 running 	 9 	 0 	 56 	 3 	 4 	 8 	 4 	 0
    15 	 3 		 running 	 9 	 0 	 4 	 2 	 4 	 8 	 4 	 0

    PID 	 Priority 	 State 		 rtime 	 wtime 	 nrun 	 q0 	 q1 	 q2 	 q3 	 q4
    1 	 0 		 sleeping 	 1 	 301 	 22 	 304 	 127 	 0 	 0 	 0
    2 	 0 		 sleeping 	 0 	 428 	 10 	 428 	 0 	 0 	 0 	 0
    14 	 3 		 runble 	 21 	 0 	 79 	 3 	 4 	 8 	 28 	 0
    15 	 4 		 running 	 21 	 0 	 5 	 2 	 4 	 8 	 16 	 12

    PID 	 Priority 	 State 		 rtime 	 wtime 	 nrun 	 q0 	 q1 	 q2 	 q3 	 q4
    1 	 0 		 sleeping 	 1 	 305 	 22 	 308 	 127 	 0 	 0 	 0
    2 	 0 		 sleeping 	 0 	 432 	 10 	 432 	 0 	 0 	 0 	 0
    14 	 3 		 sleeping 	 25 	 0 	 81 	 3 	 4 	 8 	 36 	 0
    15 	 4 		 running 	 25 	 0 	 5 	 2 	 4 	 8 	 16 	 20

    PID 	 Priority 	 State 		 rtime 	 wtime 	 nrun 	 q0 	 q1 	 q2 	 q3 	 q4
    1 	 1 		 sleeping 	 90 	 31 	 831 	 781 	 209 	 0 	 0 	 0
    2 	 0 		 sleeping 	 0 	 899 	 8 	 899 	 0 	 0 	 0 	 0
    3 	 0 		 sleeping 	 101 	 130 	 92 	 140 	 259 	 136 	 150 	 287
    3270 	 3 		 running 	 15 	 0 	 4 	 2 	 6 	 10 	 12 	 0


  19. the above is the output for usertests forktest, all 4 priority queues are being used
  20. by taking advantage of the default behavior of xv6 we implement RR in queue 4 without any additional changes
  21. since the processes are added to the queues everytime scheduler run, i.e queues are rebuilt from scratch each time, a new process being added to q0 will be run automatically. incase a process in a lower queue is running when this happens, we simply pre empt it and run the higher priority process, we use yield inside kernel trap itself
  22. this concludes the implementation for MLFQ

### Specification 3: procdump
 - this was really easy since all i had to do was print the values i was using 
 - i used compiler directives to print whatever was required
 - total runtime is the same as the waitx runtime
 - for each queue we use q_wait to store the wait time in MLFQ
 - for PBS i print p->DP for priority
 - for MLFQ p->priority is the priority
 - wtime for PBS is p->total_wtime or time spent not running 
 - the implementation is pretty straightforward and there isnt any more to add

### Comparision using benchmark program (scheduler test and time usertests forktest)
 1. FCFS: Average rtime 94,  wtime 103, Average rtime 100,  wtime 111
 2. RR: Average rtime 43,  wtime 128, Average rtime 61,  wtime 142
 3. PBS: Average rtime 41,  wtime 116, Average rtime 84,  wtime 134
 4. MLFQ: Average rtime 43,  wtime 118, Average rtime 83,  wtime 135

 #### Inference
 - the first is running it normally and the second is when i run multiple xv6's with different schedulers on different terminals simultaneously
 - FCFS performs the best as far as wait time is concerned. processes are run instantly and dont have to wait, however this might just be because of the schedulertest function sending CPU intensive processes later on
 - the run time for FCFS is very very high, this may be because every process runs to completion without preemption, out of 10 processes if 2 or 3 CPU intensive processes run till completion followed by short IO processes, the run time will be very high due to adding up of the the runtimes for the CPU intensive processes. Also it takes more time for the CPU intensive processes to run and since its FCFS, if an IO process comes before a CPU process, it might have to wait for a longer time, then it runs for a longer time and a lot of clock ticks might be wasted in the context switch
 - RR perfoms poorly as far as wait time is concerned. Its run time is similar to the other processes. the wait times are so high due to the processes being pre empted after every tick which causes a lot of useless overhead. The run times are similar due to each process getting the same amount of time to run.
 - PBS seems to be well rounded, for larger cases PBS can be considered a good choice due to its criteria for DP which allows for sleeping processes to be scheduled quickly. However this is bad if there are a lot of IO intensive processes as IOs processes will get higher priority
 - MLFQ is the most sophisticated and based on processor availability it has given the lowest wait times. We chose average times to allow for a good comparision. the run time is the same as other processes with the wait time being comprable to PBS for the exact same input. However, this is due to the overhead incurred. For larger test cases, MLFQ might outperform PBS because the overhead for transferring queues will be worth it.
 - Hence PBS seems to be the most well rounded and favourable option but i have a personal preference for MLFQ due to its level of sophistication and also because it took a LOT of time to implement
 - Ive shown only 2 but i ran a lot of them and chose the best cases to explain

### How MLFQ policy could be exploited by a process
 - this was a very interesting question cause i saw some processes with higher total run times in q2 and q3 
 - A process can exploit the wakeup and go back to the same priority feature by voluntarily giving up control right before its time slice gets over and then it can keep doing this to stay in the higher priority queues
 - since it relinquishes control before its time slice is expired it can keep coming back to the same queue each time
 - i saw a couple of processes doing this but at the time i didnt notice and thought it was an error. Later on i realised that for ticks 4 and 8, a process was sleeping right before its time slice and coming back to the same queue. but this is very rare for q0 and q1 as the time intervals are too short to be able to pull this off
 - also the aging process allows processes to go to higher priority queues. processes like init and sh just stay in q0 with very long wait times. Very rarely, pid 2 goes to q1 but it comes back to q0 in a short time 
  
