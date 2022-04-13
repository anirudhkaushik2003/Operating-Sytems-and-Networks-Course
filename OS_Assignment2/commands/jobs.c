#include "jobs.h"

job *jobs_list;

void printJobs(token_mat args_mat)
{
    char *status;
    job *p = jobs_list;
    int opt;
    // put ':' in the starting of the
    // string so that program can
    //distinguish between '?' and ':'
    int r = 0, s = 0;

    while ((opt = getopt(args_mat.num_args, args_mat.args, "rs")) != -1)
    {
        switch (opt)
        {
        case 'r':
            r = 1;
            break;
        case 's':
            s = 1;
            break;
        }
    }
    if (r == 0 && s == 0)
    {
        r = 1;
        s = 1;
    }
    int i = 1;
    while (p != NULL)
    {
        char *location = malloc(sizeof(char) * 100);
        strcpy(location, "/proc/");
        char *processID = malloc(sizeof(char) * 20);
        sprintf(processID, "%d/stat", p->pid);
        strcat(location, processID);

        int filedes = open(location, O_RDONLY);
        free(location);
        location = malloc(sizeof(char) * 100);
        if (read(filedes, location, 100) > 0)
        {

            char *status_token = strtok(location, " ");
            status_token = strtok(NULL, " ");
            status_token = strtok(NULL, " ");

            p->Status = status_token[0];

            if (p->Status == 'R')
            {
                status = "Running";
            }
            else if (p->Status == 'S' || p->Status == 'D')
            {
                status = "Running";
            }
            else if (p->Status == 'Z' || p->Status == 'z')
            {
                status = "Zombie";
            }
            else if (p->Status == 'T' || p->Status == 't')
            {
                status = "Stopped";
            }
            if (((p->Status == 'R') || (p->Status == 'S')) && r == 1)
                printf("[%d] %s %s [%d]\n", i, status, p->name, p->job_num);
            if (p->Status == 'T' && s == 1)
                printf("[%d] %s %s [%d]\n", i, status, p->name, p->job_num);

            free(location);
            i++;
        }
        p = p->next;
    }
    optind = 0;
}