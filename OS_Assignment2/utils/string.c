#include "string.h"

string remove_match(string s1, string s2)
{
    int i, j;
    string output;
    output.buf = malloc(sizeof(char) * 100);
    s1.len = strlen(s1.buf);
    s2.len = strlen(s2.buf);
    if (s1.len < s2.len)
    {
        for (i = 0; i < s1.len; i++)
        {
            if (s1.buf[i] != s2.buf[i])
                return s2;
        }
        for (i = s1.len, j = 0; i < s2.len; i++, j++)
        {
            output.buf[j] = s2.buf[i];
        }
        output.buf[j] = '\0';
        return output;
    }
    else
        return s2;
}

