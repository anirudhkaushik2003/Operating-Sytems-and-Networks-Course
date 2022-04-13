#include "exec.h"

job *jobs_list;
int num_of_jobs = 0;
int current_process;

int *p2c;

void Init_joblist()
{
    current_process = getpid();
    jobs_list = NULL;
}
void exec(token_mat args_mat)
{

    sigset_t new_mask, old_mask;
    sigaddset(&new_mask, SIGTSTP);

    //sigprocmask(SIG_SETMASK, &new_mask, &old_mask); //BLOCK ctrl z SIGNALS to prvent u from interrupting a foreground process

    int child = fork();

    if (child == -1)
    {
        perror("Could not fork child");
    }
    else if (child == 0)
    {

        if (strcmp(args_mat.args[args_mat.num_args - 1], "&") == 0)
        {
            args_mat.args[args_mat.num_args - 1] = NULL;
        }
        setpgid(0, 0);
        int p = 1;
        prctl(PR_SET_CHILD_SUBREAPER, &p);
        if (execvp(args_mat.args[0], args_mat.args) < 0)
        {
            perror("Error");
            exit(1);
        }
        exit(0);
    }
    else
    {
        int p = 1;
        prctl(PR_SET_CHILD_SUBREAPER, &p);

        if (strcmp(args_mat.args[args_mat.num_args - 1], "&") == 0)
        {

            job *new = malloc(sizeof(job));
            new->name = malloc(sizeof(char) * 100);
            strcpy(new->name, args_mat.args[0]);
            new->pid = child;

            char *location = malloc(sizeof(char) * 100);
            strcpy(location, "/proc/");
            char *processID = malloc(sizeof(char) * 20);
            sprintf(processID, "%d/stat", child);
            strcat(location, processID);
            int filedes = open(location, O_RDONLY);
            read(filedes, location, 100);
            char *status_token = strtok(location, " ");
            status_token = strtok(NULL, " ");
            status_token = strtok(NULL, " ");

            new->Status = status_token[0];

            if (jobs_list == NULL || strcmp(jobs_list->name, new->name) >= 0)
            {

                new->next = jobs_list;
                jobs_list = new;
                num_of_jobs++;
                new->job_num = num_of_jobs;
            }
            else
            {

                job *p = jobs_list;
                while (p->next != NULL && (strcmp(p->next->name, new->name) < 0))
                {

                    p = p->next;
                }
                new->next = p->next;
                p->next = new;
                num_of_jobs++;
                new->job_num = num_of_jobs;
                p = jobs_list;
            }

            printf("%d\n", child);
            setpgid(child, 0);
            tcsetpgrp(0, getpgrp());
        }
        else
        {
            current_process = child;
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            setpgid(child, 0);
            tcsetpgrp(0, __getpgid(child));
            int status;
            waitpid(child, &status, WUNTRACED | WCONTINUED);
            if (WTERMSIG(status) == 127)
            {

                job *new = malloc(sizeof(job));
                new->name = malloc(sizeof(char) * 100);

                new->pid = current_process;

                char *location = malloc(sizeof(char) * 100);
                strcpy(location, "/proc/");
                char *processID = malloc(sizeof(char) * 20);
                sprintf(processID, "%d/stat", current_process);
                strcat(location, processID);
                int filedes = open(location, O_RDONLY);
                if (read(filedes, location, 100) < 0)
                    perror("read");
                char *status_token = strtok(location, " ");
                status_token = strtok(NULL, " ");
                for (int k = 1; k < strlen(status_token) - 1; k++)
                {
                    new->name[k - 1] = status_token[k];
                }

                status_token = strtok(NULL, " ");

                new->Status = status_token[0];

                if (jobs_list == NULL || strcmp(jobs_list->name, new->name) >= 0)
                {

                    new->next = jobs_list;
                    jobs_list = new;
                    num_of_jobs++;
                    new->job_num = num_of_jobs;
                }
                else
                {

                    job *p = jobs_list;
                    while (p->next != NULL && (strcmp(p->next->name, new->name) < 0))
                    {

                        p = p->next;
                    }
                    new->next = p->next;
                    p->next = new;
                    num_of_jobs++;
                    new->job_num = num_of_jobs;
                    p = jobs_list;
                }
                printf("\n[%d] %d Stopped\n", num_of_jobs, current_process);
            }

            tcsetpgrp(0, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
        }
    }
}

void check_job(int sig)
{

    pid_t return_pid;
    job *p = jobs_list;
    int status = -1;
    job *prev = NULL;
    int i = 0;
    while (p != NULL)
    {
        //printf("Process %s with pid %d \n", p->name, p->pid);
        char *location = malloc(sizeof(char) * 100);
        strcpy(location, "/proc/");
        char *processID = malloc(sizeof(char) * 20);
        sprintf(processID, "%d/stat", p->pid);
        strcat(location, processID);
        int filedes = open(location, O_RDONLY);
        //printf("From the new world\n");
        return_pid = waitpid(p->pid, &status, WNOHANG);

        if (read(filedes, location, 100) < 0)
        {

            int exit_status = WEXITSTATUS(status);
            printf("\nProcess %s with pid %d has exited ", p->name, p->pid);
            printf(exit_status ? "with code 1 (abonormal)\n" : "normally\n");
            if (prev == NULL)
            {
                jobs_list = jobs_list->next;
                p = jobs_list;
            }
            else
            {
                prev->next = p->next;
            }

            num_of_jobs--;
        }

        if (p == NULL)
            break;
        prev = p;
        p = p->next;
        i++;
    }
}

string string_make(job *new)
{
    string out;
    out.buf = malloc(sizeof(char) * 100);
    sprintf(out.buf, "%d", new->pid);
    strcat(out.buf, " ");
    strcat(out.buf, new->name);
    strcat(out.buf, " ");
    char *stat = malloc(sizeof(char) * 10);
    stat[0] = new->Status;
    stat[1] = '\0';
    strcat(out.buf, stat);
    free(stat);
    out.len = strlen(out.buf);
    return out;
}

job *parse_str_to_job(string input)
{
    job *out = malloc(sizeof(job));
    out->name = malloc(sizeof(char) * 100);
    char *token = strtok(input.buf, " ");
    out->pid = atoi(token);
    token = strtok(NULL, " ");
    strcpy(out->name, token);
    token = strtok(NULL, " ");
    out->Status = token[0];

    return out;
}