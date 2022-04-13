#ifndef SYSCOM_H
#define SYSCOM_H

#include "../utils/string.h"
#include "../utils/token_mat.h"

#include "../commands/exec.h"

void sig(token_mat args_mat);
void bg(token_mat args_mat);
void fg(token_mat args_mat);

#endif