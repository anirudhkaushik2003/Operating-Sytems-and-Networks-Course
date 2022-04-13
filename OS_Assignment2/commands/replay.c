#include "replay.h"
#include <getopt.h>

void replay(token_mat args_mat, string root)
{
    int c, interval = -1, period = -1;
    int j = 0;
    token_mat new_mat = create_matrix();
    static int verbose_flag;
    while (1)
    {
        static struct option long_options[] = {
            {"command", required_argument, NULL, 'c'},
            {"interval", required_argument, NULL, 'i'},
            {"period", required_argument, NULL, 'p'},
            {0, 0, 0, 0}};
        int option_index = 0;
        c = getopt_long_only(args_mat.num_args, args_mat.args, "", long_options, &option_index);

        if (c == -1)
            break;
        switch (c)
        {
        case 0:
            /* code */
            break;
        case 'c':
            strcpy(new_mat.args[j], optarg);
            j++;
            break;

        case 'i':
            interval = atoi(optarg);
            break;
        case 'p':
            period = atoi(optarg);
            break;

        default:;
        }
    }
    if (optind < args_mat.num_args)
    {
        while (optind < args_mat.num_args)
        {
            strcpy(new_mat.args[j], args_mat.args[optind++]);
            j++;
        }
    }
    if (interval > period)
    {
        perror("interval can't be greater than period");
        optind = 0;
        return;
    }
    if (strcmp(new_mat.args[0], "cd") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "pwd") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "echo") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "ls") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "pinfo") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "repeat") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "exit") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "history") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "jobs") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "sig") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "bg") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "fg") == 0)
    {
    }
    else if (strcmp(new_mat.args[0], "replay") == 0)
    {
    }
    else
    {
        strcpy(new_mat.args[j++], "&");
    }

    new_mat.num_args = j;
    free(new_mat.args[j]);
    new_mat.args[j] = NULL;

    int child = fork();
    optind = 0;

    if (child == 0)
    {
        int i = 0;
        period = period / interval;
        token_mat original = create_matrix();
        for (i = 0; i < new_mat.num_args; i++)
        {
            strcpy(original.args[i], new_mat.args[i]);
        }
        original.args[i] = NULL;
        original.num_args = i;

        for (int l = 0; l < period; l++)
        {
            for (i = 0; i < original.num_args; i++)
                strcpy(new_mat.args[i], original.args[i]);
            new_mat.args[i] = NULL;
            new_mat.num_args = i;
            execute(new_mat, root);

            sleep(interval);
        }
        exit(0);
    }
    else
    {
        return;
    }
}