# README
## File info and how to run
 -run make followed by ./Simulation to run

## File structure
### spec.h
 - contains spectator definitions and some mutex locks 
### globals.h
 - contains color definitions and all the functions and extern variables used throughout the code
 - contains semaphores, locks arrays of locks etc as extern variable declared and used in main.c and globals.c
### main.c
 - the code for scanning and processing input and storing it and then creating and waiting on the threads
 - it contains the timer threads which keeps track of time
 - this enables us to use a method similar to xv6 for checking run time sleep time etc. without busy waiting

### globals.c
 - contains thread initializations and zone entry functions which help simulate the entry into zones
 - the spectator info and group info is stored in global variables and is passed around using pointers to different functions

## program working and explanation
 - after we scan all the input we initialize a thread called timer, we don't call join on this thread
### time thread
 - this thread keeps track of all the events such as goals being scored updates the global variable called clock_time
 - when it is time for a thread to be woken up it calls signal and wakes up the sleeping threads
 - for the bonus question, the members from a group reach the gate and then sleep on a conditional variable (different variable to simulate behavior similar to broadcast) once all the members of a group are at the gate we wake up all the threads waiting on the group conditional variable
 - these threads then exit and leave for dinner 
 - the timer thread enables synchronization in an accurate manner 
 - the timer thread checks if its time for a goal chance and randomly check if a team scored a goal using rand function and outputs the result

### spectator threads
 - spectators get initialized and then call wait_seat. Here based on H,A or N a number of different threads are created
 - each of these threads wait on the zones corresponding to which a spectator can apply for a seat.
 - we use sem_timedwait to wait for a zone till absolute time becomes arrival time + patience
 - the arrival is simulated using a semaphore with 0 value and no posting to it
 - this semaphore is used only for timed wait and is not meant to be locked
 - after any of the threads corresponding to a spectator thread lock a seat in a zone, the other threads are blocked using a semaphore whose initial value is 1 and that semaphore is never posted to(can only be locked once by any thread)
 - once a seat is acquired we change the spectators state to watching match and go to sleep using cond_wait
 - in case number of goals by opposing team causes us to be enraged or the clocktime == etime (time at which we are supposed to exit = arrival time + X) the timer thread wakes us up
 - we then reach the gate and wait for our friends
 - in case we didnt get a seat we go exit all the threads for the zones and go to the gate and wait for our friends
 - we wait by sleeping on a conditional variable
 - when all the ppl in a grp reach the gate all the threads wakeup and exit the simulation together

## Bonus
 - The bonus was implemented, we wait for our friends before leaving  
 - no assumptions were made

## colors
 - red: someone leaves and goes to the gate for whatever reason
 - blue: someone got a seat in a zone
 - purple: waiting for friends at the exit
 - yellow: couldn't get a seat
 - white: a team score a  goal
 - green: someone reached the stadium