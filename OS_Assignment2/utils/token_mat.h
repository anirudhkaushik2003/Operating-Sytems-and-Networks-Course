#ifndef TOKEN_MAT
#define TOKEN_MAT


#include "string.h"


typedef struct token_mat token_mat;

struct token_mat
{
    char **args;
    int num_args;
};

token_mat create_matrix();

#endif