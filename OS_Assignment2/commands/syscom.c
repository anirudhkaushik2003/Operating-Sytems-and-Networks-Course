#include "syscom.h"
#include <stdbool.h>

job *jobs_list;

void sig(token_mat args_mat)
{
    bool found = false;
    int pid = atoi(args_mat.args[1]);
    int sigint = atoi(args_mat.args[2]);
    job *p = jobs_list;
    while (p != NULL)
    {
        if (p->job_num == pid)
        {
            kill(p->pid, sigint);
            //check_job();

            found = true;
            break;
        }
        p = p->next;
    }
    if (!found)
    {
        perror("No such job exists!");
    }
}

void bg(token_mat args_mat)
{
    int job_num = atoi(args_mat.args[1]);
    bool found = false;

    job *p = jobs_list;
    while (p != NULL)
    {
        if (p->job_num == job_num)
        {
            kill(p->pid, SIGTTIN);
            kill(p->pid, SIGCONT);
            //check_job();

            found = true;
            break;
        }
        p = p->next;
    }
    if (!found)
    {
        perror("No such job exists!");
    }
}

void fg(token_mat args_mat)
{

    int job_num = atoi(args_mat.args[1]);
    bool found = false;

    job *p = jobs_list;
    while (p != NULL)
    {
        if (p->job_num == job_num)
        {
            
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, p->pid);
            kill(p->pid, SIGCONT);

            int st;
            current_process = p->pid;
            waitpid(p->pid, &st, WUNTRACED);

            tcsetpgrp(0, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
            //check_job();

            found = true;
            break;
        }
        p = p->next;
    }
    if (!found)
    {
        perror("No such job exists!");
    }
}