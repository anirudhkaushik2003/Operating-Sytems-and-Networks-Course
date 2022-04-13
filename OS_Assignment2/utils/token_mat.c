#include "token_mat.h"
#include <stdlib.h>

token_mat create_matrix()
{
    token_mat args_mat;

    args_mat.args = malloc(sizeof(char *) * 100);
    for (int i = 0; i < 100; i++)
    {
        args_mat.args[i] = malloc(sizeof(char) * 100);
    }
    return args_mat;
}