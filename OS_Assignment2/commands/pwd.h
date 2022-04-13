#ifndef PWD_H
#define PWD_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#include "../utils/string.h"

#define RED "\033[38;5;1m"
#define RESET "\033[0m"

string Initialize_root();
string getpwd();
string current_dir(string root);


#endif