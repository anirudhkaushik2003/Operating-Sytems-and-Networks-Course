#include "repeat.h"
#include <math.h>

void Repeat(token_mat args_mat)
{
    int num = 0;
    int len = strlen(args_mat.args[1]);
    for (int i = 0; i < len; i++)
    {
        if (args_mat.args[1][i] <= '9' && args_mat.args[1][i] >= '0')
            num += (args_mat.args[1][i] - '0') * pow(10, (len - i - 1));
        else
        {
            printf(RESET);
            printf(RED);
            printf("Error: 2nd Argument is not a number! Don't try to crash the shell\n");
            printf(RESET);
            return;
        }
    }
    token_mat output;
    output = create_matrix();
    output.num_args = args_mat.num_args - 2;
    for (int i = 2, j = 0; i < args_mat.num_args; i++, j++)
    {
        strcpy(output.args[j], args_mat.args[i]);
    }
    output.args[output.num_args] = NULL;
    string root;
    root.buf = malloc(sizeof(char) * 100);
    strcpy(root.buf, getenv("HOME"));
    for (int i = 0; i < num; i++)
        execute(output, root);
    for (int i = 0; i < 100; i++)
    {
        free(output.args[i]);
    }
    free(output.args);
}