#include "pipe.h"

int output_redirection;

void writeTO(char *token)
{
    int fd = open(token, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    dup2(fd, STDOUT_FILENO);
}
void appendTO(char *token)
{
    int fd = open(token, O_CREAT | O_WRONLY | O_APPEND, 0666);
    dup2(fd, STDOUT_FILENO);
}
void readFROM(char *token)
{
    int fd = open(token, O_RDONLY);
    dup2(fd, STDIN_FILENO);
}

int *createpipe()
{
    int *fds = malloc(sizeof(int)*2);
    pipe(fds);
    return fds;
}

void pipeline(char *token, char *svptr, token_mat args_mat, string root)
{

    //initialization
    char delim[] = " \t\r\n\v\f";
    int i = 0;
    int original_out, original_in;
    original_out = dup(1);
    original_in = dup(0);
    int cond = 0;

    while (token != NULL)
    {
        
        if (i > 0 && strcmp(args_mat.args[i - 1], ">") == 0)
        {

            writeTO(token);
            token = strtok_r(NULL, delim, &svptr);
            i--;
            output_redirection = 1;
        }
        else if (i > 0 && strcmp(args_mat.args[i - 1], ">>") == 0)
        {

            appendTO(token);
            token = strtok_r(NULL, delim, &svptr);
            i--;
            output_redirection = 1;
        }
        else if (i > 0 && strcmp(args_mat.args[i - 1], "<") == 0)
        {

            readFROM(token);
            token = strtok_r(NULL, delim, &svptr);
            i--;
        }
        else
        {
            
            strcpy(args_mat.args[i], token);
            token = strtok_r(NULL, delim, &svptr);
            i++;
        }
    }

    if (i > 0)
    {
        free(args_mat.args[i]);
        args_mat.args[i] = NULL;
        args_mat.num_args = i;

        execute(args_mat, root);
        dup2(original_out, STDOUT_FILENO);
        dup2(original_in, STDIN_FILENO);
        output_redirection = 0;
        char *t = malloc(sizeof(char) * 100);
        strcpy(t, "");
        for (int j = 0; j < args_mat.num_args; j++)
        {
            strcat(t, args_mat.args[j]);
            strcat(t, " ");
        }
        for (int j = 0; j < HIST_LEN; j++)
        {
            if (strcmp(history_array[j], t) == 0)
            {
                cond = 1;
                break;
            }
        }

        // add command to history_array if it doesnt exist
        if (cond != 1)
        {
            strcpy(history_array[hist_index], t);

            hist_index++;
            hist_index = hist_index % HIST_LEN;
            perma_store();
        }
        free(t);
    }
}

/*tokenize again on the basis of |
    while (token != NULL)
{
    tokenize(input);
}*/