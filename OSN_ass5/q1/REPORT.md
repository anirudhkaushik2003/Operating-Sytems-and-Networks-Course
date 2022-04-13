# README
## File info and how to run
 -run make followed by ./Simulation to run

## File structure
### course.c course.h
 - these contains initializations of the course threads and course thread utility functions

### lab.c and lab.h
 - these contains initializations of the lab threads and lab thread utility functions

### student.c student.h
 - these contains initializations of the student threads and student thread utility functions

### globals.c
 - assigning Tas checking some conditions and coducting tutorials happens here
 -locks are initialized

### main.c
 - scans input, processes it and stores it
 - creates threads and calls the init functions
 - waits on the threads

### globals.h
 - contains external variables and function prototypes

## working and entities
### student threads
 - these threads call register course function where they sleep for a fixed amount of time and then change their state and output the message
 - after this they try to apply for a tutorial, if there isnt one available for their current preference, they sleep on a condition variable
 - after waking up they check if their state changed, they check the state of the course before sleeping to make sure the course hasn't been withdrawn 
 - if course is withdrawn they update their preference and if they have no pref left they change state to exited and print the message and exit
 - if state is attending tut they do nothing since there is a tut going on
 - once the attending tut state changes they check their preference and apply for tut again
 - they are selected for tut by the course, they dont do anything other than update their current preference index
 - after selecting a course they exit the simulation

### course threads
 - these do most of the heavy lifting
 - after being initialized they check their state and if it is waiting for TA they call assign TA
 - the bonus has been implemented and the course thread checks if the TA is unassigned and if the TA's quota is less than the allowed limit and then it selects it if the TA has lesser number of taships than the current value of labs->bonus(which is set to 1 at the begining and incremented once every TA has taship equal to labs->bonus ensuring each TA is assigned only if the peers have already done the same number of tashps ensuring equal oppurtunities)
 - if ta is unassigned then locks are acquired and after acquiring the locks the unassigned condition is checked again and after this we check quota etc.
 - in the critical section (one lock for each TA) we update the state to assigned and then update taship etc
 - if the course's state is waiting slots we call conduct tut
 - conduct tut goes over the students whose current pref is the same as the course and then we change the students state(after acquiring student locks)
 - once the students have been selected (D students, D = W if W < D) we change their state to attending tut and then conduct the tut 
 - after the tut we update the ta states, course states, check if the student selected the course, update his current preference etc.
 - we then return and wait for a new TA to be assigned

### lab threads
 - these threads check whether the lab has TAs available or not and update the state of the labs
 - for bonus part, they update LABS[index].bonus to the next value for the TAs taships

### time thread
 - utility thread that detects deadlocks and helps in debugging

## bonus 
 - I have implemented the bonus
 - the bonus has been implemented and the course thread checks if the TA is unassigned and if the TA's quota is less than the allowed limit and then it selects it if the TA has lesser number of taships than the current value of labs->bonus(which is set to 1 at the begining and incremented once every TA has taship equal to labs->bonus ensuring each TA is assigned only if the peers have already done the same number of tashps ensuring equal oppurtunities)

## data structures and structs
 - all the entities are stored in an array of structs which are global variables shared across threads and protected by mutex locks
 - there are arrays of locks and conditional variables (used to simulate broadcast)

## deadlock prevention
 - we use a while statement when signalling a thread sleeping on a condition variable
 - we keep signalling that specific thread till its state is changed (confirmation that the thread woke up)
 - we use one condition variable for each thread for this exact purpose(so we can make sure it woke up like an alarm that doesnt stop ringing till you get up)

## Assumptions
 - one thread for each entity and for each lab course and student
 - we didn't busy wait for student waiting for a course as the pdf mentioned
 - other places, after confirming with the TA we may have busy waited
 - i say we in the sense that im a book, not the royal we
 - we as in both of us, u the reader and me the writer

## color
 - since the states were a lot we didnt use separate colors for each event
 - colors can be used to identify which part of the code the output is from rather than registering the output itself
 - same colors have been used for different events hence we can't give a key
 - red: used for exiting or empty labs
 - green: getting a course or seat 
 - in general red- something related to exiting, green- related to allocation, blue- related to TA assigning etc.
 