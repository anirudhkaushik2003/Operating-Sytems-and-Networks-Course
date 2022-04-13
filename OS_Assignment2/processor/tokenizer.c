#include "tokenizer.h"

char **history_array;
int output_redirection;
int num_of_jobs;

job *jobs_list;
int shell_pid;

int *p2c;

void tokenize_string(string input, string root)
{

    char *copy = malloc(sizeof(char) * 100);
    strcpy(copy, input.buf);
    int original_out, original_in;
    original_out = dup(1);
    original_in = dup(0);

    char pipedelim[] = "|";
    char *svptr1 = input.buf;
    char *token1 = strtok_r(input.buf, pipedelim, &svptr1);

    char delim[] = " \t\r\n\v\f";
    token_mat args_mat;

    int num_pipes = -1;
    while (token1 != NULL)
    {
        num_pipes++;
        token1 = strtok_r(NULL, pipedelim, &svptr1);
    }
    token1 = strtok_r(copy, pipedelim, &svptr1);
    int i = 0;
    int *fds1, *fds2;
    fds1 = malloc(sizeof(int) * 2);
    fds2 = malloc(sizeof(int) * 2);

    while (token1 != NULL)
    {
        if (num_pipes == 0)
        {
            args_mat = create_matrix();
            char *svptr = token1;
            char *token = strtok_r(token1, delim, &svptr);

            pipeline(token, svptr, args_mat, root);
            token1 = strtok_r(NULL, pipedelim, &svptr1);
            for (int j = 0; j < 100; j++)
            {
                free(args_mat.args[j]);
            }
            free(args_mat.args);
        }
        else
        {
            int *current_pipe = i % 2 == 0 ? fds1 : fds2, *other_pipe = i % 2 == 0 ? fds2 : fds1;
            if (pipe(current_pipe) < 0)
            {
                perror("Error in pipeline, pipe returned negative integer");
                return;
            }
            args_mat = create_matrix();
            char *svptr = token1;
            char *token = strtok_r(token1, delim, &svptr);
            if (strcmp(token, "exit") == 0)
            {
                exit(0);
            }
            else if (strcmp(token, "fg") == 0)
            {
                pipeline(token, svptr, args_mat, root);
                return;
            }

            int child = fork();
            if (child == 0)
            {
                if (num_pipes > i)
                {
                    output_redirection = 1;
                    close(current_pipe[0]);
                    dup2(current_pipe[1], STDOUT_FILENO);
                }
                if (i > 0)
                {
                    close(other_pipe[1]);
                    dup2(other_pipe[0], STDIN_FILENO);
                }
                pipeline(token, svptr, args_mat, root);
                exit(0);
            }
            else
            {
                int p = 1;
                prctl(PR_SET_CHILD_SUBREAPER, &p);
                wait(NULL);

                if (num_pipes > i)
                {

                    close(current_pipe[1]);
                }
                if (i > 0)
                {

                    close(other_pipe[0]);
                }
            }

            output_redirection = 0;
            dup2(original_out, STDOUT_FILENO);
            dup2(original_out, STDIN_FILENO);
            token1 = strtok_r(NULL, pipedelim, &svptr1);
            i++;

            for (int j = 0; j < 100; j++)
            {
                free(args_mat.args[j]);
            }
            free(args_mat.args);
        }
    }
    free(fds1);
    free(fds2);

    free(copy);
}

void execute(token_mat args_mat, string root)
{
    if (strcmp(args_mat.args[0], "cd") == 0)
    {
        if (args_mat.num_args > 2)
        {

            printf("Error: too many arguments\n");

            return;
        }
        changedir(args_mat.args[1], root.buf);
    }
    else if (strcmp(args_mat.args[0], "pwd") == 0)
    {
        printf("%s\n", getpwd().buf);
    }
    else if (strcmp(args_mat.args[0], "echo") == 0)
    {
        echo_out(args_mat);
    }
    else if (strcmp(args_mat.args[0], "ls") == 0)
    {
        makeLS(args_mat.num_args, args_mat.args);
    }
    else if (strcmp(args_mat.args[0], "pinfo") == 0)
    {

        char *temp = malloc(sizeof(char) * 25);
        if (args_mat.num_args < 2)
        {
            sprintf(temp, "%d", shell_pid);
        }
        else if (args_mat.num_args > 2)
        {

            printf("Error: too many arguments\n");

            return;
        }
        else
            sprintf(temp, "%s", args_mat.args[1]);

        pinfo(temp);

        free(temp);
    }
    else if (strcmp(args_mat.args[0], "repeat") == 0)
    {
        if (args_mat.num_args < 3)
        {

            printf("Error: too few arguments\n");

            return;
        }
        Repeat(args_mat);
    }
    else if (strcmp(args_mat.args[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(args_mat.args[0], "history") == 0)
    {

        if (args_mat.num_args < 2)
        {
            args_mat.args[1] = malloc(sizeof(char) * 100);
            strcpy(args_mat.args[1], "10");
        }
        int num = 0, len = strlen(args_mat.args[1]);
        for (int i = 0; i < len; i++)
        {
            if (args_mat.args[1][i] <= '9' && args_mat.args[1][i] >= '0')
                num += (args_mat.args[1][i] - '0') * pow(10, (len - i - 1));
            else
            {

                printf("Error: 2nd Argument is not a number! Don't try to crash the shell\n");

                return;
            }
        }
        if (num > 20)
            num = 20;
        history(num);
    }
    else if (strcmp(args_mat.args[0], "jobs") == 0)
    {
        if (args_mat.num_args > 3)
        {
            perror("\033[38;5;1mError, too many arguments\033[0m");
        }
        else
            printJobs(args_mat);
    }
    else if (strcmp(args_mat.args[0], "sig") == 0)
    {
        sig(args_mat);
    }
    else if (strcmp(args_mat.args[0], "bg") == 0)
    {
        if (args_mat.num_args < 2)
        {
            perror("Requires an additional argument, job number");
        }
        else if (args_mat.num_args > 2)
        {
            perror("\033[38;5;1mError, too many arguments\033[0m");
        }
        else
            bg(args_mat);
    }
    else if (strcmp(args_mat.args[0], "fg") == 0)
    {
        if (args_mat.num_args < 2)
        {
            perror("Requires an additional argument, job number");
        }
        else
            fg(args_mat);
    }
    else if (strcmp(args_mat.args[0], "replay") == 0)
    {

        replay(args_mat,root);
    }
    else if (strcmp(args_mat.args[0], "baywatch") == 0)
    {

        baywatch(args_mat);
    }
    else
    {
        exec(args_mat);
    }
}
