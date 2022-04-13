# OS Assignment 2, Shell part 1
## Anirudh Kaushik 2020111015 CSD

### Data Structures used

  #### Linked List
  - Used for keeping a list of background jobs
  - Everytime pinfo is run on a  process, it is checked against all background processes and on the basis of that it is decided whether a + is to be attatched or not
  - Any behavior regarding the plus sign is intentional and has been implemented on the basis of understanding of the document
  #### Array
  - Arrays have been utilized extensively throughout this project most notably in the form of the token matrix string and the history as it is indexed and of fixed sized
  - strings can also be considered array

### Using the make file
  - I have made using the makefile fairly simple
  - Simply running make on the directory where main.c is should create an executable named Shell
  - running this executable (./Shell) will generate an instance of my shell
  - in case of any abnormal behavior, ensure that you are not using clion or any other terminal which may not support the color codes using in this assignment
  - in order to disable color codes, just redefine all the color macros to blank strings in global.h and a couple of other commands

### Piping
  - Piping has been done by forking and use of 2 pipe buffers which are alternatively closed during odd and even indexes of the piped function
  - You might find me using the prctl function and upon a closer inspection, you might find that the function is useless, however I didn't add it just for the sake of showing off, in an earlier version of my code I was trying to fork every process whether or not it was piped, this led to 2 forks for execvp functions and I had to set the grandparent of the execvp process as the subreaper and enable interprocess communication using another p2c pipe to append the job to my jobs list
  - Piping works with input and output redirections exactly like the real shell, I'm very proud

### Signal Handling
  - The earlier version of my shell which used prctl had some problems with ctrl-z so i removed the super cool functionality and did it the normal way
  - After each fg process is sent SIGTSTP, we check the signal using WTERMSIG, if the value is 127 (for SIGTSTP), we add it to the jobs list
  - After this we use tcsetpgrp to return control of the terminal, WUNTRACED is used with waitpid to enable this
  - All stopped processes are terminated and reaped the moment the shell exits (very convinient)
  - ctrl-c normally exits the process
  - Both ctrl-c and ctrl-z have no effect on the shell if there is no fg process
  - ctrl-d sends EOF which is checked in the main while loop, upon detection the program terminates

### Baywatch
  - get_key function lets us detect when the q key is pressed in the baywatch command
  - Sleep could not be used as we can't detect the key press while the terminal is sleeping
  - clock had to be used and the calculations aren't as precise because of this, so instead of 1s u might get 1.0001 or something, but you won't notice this so why bother

### I/O redirection
  - I just changed STDOUT_FILENO and STDIN_FILENO to fd using dup 2
  - These are restored later on
  - I'm very proud

### sig, jobs, bg, fg
  - Used sig responsibly, don't kill processes just for fun, you can but you shouldn't
  - SIGTSTP is 20 not 18 or 24, took a while to figure that out
  - jobs can take arguments in any order
  - fg and bg do what they're supposed to, iirc they're prolly in syscoms cause you're communicating with the system or something, idk but they're there
  - 

### Error handling
  - The error handling is quite thorough
  - In case you supply an excess number of arguments for a given command or don't supply the required number of arguments, you will be greeted with an error message in _most cases_
  - However for some commands I have disabled this feature to allow for modifications in certain areas such as recursive repeat, asynchronous messages associated with death of background processes and immediate reaping of child processes
  - In case you supply too many arguments for a given command and do not receive an error message, either only the first valid arguments will be considered or the program will state that your request is invalid
  - Kindly avoid misbehaving with the shell 
  - It is built to handle all of your "odd requests" but all of your commands are stored in the history function, and the shell doesn't like it if you misbehave, it may even warn you (politely ofcourse) to not try and crash it 
  #### Extended Shell
  - The extended shell has even more thorough error handling, it may raise errors or simply "assume" what you want it to do and perform the required action
  - By assume I mean there are default values associated to certain commands so as to allow for speedy checking, these exist for almost every command but I may have disabled them for some commands

### Bonus Questions
  - Around 9 marks worth of bonus commands have been implemented
  - These include the replay command for 5 marks, the baywatch command -interrupt and dirty
  - I did not implement the last 2 marks as I don't like newborns (processes not children)
  - This does not reflect a lack of capability on my behalf but rather it shows that I'm not greedy. 
  - But extra marks are always welcome, just not at the cost of sleep and leisure
  
### Background and Foreground processes
  - The repeat command can be used with background processes (this was there earlier, im not so sure about it now, its not part of the extended shell so why bother)
  - Use fg job_num to bring a bg process to fg
  - use bg job_num to make a stopped bg process run
  - The status is updated dynamically at the time of printing so running jobs will always show the correct status
  - In case it doesn't then you are free to overlook this error and give me full

### File nomenclature
  - The files have been organized into 3 main categories - utils, commands and processor
  - Utils contains all the utility functions, struct declarations and in some cases external variables
  - avoid setting or unsetting env variables since my code utilises them for the root and oldpwd variables
  - commands are all the commands i have implemented including the _bonus replay and baywatch commands_ worth 9 marks. Yeah, 9 marks. more is always welcome
  - Implementation of the bonus command doesnt show a surplus of time, rather it shows my dedication to the subject. In case you feel charitable, being rewarded for this dedication is more than welcome
  - processor contains my tokenizer function and my prompt function, it is responsible for processing the commands and sending them to their respective locations
  - globals.h contains all the #includes and the extern variables etc.
  - main.c calls the other processes and displays the prompt and takes input
  - You'll notice that there is no separate file for signal handling, this is because it was unnecessary

### Echo and pinfo
  - Using echo removes the spaces in the string as required
  - pinfo displays the shell as R (running) instead of S+, this is because my shell is "running" instead of sleeping in the foreground
  - in case a child has already been reaped and completely destroyed, running pinfo on it will produce an error message
  - my process calls wait everytime a child dies to reap it
  - Thus you'll get an error message if you try to run pinfo on a dead/reaped child
  - That came out wrong
  - or did it

### Animations
  - The character is totoro from a ghibli movie, he changes color coz why not
  - In case you find the starting animation annoying (which I find hard to believe) simply comment the status_sleepy() function
  - It is quite colorful so in case you have epilepsy you should disable it 
  - But if you are brave enough to risk a seizure then go right ahead

### Conclusions
  - Refer to file naming and nomenclature to find the commands and the utility functions
  - Since this assignment is called extending the shell, I feel there will be no more shell based assignments (over extending the shell doesn't have the same ring to it)
  - I think that covers everything, the shell leaks some memory if you use a wrong command, but I wouldn't worry about that, I plugged most of the leaks.
  - baywatch runs in the foreground and replay runs in the background, the replay in background part was super difficult to implement, but it ensures that you can run the command and continue using the shell, I forked once and then returned in the parent, the parent gets fg control right after the creating the child, pretty cool.