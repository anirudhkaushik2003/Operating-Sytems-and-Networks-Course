#include "pinfo.h"

job *jobs_list;
void pinfo(char *pid)
{
    int fd;
    char *svptr;
    char *BUFFER = malloc(sizeof(char) * BUFSIZ);
    char *token;
    printf("pid -- %s\n", pid);
    char *location1 = malloc(sizeof(char) * 100);
    char *location2 = malloc(sizeof(char) * 100);

    strcpy(location1, "/proc/");
    strcat(location1, pid);
    strcat(location1, "/"); //contains /proc/pid/

    //status
    strcpy(location2, location1);
    strcat(location2, "stat");
    char *symb = "+";
    fd = open(location2, O_RDONLY);
    if (fd == -1)
    {

        perror("\033[38;5;1mNo such process :( , maybe the process has already been reaped? \nError\033[0m");
    }
    else if (read(fd, BUFFER, 1000) < 0)
    {
        perror("Error");
    }
    else
    {

        token = strtok_r(BUFFER, " ", &svptr);
        int t = 2;
        while (t--)
        {
            token = strtok_r(NULL, " ", &svptr);
        }
        if (strcmp(token, "S") == 0 || strcmp(token, "R") == 0)
        {
            int processpid = atoi(pid);
            if (tcgetpgrp(0) == processpid)
            {
                symb = "+";
            }
            else
                symb = "";
        }
        else
        {
            symb = "";
        }
        printf("Process Status -- %s%s\n", token, symb);
    }
    close(fd);

    //memory
    strcpy(location2, location1);
    strcat(location2, "statm");
    fd = open(location2, O_RDONLY);
    if (fd == -1)
    {
        perror("\033[38;5;1mError\033[0m");
    }
    else
    {
        read(fd, BUFFER, 1000);
        token = strtok_r(BUFFER, " ", &svptr);
        printf("memory -- %s\n", token);
    }
    close(fd);

    //executable path
    strcpy(location2, location1);
    strcat(location2, "exe");

    int index = readlink(location2, BUFFER, 1000);
    if (index == -1)
    {
        fprintf(stderr, "\033[38;5;1mFailed to read %s\n", location2);
        perror("Error\033[0m");
    }
    else
    {
        int cond = 0;
        BUFFER[index] = '\0';
        string exe_path;
        string buf;
        buf.buf = malloc(sizeof(char) * 100);
        strcpy(buf.buf, BUFFER);
        string root;
        root.buf = malloc(sizeof(char) * 100);
        strcpy(root.buf, getenv("HOME"));
        buf.len = strlen(buf.buf);
        root.len = strlen(root.buf);
        if (buf.len >= root.len)
        {
            for (int i = 0; i < root.len; i++)
            {
                if (root.buf[i] != buf.buf[i])
                    cond = 1;
            }
            if (cond == 0)
                exe_path = remove_match(root, buf);
            else
                exe_path = buf;
        }

        else
        {
            exe_path = buf;
            cond = 1;
        }
        char *symb;

        if (cond == 0)
            symb = "~";
        else
            symb = "";
        printf("Executable Path -- %s%s\n", symb, exe_path.buf);
    }

    free(BUFFER);
    free(location1);
    free(location2);
}
