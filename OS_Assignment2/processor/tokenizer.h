#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <math.h>

#include "../globals.h"
#include "../utils/string.h"
#include "../utils/token_mat.h"

#include "../commands/pinfo.h"

void tokenize_string(string input, string root);
void execute(token_mat args_mat, string root);


extern int *p2c;

#endif