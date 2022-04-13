# README
## File info and how to run
 - for compiling server side run make followed by ./server number of threads
 - for client run g++ client_sim.cpp -pthread then run ./a.out

## Program explanation 
 - the i/o format mentioned has been followed
 - server prints a lot of additional things
 - client follows the exact input format
 - there is a file called myqueue.cpp, using that for my queue was giving some errors so i used a normal stl queue
 - the data structure for the dictionary is an array of structs
 - we lock the mutex and if queue is empty we cond_wait till the something is pushed into the queue and the threads receive a signal
 - this is the case for all the worker threads, they wait on a cond variable and when the queue is not empty they are woken up using signal
 - muetx locks are used to make it thread safe
 - for the client side i make the threads while scanning the input as storing the arguments in an array causes "unknown crash on line 155 client_sim.cpp"
 - the input is processed before sending it to the server where it is tokenized and read, it uses the same functions that the tut code uses with some minor modifications
 - this concludes the explanation and the working 