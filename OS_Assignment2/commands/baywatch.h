#ifndef BAYWATCH_H
#define BAYWATCH_H

#include "../globals.h"
#include "../utils/string.h"
#include "../utils/token_mat.h"

void baywatch(token_mat args_mat);
void interrupt();
void dirty();

#endif