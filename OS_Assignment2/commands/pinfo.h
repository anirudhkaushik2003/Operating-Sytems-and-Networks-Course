#ifndef PINFO_H
#define PINFO_H

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>

#include "../utils/string.h"
#include "../utils/token_mat.h"
#include "../processor/tokenizer.h"

void pinfo(char *pid);

#endif