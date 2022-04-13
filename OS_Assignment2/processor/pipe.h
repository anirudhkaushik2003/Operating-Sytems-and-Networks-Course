#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <sys/prctl.h>

#include "../globals.h"
#include "../utils/string.h"
#include "../utils/token_mat.h"



void pipeline(char *token, char *svptr, token_mat args_mat, string root);
int *createpipe();

#endif