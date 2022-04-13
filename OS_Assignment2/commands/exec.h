#ifndef EXEC_H
#define EXEC_H

#define MAX_JOBS 50

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pwd.h>

#include "../utils/string.h"
#include "../utils/token_mat.h"

#include "../processor/tokenizer.h"

typedef struct jobs job;

struct jobs
{
    int pid;
    char *name;
    job *next;
    char Status;
    int job_num;
};

extern int current_process;
extern int num_of_jobs;
extern job *jobs_list;
void exec(token_mat args_mat);
void Init_joblist();
void check_job();
string string_make(job *new);
job *parse_str_to_job(string input);


#endif