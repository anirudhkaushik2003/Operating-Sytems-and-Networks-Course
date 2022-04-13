#include "parser.h"

string *parse_info(char *input1, int len1)
{
    char *token;
    char *svptr = input1;
    token = strtok_r(input1, "-", &svptr);
    string *output1 = (string *)malloc(sizeof(string));
    output1->buf = malloc(sizeof(char) * 100);

    output1->len = len1;
    strcpy(output1->buf, token);
    return output1;
}