#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "../utils/token_mat.h"

typedef struct string string;

struct string
{
    char *buf;
    int len;
};

string remove_match(string s1, string s2);


#endif