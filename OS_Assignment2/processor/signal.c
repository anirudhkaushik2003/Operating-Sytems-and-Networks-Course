#include "signal.h"

int shell_pid;
int current_process;

void signal_c(int sigint)
{
    if (current_process != getpid() && shell_pid != getpid())
        raise(SIGINT);
}

void signal_z(int sigint)
{
    
    if (current_process != getpid() && shell_pid != getpid())
    {
        
        raise(SIGTSTP);
    }
    
}

void init_signals() //called by main once
{
    signal(SIGINT, signal_c);
    signal(SIGTSTP, signal_z);
}